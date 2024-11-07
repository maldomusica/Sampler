#include "DryWet.h"

DryWet::DryWet() {}
DryWet::~DryWet() {}

void DryWet::setDryWetValue(float newDryWetValue)
{
    dryWetValue = newDryWetValue / 100.0f ;
}

void DryWet::process (  juce::AudioBuffer<float>& dryBuffer,
                        juce::AudioBuffer<float>& wetBuffer)
{
    for (int channel = 0; channel < dryBuffer.getNumChannels(); ++channel)
    {
        for (int i = 0; i < dryBuffer.getNumSamples(); ++i)
        {
            float drySample = dryBuffer.getSample(channel, i);
            float wetSample = wetBuffer.getSample(channel, i);

            float outSample = (drySample * (1 - dryWetValue)) + (wetSample * dryWetValue);

            wetBuffer.setSample(channel, i, outSample);
        }
    }
}