#include "ADSR.h"

ADSR::ADSR() {}

ADSR::~ADSR() {}

void ADSR::prepare(double sampleRate)
{
    adsr.setSampleRate(sampleRate);
}

void ADSR::updateParameters(float attack, float decay, float sustain, float release)
{
    adsrParameters.attack = attack;
    adsrParameters.decay = decay;
    adsrParameters.sustain = sustain;
    adsrParameters.release = release;
    adsr.setParameters(adsrParameters);
}

void ADSR::applyEnvelope(juce::AudioBuffer<float>& buffer, int startSample, int numSamples)
{
    adsr.applyEnvelopeToBuffer(buffer, startSample, numSamples);
}