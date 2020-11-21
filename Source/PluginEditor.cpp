/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"


//==============================================================================
HarmonicExciterAudioProcessorEditor::HarmonicExciterAudioProcessorEditor (HarmonicExciterAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (1000, 430);
    background.setSize(1000, 430);
    background.setX(0);
    background.setY(0);
    name = (TRANS("HARMONIC EXCITER"));
    gain_group.setSize(500, 218);
    gain_group.setX(250);
    gain_group.setY(130);
    hp_label = (TRANS("HIGH PASS"));
    group_label = (TRANS("GAIN"));
    in_label = (TRANS("IN"));
    ax_label = (TRANS("AX"));
    out_label = (TRANS("OUTPUT"));
    mix_label = (TRANS("MIX"));
    hz_label = (TRANS("Hz"));
    db_in = (TRANS("dB"));
    db_ax = (TRANS("dB"));
    db_out = (TRANS("dB"));
    addAndMakeVisible(hp_slider);
    addAndMakeVisible(in_slider);
    addAndMakeVisible(ax_slider);
    addAndMakeVisible(out_slider);
    addAndMakeVisible(mix_slider);
    hp_slider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    hp_slider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
    juce::Colour blck = juce::Colours::black;
    juce::Colour wht = juce::Colours::white;
    hp_knob.Knobcolor(blck, wht);
    hp_slider.setLookAndFeel(&hp_knob);
    hp_slider.onValueChange = [this] {hp_slider.getValue();};
    audioProcessor.hp = (float)hp_slider.getValue();
    hp_slider.addListener(this);
    sld_hp = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.parameters, "hp", hp_slider);
    hp_slider.setRange(5000, 10000, 1);
    in_slider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    in_slider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
    in_knob.Knobcolor(wht, blck);
    in_slider.setLookAndFeel(&in_knob);
    in_slider.onValueChange = [this] {in_slider.getValue(); };
    audioProcessor.gain_in = (float)in_slider.getValue();
    in_slider.addListener(this);
    sld_in = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.parameters, "in", in_slider);
    in_slider.setRange(0, 10, 1);
    ax_slider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    ax_slider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
    ax_knob.Knobcolor(wht, blck);
    ax_slider.setLookAndFeel(&ax_knob);
    ax_slider.onValueChange = [this] {ax_slider.getValue(); };
    audioProcessor.gain_ax = (float)ax_slider.getValue();
    ax_slider.addListener(this);
    sld_ax = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.parameters, "ax", ax_slider);
    ax_slider.setRange(0, 10, 1);
    out_slider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    out_slider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
    out_knob.Knobcolor(wht, blck);
    out_slider.setLookAndFeel(&out_knob);
    out_slider.onValueChange = [this] {out_slider.getValue(); };
    audioProcessor.gain_out = (float)out_slider.getValue();
    out_slider.addListener(this);
    sld_out = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.parameters, "out", out_slider);
    out_slider.setRange(0, 10, 1);
    mix_slider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    mix_slider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
    mix_knob.Knobcolor(blck, wht);
    mix_slider.setLookAndFeel(&mix_knob);
    mix_slider.onValueChange = [this] {mix_slider.getValue(); };
    audioProcessor.mix = (float)mix_slider.getValue();
    mix_slider.addListener(this);
    sld_mix = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.parameters, "mix", mix_slider);
    mix_slider.setRange(0, 1, 0.1f);

    hp_value = juce::String(hp_slider.getValue());
    in_value = juce::String(in_slider.getValue());
    ax_value = juce::String(ax_slider.getValue());
    out_value = juce::String(out_slider.getValue());
    mix_value = juce::String(mix_slider.getValue());
   
    startTimer(50);

}

HarmonicExciterAudioProcessorEditor::~HarmonicExciterAudioProcessorEditor()
{
}

//==============================================================================
void HarmonicExciterAudioProcessorEditor::paint (juce::Graphics& g)
{
    //paint background
    {
        juce::Colour fillColour = juce::Colours::black;
        juce::Colour strokeColour = juce::Colours::white;

        g.setColour(fillColour);
        g.fillRect(background);
        g.setColour(strokeColour);
        g.drawRect(background, 5);

    }
    //Plugin Name Label
    {
        int x = -6, y = 14, width = 996, height = 30;
        juce::Colour fillColour = juce::Colours::white;
        
        g.setColour(fillColour);
        g.setFont(juce::Font("Bauhaus 93", 50.00f, juce::Font::plain).withTypefaceStyle("Regular"));
        g.drawText(name, x, y, width, height,
            juce::Justification::centred, true);
    }
    //Plugin Group Gain Background
    {
        juce::Colour fillColour = juce::Colours::white;

        g.setColour(fillColour);
        g.fillRoundedRectangle(gain_group.getX(),gain_group.getY(),gain_group.getWidth(),gain_group.getHeight(), 10.000f);
    }
    //HP Label
    {
        int x = 52, y = 176, width = 120, height = 30;
        juce::Colour fillColour = juce::Colours::white;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour(fillColour);
        g.setFont(juce::Font("Bauhaus 93", 25.00f, juce::Font::plain).withTypefaceStyle("Regular"));
        g.drawText(hp_label, x, y, width, height,
            juce::Justification::centred, true);
    }
    //Gain label
    {
        int x = 269, y = 100, width = 467, height = 30;
        juce::Colour fillColour = juce::Colours::white;

        g.setColour(fillColour);
        g.setFont(juce::Font("Bauhaus 93", 39.60f, juce::Font::italic));
        g.drawText(group_label, x, y, width, height,
            juce::Justification::centred, true);
    }
    //In label
    {
        int x = 257, y = 176, width = 200, height = 30;
        juce::Colour fillColour = juce::Colours::black;

        g.setColour(fillColour);
        g.setFont(juce::Font("Bauhaus 93", 30.00f, juce::Font::plain).withTypefaceStyle("Regular"));
        g.drawText(in_label, x, y, width, height,
            juce::Justification::centred, true);
    }
    // Ax label
    {
        int x = 403, y = 176, width = 200, height = 30;
        juce::Colour fillColour = juce::Colours::black;
 
        g.setColour(fillColour);
        g.setFont(juce::Font("Bauhaus 93", 30.00f, juce::Font::plain).withTypefaceStyle("Regular"));
        g.drawText(ax_label, x, y, width, height,
            juce::Justification::centred, true);
    }
    // Out label
    {
        int x = 547, y = 176, width = 200, height = 30;
        juce::Colour fillColour = juce::Colours::black;

        g.setColour(fillColour);
        g.setFont(juce::Font("Bauhaus 93", 30.00f, juce::Font::plain).withTypefaceStyle("Regular"));
        g.drawText(out_label, x, y, width, height,
            juce::Justification::centred, true);
    }
    //Mix label
    {
        int x = 848, y = 176, width = 62, height = 30;
        juce::Colour fillColour = juce::Colours::white;

        g.setColour(fillColour);
        g.setFont(juce::Font("Bauhaus 93", 25.00f, juce::Font::plain).withTypefaceStyle("Regular"));
        g.drawText(mix_label, x, y, width, height,
            juce::Justification::centred, true);
    }
    //Hz label
    {
        int x = 135, y = 284, width = 20, height = 15;
        juce::Colour fillColour = juce::Colours::white;
        g.setColour(fillColour);
        g.setFont(juce::Font("Bauhaus 93", 17.00f, juce::Font::plain).withTypefaceStyle("Regular"));
        g.drawText(hz_label, x, y, width, height,
            juce::Justification::centred, true);
    }
    //dB in
    {
        int x = 370, y = 286, width = 20, height = 10;
        juce::Colour fillColour = juce::Colours::black;
        g.setColour(fillColour);
        g.setFont(juce::Font("Bauhaus 93", 17.00f, juce::Font::plain).withTypefaceStyle("Regular"));
        g.drawText(db_in, x, y, width, height,
            juce::Justification::centred, true);
    }
    //db ax
    {
        int x = 515, y = 286, width = 20, height = 10;
        juce::Colour fillColour = juce::Colours::black;
        g.setColour(fillColour);
        g.setFont(juce::Font("Bauhaus 93", 17.00f, juce::Font::plain).withTypefaceStyle("Regular"));
        g.drawText(db_ax, x, y, width, height,
            juce::Justification::centred, true);
    }
    //db out
    {
        int x = 658, y = 286, width = 20, height = 10;
        juce::Colour fillColour = juce::Colours::black;
        g.setColour(fillColour);
        g.setFont(juce::Font("Bauhaus 93", 17.00f, juce::Font::plain).withTypefaceStyle("Regular"));
        g.drawText(db_out, x, y, width, height,
            juce::Justification::centred, true);
    }
    //hp value
    {
        int x = 75, y = 284, width = 68, height = 15;
        juce::Colour fillColour = juce::Colours::white;

        g.setColour(fillColour);
        g.setFont(juce::Font("Bauhaus 93", 15.00f, juce::Font::plain).withTypefaceStyle("Regular"));
        g.drawText(hp_value, x, y, width, height,
            juce::Justification::centred, true);
    }
    //in value
    {
        int x = 335, y = 284, width = 45, height = 15;
        juce::Colour fillColour = juce::Colours::black;
        g.setColour(fillColour);
        g.setFont(juce::Font("Bauhaus 93", 15.00f, juce::Font::plain).withTypefaceStyle("Regular"));
        g.drawText(in_value, x, y, width, height,
            juce::Justification::centred, true);
    }
    //ax value 
    {
        int x = 490, y = 284, width = 30, height = 15;
        juce::Colour fillColour = juce::Colours::black;

        g.setColour(fillColour);
        g.setFont(juce::Font("Bauhaus 93", 15.00f, juce::Font::plain).withTypefaceStyle("Regular"));
        g.drawText(ax_value, x, y, width, height,
            juce::Justification::centred, true);
    }
    //out value 
    {
        int x = 625, y = 284, width = 45, height = 15;
        juce::Colour fillColour = juce::Colours::black;
        g.setColour(fillColour);
        g.setFont(juce::Font("Bauhaus 93", 15.00f, juce::Font::plain).withTypefaceStyle("Regular"));
        g.drawText(out_value, x, y, width, height,
            juce::Justification::centred, true);
    }
    //mix value
    {
        int x = 855, y = 284, width = 55, height = 15;
        juce::Colour fillColour = juce::Colours::white;
        g.setColour(fillColour);
        g.setFont(juce::Font("Bauhaus 93", 15.00f, juce::Font::plain).withTypefaceStyle("Regular"));
        g.drawText(mix_value, x, y, width, height,
            juce::Justification::centred, true);
    }
}

void HarmonicExciterAudioProcessorEditor::resized()
{
    hp_slider.setBounds(79, 214, 66, 66);
    in_slider.setBounds(326, 214, 66, 66);
    ax_slider.setBounds(472, 214, 66, 66);
    out_slider.setBounds(616, 214, 66, 66);
    mix_slider.setBounds(849, 214, 66, 66);
}
void HarmonicExciterAudioProcessorEditor::timerCallback()
{
    hp_slider.setValue(audioProcessor.hp, juce::dontSendNotification);
    in_slider.setValue(audioProcessor.gain_in, juce::dontSendNotification);
    ax_slider.setValue(audioProcessor.gain_ax, juce::dontSendNotification);
    out_slider.setValue(audioProcessor.gain_out, juce::dontSendNotification);
    mix_slider.setValue(audioProcessor.mix, juce::dontSendNotification);
    hp_value = juce::String(hp_slider.getValue());
    in_value = juce::String(in_slider.getValue());
    ax_value = juce::String(ax_slider.getValue());
    out_value = juce::String(out_slider.getValue());
    mix_value = juce::String(mix_slider.getValue());
    repaint();
}
void HarmonicExciterAudioProcessorEditor::sliderValueChanged(juce::Slider* slider)
{
    if (slider == &hp_slider) {
        hp_slider.setValue(hp_slider.getValue(), juce::dontSendNotification);
        audioProcessor.hp = (float)hp_slider.getValue();
    }
    if (slider == &in_slider) {
        in_slider.setValue(in_slider.getValue(), juce::dontSendNotification);
        audioProcessor.gain_in = (float)in_slider.getValue();
    }
    if (slider == &ax_slider) {
        ax_slider.setValue(ax_slider.getValue(), juce::dontSendNotification);
        audioProcessor.gain_ax = (float)ax_slider.getValue();
    }
    if (slider == &out_slider) {
        out_slider.setValue(out_slider.getValue(), juce::dontSendNotification);
        audioProcessor.gain_out = (float)out_slider.getValue();
    }
    if (slider == &mix_slider) {
        mix_slider.setValue(mix_slider.getValue(), juce::dontSendNotification);
        audioProcessor.mix = (float)mix_slider.getValue();
    }
}