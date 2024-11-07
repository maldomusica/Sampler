#pragma once
#include <JuceHeader.h>

class Gain
{
public:

		Gain();

		~Gain();


		void setGainValue(float newGain);
		void process(juce::AudioBuffer<float>& buffer);

private:

	float gainValue { 1.0f };

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Gain)
};
