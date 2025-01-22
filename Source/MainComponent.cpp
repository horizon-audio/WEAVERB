#include "MainComponent.h"

void KnobLookAndFeel::drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos, float rotaryStartAngle, float rotaryEndAngle, juce::Slider& slider)
{
    auto knobImage = juce::ImageCache::getFromMemory(BinaryData::knob_png, BinaryData::knob_pngSize);

    g.drawImageWithin(knobImage, x, y, width, height, juce::RectanglePlacement::fillDestination);

    auto outline = slider.findColour(juce::Slider::rotarySliderOutlineColourId);
    auto fill = slider.findColour(juce::Slider::rotarySliderFillColourId);

    auto bounds = juce::Rectangle<int>(x, y, width, height).toFloat().reduced(10);

    auto radius = juce::jmin(bounds.getWidth(), bounds.getHeight()) / 2.0f;
    auto toAngle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);
    auto lineW = bounds.getWidth() / 22.0f;

    const float centreX = x + width * 0.5f;
    const float centreY = y + height * 0.5f;
    const float rx = centreX - radius;
    const float ry = centreY - radius;
    const float rw = radius * 2.0f;
    const float angle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);

    auto arcRadius = radius - lineW * 0.5f;

    float position = sliderPos;
    auto isMouseOver = slider.isMouseOverOrDragging() && slider.isEnabled();

    auto thumb = juce::ImageCache::getFromMemory(BinaryData::thumb_png, BinaryData::thumb_pngSize);

    g.drawImageTransformed(thumb, juce::AffineTransform::rotation(angle - 90.0).translated(bounds.getCentreX(), bounds.getCentreY()));

    
    DBG(slider.getValue());
}


MainComponent::MainComponent()
 : voidKnob(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag, juce::Slider::TextEntryBoxPosition::NoTextBox)
{
    setSize (984, 335);

    voidKnob.setLookAndFeel(&knobLNF);

    background = juce::ImageCache::getFromMemory(BinaryData::background_png, BinaryData::background_pngSize);
    
    addAndMakeVisible(voidKnob);
}

MainComponent::~MainComponent()
{
    voidKnob.setLookAndFeel(nullptr);
}

//==============================================================================
void MainComponent::paint (juce::Graphics& g)
{
    g.drawImageWithin(background, 0, 0, getWidth(), getHeight(), juce::RectanglePlacement::fillDestination);


}

void MainComponent::resized()
{
    voidKnob.setBounds(465, 200, 80, 80);
}