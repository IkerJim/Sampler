/*
  ==============================================================================

    Synth.cpp
    Created: 8 Apr 2026 1:07:26pm
    Author:  ikerr

  ==============================================================================
*/

#include "Synth.h"

void Synth::noteOn(int midiChannel,
                   int midiNoteNumber,
                   float velocity)
{
    const juce::ScopedLock sl(lock);

    if (currentSound != nullptr && currentSound->appliesToNote(midiNoteNumber) && currentSound->appliesToChannel(midiChannel))
    {
        startVoice(findFreeVoice(currentSound.get(), midiChannel, midiNoteNumber, isNoteStealingEnabled()),
                                 currentSound.get(), midiChannel, midiNoteNumber, velocity);
    }
}

void Synth::changeSound(const juce::SynthesiserSound::Ptr& newSound)
{
    {
        const juce::ScopedLock sl(lock);
        currentSound = newSound;
    }

    sounds.add(newSound);
}