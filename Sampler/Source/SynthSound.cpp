/*
  ==============================================================================

    SynthSound.cpp
    Created: 7 Apr 2026 9:03:11am
    Author:  ikerr

  ==============================================================================
*/

#include "SynthSound.h"

bool Sound::appliesToNote(int midiNoteNumber)
{
    return true;
}

bool Sound::appliesToChannel(int midiChannel)
{
    return true;
}