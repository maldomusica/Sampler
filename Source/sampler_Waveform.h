#pragma once
#include <JuceHeader.h>
#include "PluginProcessor.h"

class sampler_Waveform : public juce::Component
{
public:
	sampler_Waveform (SamplerAudioProcessor&);
	~sampler_Waveform () override;

    void paint(juce::Graphics&) override;
    void resized() override;

private:

	std::vector<float> audioPoints;
	juce::String fileName{ "" };

	SamplerAudioProcessor& audioProcessorMine;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(sampler_Waveform)
};
