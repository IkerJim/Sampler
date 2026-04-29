/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SamplerAudioProcessor::SamplerAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ),
    apvts(*this, nullptr, "PARAMETERS", setLayout())
#endif
{
    formatManager.registerBasicFormats();
}

SamplerAudioProcessor::~SamplerAudioProcessor()
{
}

//==============================================================================
const juce::String SamplerAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool SamplerAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool SamplerAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool SamplerAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double SamplerAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int SamplerAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int SamplerAudioProcessor::getCurrentProgram()
{
    return 0;
}

void SamplerAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String SamplerAudioProcessor::getProgramName (int index)
{
    return {};
}

void SamplerAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void SamplerAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    synth.setCurrentPlaybackSampleRate(sampleRate);

    synth.addVoice(new Voice());
}

void SamplerAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool SamplerAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void SamplerAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.
    
    getParameters();
    
    synth.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());
    
}

//==============================================================================
bool SamplerAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* SamplerAudioProcessor::createEditor()
{
    return new SamplerAudioProcessorEditor (*this, apvts);
}

//==============================================================================
void SamplerAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void SamplerAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SamplerAudioProcessor();
}

void SamplerAudioProcessor::openButtonClicked()
{
    chooser = std::make_unique<juce::FileChooser>("Select a file", 
                                                  juce::File::getSpecialLocation(juce::File::userHomeDirectory), 
                                                  "*.wav");

    auto chooserFlags = juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectFiles;

    chooser->launchAsync(chooserFlags, [this](const juce::FileChooser& fc)
        {
            auto file = fc.getResult();

            if (file == juce::File())
                return;

            std::unique_ptr<juce::AudioFormatReader> reader(formatManager.createReaderFor(file));

            if (reader.get() != nullptr)
            {
                synth.changeSound(new Sound(*reader, 10.0f));
            }
        });
}

juce::AudioProcessorValueTreeState::ParameterLayout SamplerAudioProcessor::setLayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;

    auto position = std::make_unique<juce::AudioParameterFloat>(
        "POSITION",
        "Position",
        juce::NormalisableRange<float>(0.0f, 100.0f),
        0.0f,
        juce::AudioParameterFloatAttributes().withStringFromValueFunction([](float value, int) { return juce::String(int(value)) + " %"; })
    );
    auto speed = std::make_unique<juce::AudioParameterFloat>(
        "SPEED",
        "Speed",
        juce::NormalisableRange<float>(-1.0f, 1.0f),
        1.0f
    );

    layout.add(std::move(position));
    layout.add(std::move(speed));

    return layout;
}

void SamplerAudioProcessor::setParameters()
{
    positionParameter = dynamic_cast<juce::AudioParameterFloat*>(apvts.getParameter("POSITION"));
    speedParameter = dynamic_cast<juce::AudioParameterFloat*>(apvts.getParameter("SPEED"));
}

void SamplerAudioProcessor::getParameters()
{
    DBG(positionParameter->get());
    DBG(speedParameter->get());
}
