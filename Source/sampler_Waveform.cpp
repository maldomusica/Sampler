#include "sampler_Waveform.h"

sampler_Waveform::sampler_Waveform(SamplerAudioProcessor& p) : audioProcessorMine(p)
{
	setSize (600, 200);
}

sampler_Waveform::~sampler_Waveform()
{
}

void sampler_Waveform::paint(juce::Graphics& g)
{
	auto waveform = audioProcessorMine.getWaveform();

	if (waveform.getNumSamples() > 0)
	{
		juce::Path p;
		audioPoints.clear();

		auto ratio = waveform.getNumSamples() / 600;
		auto buffer = waveform.getReadPointer(0);

		for (int sample = 0; sample < waveform.getNumSamples(); sample += ratio)
		{
			audioPoints.push_back(buffer[sample]);
		}

		g.setColour(juce::Colours::indigo);
		p.startNewSubPath(50, 200/2);

		for (int sample = 0; sample < audioPoints.size(); sample++)
		{
			auto point = juce::jmap(audioPoints[sample], -1.0f, 1.0f, 0.0f, 200.0f);
			p.lineTo(sample, point);
		}

		g.strokePath(p, juce::PathStrokeType(1));
		g.setColour(juce::Colours::white);
		g.setFont(15.0f);
		auto textBounds = getLocalBounds().reduced(10, 10);
		g.drawFittedText(fileName, textBounds, juce::Justification::topLeft, 1);

		//auto playHeadPosition = juce::jmap<int>(audioProcessorMine.getSampleCount(), 0,);
	}

	else
	{
		g.setColour(juce::Colours::white);
		g.setFont(40.0f);
		g.drawFittedText("Carga un audio", 200, 50, 200, 40, juce::Justification::centred, 1);
	}
}

void sampler_Waveform::resized()
{

}