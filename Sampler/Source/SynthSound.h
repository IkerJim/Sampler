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
    Sound(juce::AudioFormatReader& source, double maxDuration)
    {
        sampleRate = source.sampleRate;

        if (sampleRate > 0 && source.lengthInSamples > 0)
        {
            length = juce::jmin(static_cast<int>(source.lengthInSamples), 
                static_cast<int>(sampleRate * maxDuration));
            buffer.setSize(source.numChannels, length + 4);
            source.read(&buffer, 0, length + 4, 0, true, true);
        }
    }
    
    struct Parameters
    {
        float startPosition;
        float speedRatio;
    };

    void setParameters(const Parameters& newParameters)
    {
        parameters = newParameters;
    }

    bool appliesToNote(int midiNoteNumber) override;
    bool appliesToChannel(int midiChannel) override;

private:
    friend class Voice;

    juce::AudioBuffer<float> buffer;
    int length;
    double sampleRate;
    Parameters parameters;
};