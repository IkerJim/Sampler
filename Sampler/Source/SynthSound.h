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
    Sound(juce::AudioFormatReader& source, int midiNoteForNormalPitch, double maxLengthInSecs);

    bool appliesToNote(int midiNoteNumber) override;
    bool appliesToChannel(int midiChannel) override;

private:
    friend class Voice;

    juce::AudioBuffer<float> data;
    double sourceSampleRate;
    int length;
    int midiRootNote;
};