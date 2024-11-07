#include "StateVariableFilter.h"

StateVariableFilter::StateVariableFilter() {}
StateVariableFilter::~StateVariableFilter() {}

void StateVariableFilter::prepare(juce::dsp::ProcessSpec& spec)
{
	stateVariableFilter.prepare(spec);
}

void StateVariableFilter::process(juce::AudioBuffer<float>& buffer)
{
	auto audioBlock = juce::dsp::AudioBlock<float>(buffer);
	auto context = juce::dsp::ProcessContextReplacing<float>(audioBlock);

	stateVariableFilter.process(context);
}

void StateVariableFilter::updateFilter(float cutoffFrec, float resonance, int filterType)
{
	stateVariableFilter.state->type = static_cast<juce::dsp::StateVariableFilter::Parameters<float>::Type>(filterType);
	stateVariableFilter.state->setCutOffFrequency(48000.0, cutoffFrec, resonance);
}