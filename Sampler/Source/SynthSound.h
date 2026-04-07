/*
  ==============================================================================

    SynthSound.h
    Created: 7 Apr 2026 9:03:11am
    Author:  ikerr

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class Sound : public juce::SynthesiserSound
{
public:
    bool appliesToNote(int midiNoteNumber) override;
    bool appliesToChannel(int midiChannel) override;
};