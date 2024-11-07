#pragma once
#include <JuceHeader.h>

class StateVariableFilter
{
public:
	StateVariableFilter();
	~StateVariableFilter();

	void prepare(juce::dsp::ProcessSpec& spec);
	void process(juce::AudioBuffer<float>& buffer);
	void updateFilter(float cutoffFrec, float resonance, int filterType);

private:
	juce::dsp::ProcessorDuplicator<juce::dsp::StateVariableFilter::Filter<float>, 
	juce::dsp::StateVariableFilter::Parameters<float>> stateVariableFilter;
};