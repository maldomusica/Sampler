#pragma once
#include <JuceHeader.h>
#include <cmath>

class Distortion
{
public:

    Distortion();
    ~Distortion();

    void setGain (float inGain);
    void setDistortion (int inDistortionIndex);
    
    float softClipping (float inSample);
    float hardClipping (float inSample);
    float bitCrusher (float inSample);
    
    void process (juce::AudioBuffer<float>& inBuffer);

private:
    int distortionIndex { 0 };
    float distortionGain { 1.0f };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Distortion)
};

