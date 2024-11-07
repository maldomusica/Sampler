#include "Distortion.h"

Distortion::Distortion() {}
Distortion::~Distortion() {}

void Distortion::setGain (float inGain)
{
    distortionGain = inGain;
    //DBG("Dist updated" + juce::String(distortionGain));
}

void Distortion::setDistortion (int inDistortionIndex)
{
    distortionIndex = inDistortionIndex;
}

float Distortion::softClipping (float inSample)
{
    float gainSample = distortionGain * inSample;
    return (gainSample) - ( powf(gainSample, 3.0f) / 3.0f) ;
}

float Distortion::hardClipping (float inSample)
{
    float threshold = 0.55f;
    float gainSample = distortionGain * inSample;
    
    if (gainSample > threshold)
    {
        return threshold;
    }
    
    if (gainSample < -threshold)
    {
        return -threshold;
    }
    
    return gainSample;
}

float Distortion::bitCrusher(float inSample)
{
    int BitDepth = 4;
    int max = powf(2, BitDepth) - 1;
    float bitCrushSample = round((inSample + 1.0) * max) / max - 1.0;
    return bitCrushSample;
}


void Distortion::process (juce::AudioBuffer<float> &inBuffer)
{
    for (int channel = 0; channel < inBuffer.getNumChannels(); channel++)
    {
        for (int i = 0; i < inBuffer.getNumSamples();  i++)
        {
            auto sample = inBuffer.getSample (channel, i);
            float processedSample;
            
            switch (distortionIndex)
            {
                case 0:
                    processedSample = softClipping (sample);
                    break;
                
                case 1:
                    processedSample = hardClipping (sample);
                    break;
                    
                case 2:
                    processedSample = bitCrusher (sample);
                    break;
                    
                default:
                    processedSample = sample;
                    break;
            }
            
            inBuffer.setSample (channel, i, processedSample);
        }
    }
}
