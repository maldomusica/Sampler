#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "sampler_Waveform.h"
#include "LookAndFeel/KnobLookAndFeel.h"

//==============================================================================
class SamplerAudioProcessorEditor  : public juce::AudioProcessorEditor,
                                     public juce::FileDragAndDropTarget

{
public:
    SamplerAudioProcessorEditor (SamplerAudioProcessor&);
    ~SamplerAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    
    bool isInterestedInFileDrag(const juce::StringArray& files) override;
    void filesDropped (const juce::StringArray& files, int x, int y) override;

    void prepareSliders();

private:

    std::vector<float> audioPoints;
    juce::String fileName{ "" };

    //-----BACKGROUND----
    juce::Image backgroundImage;
    juce::ImageComponent backgroundComponent;

    //-----WAVEFORM------
    sampler_Waveform myWave;

    //-----SLIDERS-------
    juce::Slider inGainSlider;
    juce::Label inGainLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> inGainAttach;

    KnobLookAndFeel knobLookAndFeel;

    //-----IMAGES--------
    /*juce::Image glass;
    juce::ImageComponent glassComponent;*/

    SamplerAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SamplerAudioProcessorEditor)
};
