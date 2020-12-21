/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "knob.h"

//==============================================================================
/**
*/
class HarmonicExciterAudioProcessorEditor  : public juce::AudioProcessorEditor, private juce::Timer, private juce::Slider::Listener
{
public:
    HarmonicExciterAudioProcessorEditor (HarmonicExciterAudioProcessor&);
    ~HarmonicExciterAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    void timerCallback();
private:
    juce::Rectangle<int> background; //background Gui
    juce::String name; // Plugin Name 
    juce::Rectangle<int> gain_group;
    juce::String hp_label, group_label, in_label, ax_label, out_label, mix_label, hz_label, db_in, db_ax, db_out; //labels knobs
    juce::Slider hp_slider, in_slider, ax_slider, out_slider, mix_slider; // define sliders to knob transform
    Knob hp_knob, in_knob, ax_knob, out_knob, mix_knob; //define knobs
    juce::String hp_value, in_value, ax_value, out_value, mix_value; //define slider value 
    HarmonicExciterAudioProcessor& audioProcessor;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> sld_hp, sld_in, sld_ax, sld_out, sld_mix;
    void sliderValueChanged(juce::Slider* slider);
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (HarmonicExciterAudioProcessorEditor)
};
