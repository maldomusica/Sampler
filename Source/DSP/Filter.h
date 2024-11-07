#pragma once
#include <JuceHeader.h>

class Filter
{
public:
    enum class FilterType { LowPass, HighPass, BandPass };

    Filter() = default;
    ~Filter() = default;

    void prepare(const juce::dsp::ProcessSpec& spec);
    void process(juce::AudioBuffer<float>& inBuffer);
    void updateParameters(FilterType filterType, float cutoffFrequency, float resonance);

private:
    //juce::dsp::ProcessorDuplicator<juce::dsp::StateVariableTPTFilter<float>, juce::dsp::StateVariableTPTFilter<float>::Parameters> filter;
    juce::dsp::StateVariableTPTFilter<float> filter;
    float cutoffFrequency = 1000.0f;
    float resonance = 0.707f;
};

