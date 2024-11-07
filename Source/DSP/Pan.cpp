#include "Pan.h"

Pan::Pan() {}

Pan::~Pan() {}

void Pan::setPanningValue(float panValue)
{
    panningValue = panValue;
}

void Pan::process(juce::AudioBuffer<float>& buffer)
{
    for (int channel = 0; channel < buffer.getNumChannels(); channel++)
    {
        for (int i = 0; i < buffer.getNumSamples(); i++)
        {
            //get Sample
            float sample = buffer.getSample(channel, i);

            // L Channel
            if (channel == 0)
            {
                sample *= sqrtf(((PI/2.0f) - panningValue) * (2.0f/PI) * cosf(panningValue));
            }

            //R Channel
            else
            {
                sample *= sqrtf(panningValue * (2.0f/PI) * sinf(panningValue));
            }

            buffer.setSample(channel, i,  sample);
        }
    }
}