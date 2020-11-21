/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once
#define _USE_MATH_DEFINES
#include <JuceHeader.h>
#include <cmath>

//==============================================================================
/**
*/
class HarmonicExciterAudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    HarmonicExciterAudioProcessor();
    ~HarmonicExciterAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    float hp, gain_in, gain_ax, gain_out, mix;
    juce::AudioProcessorValueTreeState::ParameterLayout createParameters();
    juce::AudioProcessorValueTreeState parameters;
private:
    float fs; //samplerfrequency
    float gain_in_linear, gain_ax_linear, gain_out_linear;
    juce::AudioBuffer<float> l_buffer, r_buffer;
    float in_l, in_ant_l, a_l, a_ant_l; //variables to high pass filter left
    float in_r, in_ant_r, a_r, a_ant_r; //variables to high pass filter right
    float distortion(float x);
    float highpass(float x, float x_ant,float&a, float a_ant, float frec_sampl, float fc);
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (HarmonicExciterAudioProcessor)
};
