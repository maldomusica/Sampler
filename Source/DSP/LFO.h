#pragma once
#include <JuceHeader.h>
#include "DryWet.h"

class LFO
{
public:

    enum Type
    {
        Sine = 0,
        Square = 1,
        Sawtooth = 2,
        Triangle = 3
    };

    LFO();
    ~LFO();

    void setFrequencyValue (float frecValue);

    void prepare(double theSampleRate);
    void process(juce::AudioBuffer<float>& buffer);

    void setLfoType(Type newLfoType);

    float sineWave(int channel);
    float squareWave(int channel);
    float triangleWave(int channel);
    float sawtoothWave(int channel);

private:
    float sampleRate{ 48000.0f };
    float twoPI = juce::MathConstants<float>::twoPi;
    float frequency{ 0.0f };
    float time[2];
    float deltaTime[2];
    Type lfoType;

    DryWet lfoDryWet;
    juce::AudioBuffer<float> dryBuffer;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LFO)

};
