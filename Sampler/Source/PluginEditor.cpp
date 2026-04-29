/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SamplerAudioProcessorEditor::SamplerAudioProcessorEditor (SamplerAudioProcessor& p, juce::AudioProcessorValueTreeState& apvts)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    addAndMakeVisible(openButton);
    addAndMakeVisible(speedSlider);

    openButton.onClick = [this] {audioProcessor.openButtonClicked(); };

    speedSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    speedSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 100, 25);

    speedAttachment = std::make_unique<SliderAttachment>(apvts, "SPEED", speedSlider);

    setSize (400, 300);
}

SamplerAudioProcessorEditor::~SamplerAudioProcessorEditor()
{
}

//==============================================================================
void SamplerAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (juce::FontOptions (15.0f));
    g.drawFittedText ("Hello World!", getLocalBounds(), juce::Justification::centred, 1);
}

void SamplerAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    auto bounds = getLocalBounds().reduced(10);

    openButton.setBounds(bounds.removeFromTop(25).removeFromLeft(50));
    speedSlider.setBounds(bounds.withSizeKeepingCentre(85, 85));
}
