#pragma once
#include <JuceHeader.h>

class Pan
{
public:
	Pan();
	~Pan();

    void setPanningValue(float panValue);

    void process (juce::AudioBuffer<float>& buffer);

private:
    float PI = juce::MathConstants<float>::pi;

    float panningValue{ 0.0f };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Pan)
};