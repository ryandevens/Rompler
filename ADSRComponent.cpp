/*
  ==============================================================================

    ADSRComponent.cpp
    Created: 12 Jul 2021 7:57:36am
    Author:  ryand

  ==============================================================================
*/

#include <JuceHeader.h>
#include "ADSRComponent.h"

//==============================================================================
ADSRComponent::ADSRComponent(SamplerAudioProcessor& p) : processor (p)
{
    //Attack Slider
    setLookAndFeel(&pulsarFeel);

    mAttackSlider.setSliderStyle(juce::Slider::SliderStyle::LinearBarVertical);
    mAttackSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 40, 20);
    //mAttackSlider.setRange (0.0f, 5.0f, 0.01f);
    addAndMakeVisible(mAttackSlider);

    mAttackLabel.setFont(10.0f);
    mAttackLabel.setText("Attack", juce::NotificationType::dontSendNotification);
    mAttackLabel.setJustificationType(juce::Justification::centredTop);
    mAttackLabel.attachToComponent(&mAttackSlider, false);

    mAttackAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processor.getValueTree(), "ATTACK", mAttackSlider);

    //Decay Slider
    mDecaySlider.setSliderStyle(juce::Slider::SliderStyle::LinearBarVertical);
    mDecaySlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 40, 20);
    //mDecaySlider.setRange (0.0f, 5.0f, 0.01f);
    addAndMakeVisible(mDecaySlider);

    mDecayLabel.setFont(10.0f);
    mDecayLabel.setText("Decay", juce::NotificationType::dontSendNotification);
    mDecayLabel.setJustificationType(juce::Justification::centredTop);
    mDecayLabel.attachToComponent(&mDecaySlider, false);

    mDecayAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processor.getValueTree(), "DECAY", mDecaySlider);

    //Sustain Slider
    mSustainSlider.setSliderStyle(juce::Slider::SliderStyle::LinearBarVertical);
    mSustainSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 40, 20);
    //mSustainSlider.setRange (0.0f, 1.0f, 0.01f);
    addAndMakeVisible(mSustainSlider);

    mSustainLabel.setFont(10.0f);
    mSustainLabel.setText("Sustain", juce::NotificationType::dontSendNotification);
    mSustainLabel.setJustificationType(juce::Justification::centredTop);
    mSustainLabel.attachToComponent(&mSustainSlider, false);

    mSustainAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processor.getValueTree(), "SUSTAIN", mSustainSlider);

    //Release Slider
    mReleaseSlider.setSliderStyle(juce::Slider::SliderStyle::LinearBarVertical);
    mReleaseSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 40, 20);
    //mReleaseSlider.setRange (0.0f, 5.0f, 0.01f);
    addAndMakeVisible(mReleaseSlider);

    mReleaseLabel.setFont(10.0f);
    mReleaseLabel.setText("Release", juce::NotificationType::dontSendNotification);
    mReleaseLabel.setJustificationType(juce::Justification::centredTop);
    mReleaseLabel.attachToComponent(&mReleaseSlider, false);

    mReleaseAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processor.getValueTree(), "RELEASE", mReleaseSlider);
}

ADSRComponent::~ADSRComponent()
{
    setLookAndFeel(nullptr);
}

void ADSRComponent::paint (juce::Graphics& g)
{
    g.fillAll(juce::Colours::black);
}

void ADSRComponent::resized()
{
    const auto startX = 0.025f;
    const auto startY = 0.2f;
    const auto dialWidth = 0.2f;
    const auto dialHeight = 0.75f;

    mAttackSlider.setBoundsRelative(startX, startY, dialWidth, dialHeight);
    mDecaySlider.setBoundsRelative(startX + dialWidth + 0.05f, startY, dialWidth, dialHeight);
    mSustainSlider.setBoundsRelative(startX + (dialWidth * 2) + 0.1f, startY, dialWidth, dialHeight);
    mReleaseSlider.setBoundsRelative(startX + (dialWidth * 3) + 0.15f, startY, dialWidth, dialHeight);
}
