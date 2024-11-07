#pragma once
#include <JuceHeader.h>
#include "DSP/Gain.h"
#include "DSP/Distortion.h"
#include "DSP/LFO.h"
#include "DSP/ADSR.h"
#include "DSP/Pan.h"
#include "DSP/Filter.h"

//==============================================================================
class SamplerAudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    SamplerAudioProcessor();
    ~SamplerAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    void loadFile();
    void loadFile (const juce::String& path);
    int getNumSamplerSounds() { return mSampler.getNumSounds(); }
    juce::AudioBuffer<float>& getWaveform() { return waveForm; }

    juce::AudioProcessorValueTreeState apvts;
    juce::AudioProcessorValueTreeState::ParameterLayout createParameters();

    void updateParameters();

private:

    Gain inGain;
    Distortion distortion;
    bool distBypass;
    Gain outGain;

    LFO lfo;
    bool lfoBypass;

    ADSR adsr;

    Filter filter;

    Pan panning;
        
    //Sampler ----------------------------------------------------------------------
    juce::AudioBuffer<float> waveForm;

    juce::Synthesiser mSampler;
    const int mNumVoices { 3 };

    juce::AudioFormatManager mFormatManager;
    juce::AudioFormatReader* mFormatReader { nullptr };

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SamplerAudioProcessor)
};
