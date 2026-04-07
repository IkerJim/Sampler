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

}

void Voice::stopNote(float velocity, bool allowTailOff)
{

}

void Voice::pitchWheelMoved(int newPitchWheelValue)
{

}

void Voice::controllerMoved(int controllerNumber, int newControllerValue)
{

}

void Voice::renderNextBlock(juce::AudioBuffer< float >& outputBuffer, int startSample, int numSamples)
{

}