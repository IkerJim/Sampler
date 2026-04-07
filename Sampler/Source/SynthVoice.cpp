/*
  ==============================================================================

    SynthVoice.cpp
    Created: 7 Apr 2026 9:03:33am
    Author:  ikerr

  ==============================================================================
*/

#include "SynthVoice.h"

bool Voice::canPlaySound(juce::SynthesiserSound* sound)
{
    return dynamic_cast<Sound*>(sound) != nullptr;
}

void Voice::startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound* sound, int currentPitchWheelPosition)
{
    if (auto* s = dynamic_cast<Sound*>(sound))
    {
        pitchRatio = std::pow(2.0, (double)(midiNoteNumber - s->midiRootNote) / 12.0) * s->sourceSampleRate / getSampleRate();

        sourceSamplePosition = 0.0;
    }
}

void Voice::stopNote(float velocity, bool allowTailOff)
{
    clearCurrentNote();
}

void Voice::pitchWheelMoved(int newPitchWheelValue)
{

}

void Voice::controllerMoved(int controllerNumber, int newControllerValue)
{

}

void Voice::renderNextBlock(juce::AudioBuffer< float >& outputBuffer, int startSample, int numSamples)
{
    if (auto* playingSound = dynamic_cast<Sound*>(getCurrentlyPlayingSound().get()))
    {
        auto data = playingSound->data;

        const float* inL = data.getReadPointer(0);
        const float* inR = data.getNumChannels() > 1 ? data.getReadPointer(1) : inL;

        float* outL = outputBuffer.getWritePointer(0, startSample);
        float* outR = outputBuffer.getNumChannels() > 1 ? outputBuffer.getWritePointer(1, startSample) : nullptr;

        while (--numSamples >= 0)
        {
            int pos = (int)sourceSamplePosition;
            float frac = sourceSamplePosition - (float)pos;

            float l = inL[pos] + frac * (inL[pos + 1] - inL[pos]);
            float r = inR[pos] + frac * (inR[pos + 1] - inR[pos]);

            if (outR != nullptr)
            {
                *outL++ += l;
                *outR++ += r;
            }
            else
            {
                *outL++ += (l + r) * 0.5;
            }

            sourceSamplePosition += pitchRatio;
            if (sourceSamplePosition > playingSound->length)
            {
                stopNote(0.0f, true);
                break;
            }
        }
    }
}