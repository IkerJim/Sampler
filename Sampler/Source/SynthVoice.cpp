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
    if (auto* currentSound = dynamic_cast<Sound*>(sound))
    {
        pitchRatio = currentSound->sampleRate / getSampleRate() *
            std::pow(2.0f, static_cast<float>(midiNoteNumber - 60) / 12.0f);
        samplePosition = 0.0f;
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
        const float* leftInputChannel = playingSound->buffer.getReadPointer(0);
        const float* rightInputChannel = playingSound->buffer.getNumChannels() > 1 ?
            playingSound->buffer.getReadPointer(0) : leftInputChannel;

        float* leftOutputChannel = outputBuffer.getWritePointer(0, startSample);
        float* rightOutputChannel = outputBuffer.getNumChannels() > 1 ?
            outputBuffer.getWritePointer(1, startSample) : nullptr;

        while (--numSamples >= 0)
        {
            int position = static_cast<int>(samplePosition);
            float alpha = samplePosition - position;

            float l = (alpha - 1) * leftInputChannel[position] + alpha * leftInputChannel[position + 1];
            float r = (alpha - 1) * rightInputChannel[position] + alpha * rightInputChannel[position + 1];

            /*
            ADSR
            */

            if (rightOutputChannel != nullptr)
            {
                *leftOutputChannel++ += l;
                *rightOutputChannel++ += r;
            }
            else
            {
                *leftOutputChannel++ += (l + r) / 2;
            }

            samplePosition += pitchRatio;
            if (samplePosition > playingSound->length)
            {
                stopNote(0.0f, false);
                break;
            }
        }
    }
}
