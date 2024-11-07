#include "LFO.h"
#include "DryWet.h"

LFO::LFO() {}
LFO::~LFO() {}

void LFO::setFrequencyValue (float frecValue)
{
    frequency = frecValue;
}

void LFO::prepare (double theSampleRate)
{
    sampleRate = static_cast<float>(theSampleRate); //casteo la variable: paso de un tipo de variable a otra

    for (int i = 0; i < 2; i++) //inicializo mis variables time y deltaTime
    {
        time[i] = 0.0f;
        deltaTime[i] = 1.0f / sampleRate; //saco el valor del Periodo
    }
}

void LFO::process(juce::AudioBuffer<float>& buffer)
{
    for (int channel = 0; channel < buffer.getNumChannels(); channel++)
    {
        for (int i = 0; i < buffer.getNumSamples(); i++)
        {
            float x = buffer.getSample(channel, i);

            float lfoValue{ 0.0f };

            switch (lfoType)
            {
            case Type::Sine:
                lfoValue = sineWave(channel);
                break;

            case Type::Square:
                lfoValue = squareWave(channel);
                break;

            case Type::Sawtooth:
                lfoValue = sawtoothWave(channel);
                break;

            case Type::Triangle:
                lfoValue = triangleWave(channel);
                break;

            default:
                break;
            }

            float y = x * lfoValue;

            buffer.setSample(channel, i, y);

            time[channel] += deltaTime[channel];

            //Reset time variable
            if (time[channel] >= 1.0f)
                time[channel] = 0.0f;
        }
    }
}

void LFO::setLfoType(Type newLfoType)
{
    lfoType = newLfoType;
}


float LFO::sineWave(int channel)
{
    return 0.5f + (0.5f * sinf(juce::MathConstants<float>::twoPi * frequency * time[channel]));
}

float LFO::squareWave(int channel)
{
    float fullPeriodTime = 1.0f / frequency;
    float halfPeriodTime = fullPeriodTime / 2.0f;
    float localTime = fmod(time[channel], fullPeriodTime);

    if (localTime < halfPeriodTime)
        return 1.0f;
    else
        return 0.0f;
}

float LFO::triangleWave(int channel)
{
    float res = 0.0f;
    float fullPeriodTime = 1.0f / frequency;
    float localTime = fmod(time[channel], fullPeriodTime);

    float value = localTime / fullPeriodTime;

    if (value < 0.25f)
    {
        res = value * 4.0f;
    }
    else if (value < 0.75f)
    {
        res = 2.0f - (value * 4.0f);
    }
    else
    {
        res = value * 4.0f - 4.0f;
    }

    return res;
}

float LFO::sawtoothWave(int channel)
{
    float fullPeriodTime = 1.0f / frequency;
    float localTime = fmod(time[channel], fullPeriodTime);

    return (localTime / fullPeriodTime) * 2.0f - 1.0f;
}