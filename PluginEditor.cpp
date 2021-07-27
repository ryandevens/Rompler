/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SamplerAudioProcessorEditor::SamplerAudioProcessorEditor (SamplerAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p), sampleThumbnail(p), mADSR(p), menu(p)
{
    setSize(600, 400);

    addAndMakeVisible(sampleThumbnail);

    addAndMakeVisible(mADSR);

    menu.setTopLeftPosition(260, 30);
    addAndMakeVisible(menu);

    startTimerHz(30);

    sampleThumbnail.setBoundsRelative(0.1f, 0.2f, 0.8f, 0.5);
    mADSR.setBoundsRelative(0.65f, 0.75f, 0.3f, 0.25f);

}

SamplerAudioProcessorEditor::~SamplerAudioProcessorEditor()
{
    stopTimer();

}

//==============================================================================
void SamplerAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll(juce::Colours::black);
}

void SamplerAudioProcessorEditor::timerCallback()
{
    repaint();
}


void SamplerAudioProcessorEditor::resized()
{
    sampleThumbnail.setBoundsRelative(0.1f, 0.2f, 0.8f, 0.5);
    mADSR.setBoundsRelative(0.65f, 0.75f, 0.3f, 0.25f);
}



