/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "SampleThumbnail.h"
#include "ADSRComponent.h"
#include "RompMenu.h"
//==============================================================================
/**
*/
class SamplerAudioProcessorEditor  : public juce::AudioProcessorEditor, public juce::Timer

{
public:
    SamplerAudioProcessorEditor (SamplerAudioProcessor&);
    ~SamplerAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

    void timerCallback() override;


private:
    SampleThumbnail sampleThumbnail;
    ADSRComponent mADSR;
    RompMenu    menu;
    SamplerAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SamplerAudioProcessorEditor)
};
