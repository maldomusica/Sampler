#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SamplerAudioProcessorEditor::SamplerAudioProcessorEditor (SamplerAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p), myWave (p)
{
    setSize (700, 500);

    backgroundImage = juce::ImageCache::getFromMemory ( BinaryData::backg_png,
                                                        BinaryData::backg_pngSize);

    backgroundComponent.setImage(backgroundImage, juce::RectanglePlacement::stretchToFit);
    addAndMakeVisible(backgroundComponent);

    //glass = juce::ImageCache::getFromMemory(   BinaryData::glass_png,
    //                                           BinaryData::glass_pngSize);

    //glassComponent.setImage(backgroundImage, juce::RectanglePlacement::stretchToFit);
    //addAndMakeVisible(glassComponent);

    addAndMakeVisible(myWave);

    prepareSliders();
    setLookAndFeel(&knobLookAndFeel);
}

SamplerAudioProcessorEditor::~SamplerAudioProcessorEditor()
{
    setLookAndFeel(nullptr);
}

//==============================================================================
void SamplerAudioProcessorEditor::paint (juce::Graphics& g)
{

}

void SamplerAudioProcessorEditor::resized()
{
    backgroundComponent.setBounds(0, 0, getWidth(), getHeight()); 
    
    inGainSlider.setBounds(50, 180, 100, 100);
   
    myWave.setBounds(50, 10, myWave.getWidth(), myWave.getHeight());
}

void SamplerAudioProcessorEditor::prepareSliders()
{
    inGainSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    inGainSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    addAndMakeVisible(inGainSlider);

    inGainAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "inGain", inGainSlider);
}

bool SamplerAudioProcessorEditor::isInterestedInFileDrag(const juce::StringArray& files)
{
    for (auto file : files)
    {
        if (file.contains(".wav") || file.contains(".mp3") || file.contains(".aif") || file.contains(".aiff"))
        {
            return true;
        }
    }
    
    return false;
}

void SamplerAudioProcessorEditor::filesDropped(const juce::StringArray& files, int x, int y)
{
    for (auto file : files)
    {
        if (isInterestedInFileDrag(file)) 
        {
            // Load file
            auto myFile = std::make_unique<juce::File>(file);       //Linea de Yisas
            //fileName = myFile->getFileNameWithoutExtension() + myFile->getFileExtension();
            audioProcessor.loadFile (file);
        }
    }

    repaint();
}