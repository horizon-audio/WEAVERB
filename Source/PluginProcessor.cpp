/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

static juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;


    layout.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{ ParamIDs::size, 1 },
        ParamIDs::size,
        juce::NormalisableRange{ 0.0f, 100.0f, 0.01f, 1.0f },
        50.0f));

    layout.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{ ParamIDs::damp, 1 },
        ParamIDs::damp,
        juce::NormalisableRange{ 0.0f, 100.0f, 0.01f, 1.0f },
        50.0f));

    layout.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{ ParamIDs::width, 1 },
        ParamIDs::width,
        juce::NormalisableRange{ 0.0f, 100.0f, 0.01f, 1.0f },
        50.0f));

    layout.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{ ParamIDs::mix, 1 },
        ParamIDs::mix,
        juce::NormalisableRange{ 0.0f, 100.0f, 0.01f, 1.0f },
        50.0f));

    return layout;
}

//==============================================================================
WEAVERBAudioProcessor::WEAVERBAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ),
    apvts(*this, nullptr, "Parameters", createParameterLayout())
#endif
{
    size = dynamic_cast<juce::AudioParameterFloat*>(apvts.getParameter(ParamIDs::size));
    damp = dynamic_cast<juce::AudioParameterFloat*>(apvts.getParameter(ParamIDs::damp));
    width = dynamic_cast<juce::AudioParameterFloat*>(apvts.getParameter(ParamIDs::width));
    mix = dynamic_cast<juce::AudioParameterFloat*>(apvts.getParameter(ParamIDs::mix));
}

WEAVERBAudioProcessor::~WEAVERBAudioProcessor()
{

}

//==============================================================================
const juce::String WEAVERBAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool WEAVERBAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool WEAVERBAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool WEAVERBAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double WEAVERBAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int WEAVERBAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int WEAVERBAudioProcessor::getCurrentProgram()
{
    return 0;
}

void WEAVERBAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String WEAVERBAudioProcessor::getProgramName (int index)
{
    return {};
}

void WEAVERBAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void WEAVERBAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    juce::dsp::ProcessSpec spec{};

    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = static_cast<juce::uint32> (samplesPerBlock);
    spec.numChannels = static_cast<juce::uint32> (getTotalNumOutputChannels());

    reverb.prepare(spec);
}

void WEAVERBAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool WEAVERBAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void WEAVERBAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ignoreUnused(midiMessages);
    juce::ScopedNoDenormals noDenormals;

    updateReverbParams();

    juce::dsp::AudioBlock<float> block(buffer);
    juce::dsp::ProcessContextReplacing ctx(block);
    reverb.process(ctx);
}

//==============================================================================
bool WEAVERBAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* WEAVERBAudioProcessor::createEditor()
{
    return new WEAVERBAudioProcessorEditor (*this);
}

//==============================================================================
void WEAVERBAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void WEAVERBAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

void WEAVERBAudioProcessor::updateReverbParams()
{
    params.roomSize = size->get() * 0.01f;
    params.damping = damp->get() * 0.01f;
    params.width = width->get() * 0.01f;
    params.wetLevel = mix->get() * 0.01f;
    params.dryLevel = 1.0f - mix->get() * 0.01f;

    reverb.setParameters(params);
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new WEAVERBAudioProcessor();
}
