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
    addAndMakeVisible(positionSlider);
    addAndMakeVisible(speedSlider);

    openButton.onClick = [this] {audioProcessor.openButtonClicked(); };

    positionSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    positionSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 100, 25);

    speedSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    speedSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 100, 25);

    positionLabel.setText("Start Position", juce::dontSendNotification);
    positionLabel.setJustificationType(juce::Justification::centred);
    positionLabel.attachToComponent(&positionSlider, false);

    speedLabel.setText("Speed Ratio", juce::dontSendNotification);
    speedLabel.setJustificationType(juce::Justification::centred);
    speedLabel.attachToComponent(&speedSlider, false);

    positionAttachment = std::make_unique<SliderAttachment>(apvts, "POSITION", positionSlider);
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
}

void SamplerAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    auto bounds = getLocalBounds().reduced(10);

    juce::FlexBox fb;
    fb.flexWrap = juce::FlexBox::Wrap::wrap;
    fb.flexDirection = juce::FlexBox::Direction::row;
    fb.alignContent = juce::FlexBox::AlignContent::center;
    fb.justifyContent = juce::FlexBox::JustifyContent::spaceAround;
    fb.items.add(juce::FlexItem(positionSlider).withWidth(85).withHeight(85));
    fb.items.add(juce::FlexItem(speedSlider).withWidth(85).withHeight(85));
    fb.performLayout(bounds);

    openButton.setBounds(10, 10, 50, 25);
}
