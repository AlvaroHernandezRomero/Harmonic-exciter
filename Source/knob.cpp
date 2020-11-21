/*
  ==============================================================================

    Knob.cpp
    Created: 12 Sep 2020 5:10:18pm
    Author:  Alvaro

  ==============================================================================
*/

#include <JuceHeader.h>
#include "Knob.h"

//==============================================================================
Knob::Knob()
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

}

Knob::~Knob()
{
}
void Knob::Knobcolor(juce::Colour bck, juce::Colour ring)
{
    background = bck;
    edge = ring;

}
void Knob::drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPosProportional, float rotaryStartAngle, float rotaryEndAngle, juce::Slider&sl)
{
    auto radius = (float)juce::jmin(width / 2, height / 2) - 4.0f;
    auto centreX = (float)x + (float)width * 0.5f;
    auto centreY = (float)y + (float)height * 0.5f;
    auto rx = centreX - radius;
    auto ry = centreY - radius;
    auto rw = radius * 2.0f;
    auto angle = rotaryStartAngle + sliderPosProportional * (rotaryEndAngle - rotaryStartAngle);
    juce::Colour fillColour = background;
    juce::Colour strokeColour = edge;
    g.setColour(fillColour);
    g.fillEllipse(rx, ry, rw, rw);
    // outline
    g.setColour(strokeColour);
    g.drawEllipse(rx, ry, rw, rw, 5.000f);
    juce::Path p;
    auto pointerLength = radius*0.75f;
    auto pointerThickness = 4.0f;
    p.addRectangle(-pointerThickness * 0.5f, -radius, pointerThickness, pointerLength);
    p.applyTransform(juce::AffineTransform::rotation(angle).translated(centreX, centreY));
    g.setColour(strokeColour);
    g.fillPath(p);
    
}