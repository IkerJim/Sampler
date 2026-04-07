/*
  ==============================================================================

    SynthSound.cpp
    Created: 7 Apr 2026 9:03:11am
    Author:  ikerr

  ==============================================================================
*/

#include "SynthSound.h"

Sound::Sound(juce::AudioFormatReader& source, int midiNoteForNormalPitch, double maxLengthInSecs)
    : sourceSampleRate(source.sampleRate),
      midiRootNote(midiNoteForNormalPitch)
{
    if (sourceSampleRate > 0 && source.lengthInSamples > 0)
    {
        length = std::min((int) (maxLengthInSecs * sourceSampleRate),
                          (int) source.lengthInSamples);

        data.setSize(std::min((int)source.numChannels, 2), length);
        source.read(&data, 0, length, 0, true, true);
    }
}

bool Sound::appliesToNote(int midiNoteNumber)
{
    return true;
}

bool Sound::appliesToChannel(int midiChannel)
{
    return true;
}