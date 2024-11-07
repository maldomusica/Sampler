#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SamplerAudioProcessor::SamplerAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ),
                     apvts (*this, nullptr, "Parameters", createParameters())
#endif
{

    mFormatManager.registerBasicFormats();

    for (int i = 0; i < mNumVoices; i++)
    {
        mSampler.addVoice (new juce::SamplerVoice());
    }

}

SamplerAudioProcessor::~SamplerAudioProcessor()
{
    mFormatReader = nullptr;
}

juce::AudioProcessorValueTreeState::ParameterLayout SamplerAudioProcessor::createParameters()
{
    juce::AudioProcessorValueTreeState::ParameterLayout parameters;

    parameters.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{ "inGain", 1 }, "inGain", 0.0f, 2.0f, 1.0f));
    
    // ----------- DRIVE --------------------------------------------------------------------------------------------------------
    parameters.add(std::make_unique<juce::AudioParameterChoice>(juce::ParameterID{ "DistortionType", 1 }, "DistortionType",
                                    juce::StringArray{ "Soft Clipping", "Hard Clipping", "BitCrusher" }, 0));
    parameters.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{ "DistGain", 1 }, "DistGain", 0.0f, 5.0f, 0.5f));
    parameters.add(std::make_unique<juce::AudioParameterBool>(juce::ParameterID{ "DistBypass" , 1 }, "DistBypass", false));

    // ---------- MOD -----------------------------------------------------------------------------------------------------------
    parameters.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{ "LFORate", 1 }, "LFORate", 0.0f, 20.0f, 5.0f));
    parameters.add(std::make_unique<juce::AudioParameterBool>(juce::ParameterID{ "LFOBypass" , 1 }, "LFOBypass", false));
    parameters.add(std::make_unique<juce::AudioParameterChoice>(juce::ParameterID{ "LFOTypes", 1 }, "LFO Types", 
                                    juce::StringArray{ "Sine", "Square", "Saw", "Triangle" }, 0));
    parameters.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{ "LFODryWet", 1 }, "LFODrywet", 0.0f, 100.0f, 50.0f));


    // ---------- REGULAR PARAMS ------------------------------------------------------------------------------------------------

    // ADSR --------------------
    parameters.add(std::make_unique<juce::AudioParameterFloat>("Attack", "Attack", 0.1f, 5000.0f, 0.2f)); 
    parameters.add(std::make_unique<juce::AudioParameterFloat>("Decay", "Decay", 0.1f, 5000.0f, 1.0f)); 
    parameters.add(std::make_unique<juce::AudioParameterFloat>("Sustain", "Sustain", 0.0f, 1.0f, 1.0f)); 
    parameters.add(std::make_unique<juce::AudioParameterFloat>("Release", "Release", 0.1f, 5000.0f, 1.0f));

    float PI = juce::MathConstants<float>::pi;
    parameters.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{ "Pan", 1 }, "Pan", 0.0f, PI/2.0f, PI/4.0f));
    
    parameters.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{ "outGain", 1 }, "outGain", 0.0f, 2.0f, 1.0f));

    parameters.add(std::make_unique<juce::AudioParameterChoice>(juce::ParameterID{ "FilterType", 1 }, "FilterType", 
                                                                juce::StringArray{ "LowPass", "HighPass", "BandPass" }, 0));

    //parameters.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{ "Cutoff", 1 }, "Cutoff", juce::NormalisableRange<float>(20.0f, 20000.0f, 1.0f, 0.25f), 100.0f));
    
    //parameters.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{ "DryWet", 1 }, "DryWet", 0.0f, 100.0f, 50.0f));


    return parameters;
}

//==============================================================================
const juce::String SamplerAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool SamplerAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool SamplerAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool SamplerAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double SamplerAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int SamplerAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int SamplerAudioProcessor::getCurrentProgram()
{
    return 0;
}

void SamplerAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String SamplerAudioProcessor::getProgramName (int index)
{
    return {};
}

void SamplerAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void SamplerAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    mSampler.setCurrentPlaybackSampleRate (sampleRate);

    adsr.prepare(sampleRate);

    lfo.prepare(sampleRate);

    // ----- Filter Prepare ------
    juce::dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.numChannels = getTotalNumOutputChannels();
    spec.maximumBlockSize = samplesPerBlock;

    filter.prepare(spec);

}

void SamplerAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool SamplerAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void SamplerAudioProcessor::updateParameters()
{
    float inGainValue = *apvts.getRawParameterValue("inGain");
    inGain.setGainValue(inGainValue);
    
    // ----------- DRIVE --------------------------------------------------------------------------------------------------------
    float distortionType = *apvts.getRawParameterValue("DistortionType");
    float distGainValue = *apvts.getRawParameterValue("DistGain");
    bool inDistBypassValue = *apvts.getRawParameterValue("DistBypass");

    distortion.setDistortion(distortionType);
    distortion.setGain(distGainValue);
    distBypass = inDistBypassValue;

    // ---------- MOD -----------------------------------------------------------------------------------------------------------
    float inLFORateValue = *apvts.getRawParameterValue("LFORate");
    bool inLFOBypassValue = *apvts.getRawParameterValue("LFOBypass");
    float inLFODryWetValue = *apvts.getRawParameterValue("LFODryWet");
    auto inLFOType = apvts.getRawParameterValue("LFOTypes")->load();

    lfo.setFrequencyValue(inLFORateValue);
    lfoBypass = inLFOBypassValue;
    //lfo.setDryWetValue = inLFODryWetValue;
    lfo.setLfoType((LFO::Type)inLFOType);


    // ---------- ADSR ----------------------------------------------------------------------------------------------------------
    auto attack = apvts.getRawParameterValue("Attack")->load();
    auto decay = apvts.getRawParameterValue("Decay")->load();
    auto sustain = apvts.getRawParameterValue("Sustain")->load();
    auto release = apvts.getRawParameterValue("Release")->load();

    // ---------- FILTER ---------------------------------------------------------------------------------------------------------
    int filterType = apvts.getRawParameterValue("FilterType")->load();
    filter.updateParameters((Filter::FilterType) filterType, 500.0f, 0.7f); //casteo para tipo de filtro

    
    // ---------- GENERAL -------------------------------------------------------------------------------------------------------
    float inPanValue = *apvts.getRawParameterValue("Pan");
    float outGainValue = *apvts.getRawParameterValue("outGain");

    panning.setPanningValue(inPanValue);
    outGain.setGainValue(outGainValue);


}

void SamplerAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    mSampler.renderNextBlock (buffer, midiMessages, 0, buffer.getNumSamples());

    updateParameters();

    inGain.process (buffer);

    if (! distBypass)
    {
        distortion.process(buffer);
    }
        
    if (! lfoBypass)
    {
        lfo.process (buffer);
    }

    // Filters
    filter.process(buffer);

    // ---------------- ADSR ------------------------------------------------------------------------------------
    //adsr.updateParameters(attack, decay, sustain, release);
    //adsr.applyEnvelope(buffer, 0, buffer.getNumSamples());

    panning.process(buffer);

    outGain.process(buffer);

    
}

void SamplerAudioProcessor::loadFile()
{
    juce::FileChooser chooser{ "Carga un archivo" };

    if (chooser.browseForFileToOpen())
    {
        auto file = chooser.getResult();
        mFormatReader = mFormatManager.createReaderFor(file);
    }

    juce::BigInteger range;
    range.setRange(0, 128, true);

    mSampler.addSound(new juce::SamplerSound("Sample", *mFormatReader, range, 60, 0.1, 0.1, 10.0));
}

void SamplerAudioProcessor::loadFile(const juce::String& path)
{
    mSampler.clearSounds();

    auto file = juce::File(path);
    mFormatReader = mFormatManager.createReaderFor(file);

    auto sampleLength = static_cast<int>(mFormatReader->lengthInSamples);
    waveForm.setSize(1, sampleLength);
    mFormatReader->read(&waveForm, 0, sampleLength, 0, true, false);

    //auto buffer = waveForm.getReadPointer(0);

    juce::BigInteger range;
    range.setRange(0, 128, true);
    mSampler.addSound(new juce::SamplerSound("Sample", *mFormatReader, range, 60, 0.1, 0.1, 10.0));
}

//==============================================================================
bool SamplerAudioProcessor::hasEditor() const
{
    return true;
}

juce::AudioProcessorEditor* SamplerAudioProcessor::createEditor()
{
    return new SamplerAudioProcessorEditor (*this);
}

//==============================================================================
void SamplerAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    std::unique_ptr<juce::XmlElement> xml (apvts.state.createXml());
    copyXmlToBinary(*xml, destData);
}

void SamplerAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));

    if (xmlState->hasTagName (apvts.state.getType() ))
    {
        auto state = juce::ValueTree::fromXml(*xmlState);
        apvts.replaceState(state);
    }
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SamplerAudioProcessor();
}
