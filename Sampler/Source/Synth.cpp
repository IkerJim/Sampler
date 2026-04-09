/*
  ==============================================================================

    Synth.cpp
    Created: 8 Apr 2026 1:07:26pm
    Author:  ikerr

  ==============================================================================
*/

#include "Synth.h"

Synth::Synth():
    Thread("")
{
    startThread();
}

Synth::~Synth()
{
    stopThread(1000);
}

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

void Synth::run()
{
    while (!threadShouldExit())
    {
        checkForBuffersToFree();
        wait(500);
    }
}

void Synth::checkForBuffersToFree()
{
    for (int i = sounds.size(); --i >= 0;)
    {
        juce::SynthesiserSound::Ptr sound(sounds.getUnchecked(i));
        if (sound->getReferenceCount() <= 2)
            sounds.remove(i);
    }
}