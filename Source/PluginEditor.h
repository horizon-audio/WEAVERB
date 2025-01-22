/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

class KnobLookAndFeel : public juce::LookAndFeel_V4
{
public:

    void drawRotarySlider(juce::Graphics& g,
        int x, int y, int width, int height,
        float sliderPosProportional,
        float rotaryStartAngle,
        float rotaryEndAngle,
        juce::Slider& slider);
};


class WEAVERBAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    WEAVERBAudioProcessorEditor (WEAVERBAudioProcessor&);
    ~WEAVERBAudioProcessorEditor() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:

    juce::Image background;

    juce::Slider voidKnob;
    juce::Slider fadeKnob;
    juce::Slider spreadKnob;
    juce::Slider mixKnob;

    KnobLookAndFeel knobLNF;
   
    WEAVERBAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WEAVERBAudioProcessorEditor)
};
