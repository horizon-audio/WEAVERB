/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

using namespace juce;

void KnobLookAndFeel::drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos, float rotaryStartAngle, float rotaryEndAngle, juce::Slider& slider)
{
    auto outline = slider.findColour(Slider::rotarySliderOutlineColourId);
    auto fill = slider.findColour(Slider::rotarySliderFillColourId);

    auto bounds = Rectangle<int>(x, y, width, height).toFloat().reduced(10);

    auto radius = jmin(bounds.getWidth(), bounds.getHeight()) / 2.0f;
    auto toAngle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);
    auto lineW = jmin(8.0f, radius * 0.5f);
    auto arcRadius = radius - lineW * 0.5f;

    g.setColour(juce::Colours::white.darker(0.1f));
    g.fillEllipse(x, y, width, height);

    g.setColour(juce::Colours::white.darker(0.2f));
    g.drawEllipse(x, y, width, height, 3.0f);

    auto thumbWidth = lineW * 2.0f;
    Point<float> thumbPoint(bounds.getCentreX() + arcRadius * std::cos(toAngle - MathConstants<float>::halfPi),
        bounds.getCentreY() + arcRadius * std::sin(toAngle - MathConstants<float>::halfPi));

    g.setColour(juce::Colours::white);
    g.fillEllipse(Rectangle<float>(thumbWidth, thumbWidth).withCentre(thumbPoint));

    g.setColour(juce::Colours::white.darker(0.2f));
    g.drawEllipse(Rectangle<float>(thumbWidth, thumbWidth).withCentre(thumbPoint), 1.5f);
}

WEAVERBAudioProcessorEditor::WEAVERBAudioProcessorEditor (WEAVERBAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p),
    voidKnob(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag, juce::Slider::TextEntryBoxPosition::NoTextBox),
    fadeKnob(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag, juce::Slider::TextEntryBoxPosition::NoTextBox),
    spreadKnob(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag, juce::Slider::TextEntryBoxPosition::NoTextBox),
    mixKnob(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag, juce::Slider::TextEntryBoxPosition::NoTextBox)
{

    setSize(984, 335);

    voidKnob.setLookAndFeel(&knobLNF);
    fadeKnob.setLookAndFeel(&knobLNF);
    spreadKnob.setLookAndFeel(&knobLNF);
    mixKnob.setLookAndFeel(&knobLNF);

    background = juce::ImageCache::getFromMemory(BinaryData::background_png, BinaryData::background_pngSize);

    addAndMakeVisible(voidKnob);
    addAndMakeVisible(fadeKnob);
    addAndMakeVisible(spreadKnob);
    addAndMakeVisible(mixKnob);
}

WEAVERBAudioProcessorEditor::~WEAVERBAudioProcessorEditor()
{
    voidKnob.setLookAndFeel(nullptr);
    fadeKnob.setLookAndFeel(nullptr);
    spreadKnob.setLookAndFeel(nullptr);
    mixKnob.setLookAndFeel(nullptr);
}

void WEAVERBAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.drawImageWithin(background, 0, 0, getWidth(), getHeight(), juce::RectanglePlacement::fillDestination);
}

void WEAVERBAudioProcessorEditor::resized()
{
    voidKnob.setBounds(470, 195, 80, 80);
    fadeKnob.setBounds(560, 195, 80, 80);
    spreadKnob.setBounds(655, 195, 80, 80);
    mixKnob.setBounds(745, 195, 80, 80);
}
