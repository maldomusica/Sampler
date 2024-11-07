#pragma once
#include <JuceHeader.h>

class DryWet {

public:

    DryWet();
    ~DryWet();

    void setDryWetValue(float newDryWetValue);

    void process(   juce::AudioBuffer<float>& dryBuffer, 
                    juce::AudioBuffer<float>& wetBuffer);

private:

    float dryWetValue{ 0.5f };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DryWet)
};