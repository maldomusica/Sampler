#include "Gain.h"

Gain::Gain() {}

Gain::~Gain() {}

//Modificar el Volumen
void Gain::setGainValue (float newGain)
{
	gainValue = newGain;
}

void Gain::process(juce::AudioBuffer<float>& buffer)
{
	for (int channel = 0; channel < buffer.getNumChannels(); channel++)
	{
		for (int i = 0; i < buffer.getNumSamples(); i++)
		{
			//Get Sample
			float x = buffer.getSample(channel, i);

			//Aply Gain Process
			float y = x * gainValue;

			buffer.setSample(channel, i, y);
		}
	}
}