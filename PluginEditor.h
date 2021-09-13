/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "WaveThumbnail.h"
#include "ADSRComponent.h"
#include "RompMenu.h"
#include "PulsarUIFeel.h"


//==============================================================================
/**
*/
class RomplerAudioProcessorEditor  : public AudioProcessorEditor,
                                          public Timer
{
public:
    RomplerAudioProcessorEditor (RomplerAudioProcessor&);
    ~RomplerAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;
    
    void timerCallback() override;

private:    
    PulsarUIFeel pulsarFeel;
    std::unique_ptr<WaveThumbnail> mWaveThumbnail;
    std::unique_ptr<ADSRComponent> mADSR;
    std::unique_ptr<RompMenu> rompMenu;

    RomplerAudioProcessor& processor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (RomplerAudioProcessorEditor)
};
