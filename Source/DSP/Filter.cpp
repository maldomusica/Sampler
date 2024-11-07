#include "Filter.h"

void Filter::prepare(const juce::dsp::ProcessSpec& spec)
{
    filter.prepare(spec);
}

void Filter::process(juce::AudioBuffer<float>& inBuffer)
{
    juce::dsp::AudioBlock<float> block (inBuffer);

    juce::dsp::ProcessContextReplacing<float> context(block);
    filter.process(context);
}

void Filter::updateParameters(FilterType filterType, float cutoffFrequency, float resonance)
{
    this->cutoffFrequency = cutoffFrequency;
    this->resonance = resonance;

    filter.setCutoffFrequency(cutoffFrequency); // Establece la frecuencia de corte
    filter.setResonance(resonance); // Establece la resonancia

    // Establecer los parámetros según el tipo de filtro
    switch (filterType)
    {
    case FilterType::LowPass:
        filter.setType(juce::dsp::StateVariableTPTFilterType::lowpass);
        break;
    case FilterType::HighPass:
        filter.setType(juce::dsp::StateVariableTPTFilterType::highpass);
        break;
    case FilterType::BandPass:
        filter.setType(juce::dsp::StateVariableTPTFilterType::bandpass);
        break;
    }
}