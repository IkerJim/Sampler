/*
  ==============================================================================

    Synth.h
    Created: 8 Apr 2026 1:07:26pm
    Author:  ikerr

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class Synth : public juce::Synthesiser, 
              public juce::Thread
{
public:
    Synth();

    ~Synth();

    void noteOn(int midiChannel,
                int midiNoteNumber,
                float velocity) override;

    void changeSound(const juce::SynthesiserSound::Ptr& newSound);

    void run() override;

    void checkForBuffersToFree();

private:
    juce::SynthesiserSound::Ptr currentSound;
};