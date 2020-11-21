/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include <cmath>

//==============================================================================
HarmonicExciterAudioProcessor::HarmonicExciterAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
    : AudioProcessor(BusesProperties()
#if ! JucePlugin_IsMidiEffect
#if ! JucePlugin_IsSynth
        .withInput("Input", juce::AudioChannelSet::stereo(), true)
#endif
        .withOutput("Output", juce::AudioChannelSet::stereo(), true)
#endif
    ), parameters(*this, nullptr, "SlidersValue", createParameters())
#endif
{
    parameters.state = juce::ValueTree("SlidersValue");
    l_buffer.clear();
    r_buffer.clear();
    hp = 5000; //high pass value
    gain_in = 0;
    gain_ax = 0;
    gain_out = 0;
    mix = 0;
    gain_in_linear = juce::Decibels::decibelsToGain(gain_in);
    gain_ax_linear = juce::Decibels::decibelsToGain(gain_ax);
    gain_out_linear = juce::Decibels::decibelsToGain(gain_out);
    in_l = 0;
    in_ant_l = 0;
    a_l = 0;
    a_ant_l = 0;
    in_r = 0;
    in_ant_r = 0;
    a_r = 0;
    a_ant_r = 0;
}
HarmonicExciterAudioProcessor::~HarmonicExciterAudioProcessor()
{
}

//==============================================================================
const juce::String HarmonicExciterAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool HarmonicExciterAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool HarmonicExciterAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool HarmonicExciterAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double HarmonicExciterAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int HarmonicExciterAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int HarmonicExciterAudioProcessor::getCurrentProgram()
{
    return 0;
}

void HarmonicExciterAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String HarmonicExciterAudioProcessor::getProgramName (int index)
{
    return {};
}

void HarmonicExciterAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void HarmonicExciterAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    fs = (float)sampleRate;
}

void HarmonicExciterAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool HarmonicExciterAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
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

void HarmonicExciterAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    gain_in_linear = juce::Decibels::decibelsToGain(gain_in);
    gain_ax_linear = juce::Decibels::decibelsToGain(gain_ax);
    gain_out_linear = juce::Decibels::decibelsToGain(gain_out);
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    buffer.applyGain(gain_in_linear);
    for (int i = 0; i < buffer.getNumSamples(); i++) {
        for (int channel = 0; channel < totalNumInputChannels; ++channel) {
            auto* channelData = buffer.getWritePointer(channel);
            if (channel == 0) {
                //in_l = distortion(channelData[i]);
                in_l = channelData[i];
                //float out_hp = highpass(in_l, in_ant_l, a_l, a_ant_l, fs, hp);
                /*float outdist = (out_hp + abs(out_hp)) / 2; //half wave rectification
                outdist = outdist * gain_ax_linear;*/
                float outdist = (in_l+ abs(in_l)) / 2;
                float out_hp = highpass(outdist, in_ant_l, a_l, a_ant_l, fs, hp);
                out_hp = out_hp * gain_ax_linear;
                //float out = channelData[i] * (1 - mix) + outdist * mix;
                float out = channelData[i] * (1 - mix) + out_hp * mix;
                channelData[i] = out * gain_out_linear;
                //in_ant_l = in_l;
                in_ant_l = outdist;
                a_ant_l = a_l;
            }
            if (channel == 1) {
                //in_r = distortion(channelData[i]);
                in_r = channelData[i];
                //float out_hp = highpass(in_r, in_ant_r, a_r, a_ant_r, fs, hp);
                //float outdist = (out_hp + abs(out_hp)) / 2; //half wave rectification
                //outdist = outdist * gain_ax_linear;
                //float out = channelData[i] * (1 - mix) + outdist * mix;
                float outdist = (in_r + abs(in_r)) / 2;
                float out_hp = highpass(outdist, in_ant_r, a_r, a_ant_r, fs, hp);
                out_hp = out_hp * gain_ax_linear;
                float out = channelData[i] * (1 - mix) + out_hp * mix;
                channelData[i] = out * gain_out_linear;
                //in_ant_r = in_r;
                in_ant_r = outdist;
                a_ant_r = a_r;
            }
            
        }
    }
}


float HarmonicExciterAudioProcessor::distortion(float x)
{
    float th = 1 / 3;
    float y = 0;

    if (abs(x) < th) {
        y = 2 * x;
    }// line zone
    if (abs(x) >= th) {
        //quadratic zone
        if (x > 0) {
            y = (3 - pow(2 - x * 3, 2)) / 3;
        }
        if (x < 0) {
            y = -(3 - pow(2 - abs(x) * 3, 2)) / 3;
        }
    }
    if (abs(x) > 2 * th) {
        //contant zone
        if (x > 0) {
            y = 1;
        }
        if (x < 0) {
            y = -1;
        }
    }
    return y;
    
}
float HarmonicExciterAudioProcessor::highpass(float x, float x_ant,float& a, float a_ant, float frec_sampl, float fc)
{
    float c = (tanf(M_PI * fc / frec_sampl) - 1) / (tan(M_PI * fc / frec_sampl) + 1);
    
    float y;
    a = c * x + x_ant - c * a_ant;
    y = 0.5f * (x - a);
    return y;

}

//==============================================================================
bool HarmonicExciterAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* HarmonicExciterAudioProcessor::createEditor()
{
    return new HarmonicExciterAudioProcessorEditor (*this);
}

//==============================================================================
void HarmonicExciterAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    std::unique_ptr<juce::XmlElement> xml(parameters.copyState().createXml());
    copyXmlToBinary(*xml, destData);
}

void HarmonicExciterAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> params(getXmlFromBinary(data, sizeInBytes));

    if (params != nullptr)
    {
        if (params->hasTagName(parameters.copyState().getType())) {
            parameters.state = juce::ValueTree::fromXml(*params);
        }
    }
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new HarmonicExciterAudioProcessor();
}
juce::AudioProcessorValueTreeState::ParameterLayout
HarmonicExciterAudioProcessor::createParameters()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> param;
    param.push_back(std::make_unique<juce::AudioParameterFloat>("hp", "Hp", 5000.0f, 10000.0f, 5000.0f));
    param.push_back(std::make_unique<juce::AudioParameterFloat>("in", "In", 0.0f, 10.0f, 0.0f));
    param.push_back(std::make_unique<juce::AudioParameterFloat>("ax", "Ax", 0.0f, 10.0f, 0.0f));
    param.push_back(std::make_unique<juce::AudioParameterFloat>("out", "Out", 0.0f, 10.0f, 0.0f));
    param.push_back(std::make_unique<juce::AudioParameterFloat>("mix", "Mix", 0.0f, 1.0f, 0.0f));
    return{ param.begin(),param.end() };
}