/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
RomplerAudioProcessorEditor::RomplerAudioProcessorEditor (RomplerAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
    setLookAndFeel(&pulsarFeel);
    mWaveThumbnail = std::make_unique<WaveThumbnail>(p);
    addAndMakeVisible (mWaveThumbnail.get());

    mADSR = std::make_unique<ADSRComponent>(p);
    addAndMakeVisible (mADSR.get());

    rompMenu = std::make_unique<RompMenu>(p);
    addAndMakeVisible(rompMenu.get());

    startTimerHz (30);
    
    setSize (600, 400);
}

RomplerAudioProcessorEditor::~RomplerAudioProcessorEditor()
{
    stopTimer();
}

//==============================================================================
void RomplerAudioProcessorEditor::paint (Graphics& g)
{
    auto color = Colour{ 0.001f, 0.f, 0.0005f, 1.f };
    g.fillAll(color);
}

void RomplerAudioProcessorEditor::resized()
{
    mWaveThumbnail->setBoundsRelative (0.0f, 0.2f, 1.0f, 0.6);
    mADSR->setBoundsRelative (0.0f, 0.8f, 1.0f, 0.2f);
    rompMenu->setBoundsRelative(0.f, 0.05f, 1.0f, 0.2f);
}

void RomplerAudioProcessorEditor::timerCallback()
{
    repaint();
}



