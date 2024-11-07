#pragma once
#include <JuceHeader.h>

class ADSR
{
public:
    ADSR();
    ~ADSR();

    void prepare(double sampleRate);
    void updateParameters(float attack, float decay, float sustain, float release);
    void applyEnvelope(juce::AudioBuffer<float>& buffer, int startSample, int numSamples);

private:
    juce::ADSR adsr;
    juce::ADSR::Parameters adsrParameters;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ADSR)
};
