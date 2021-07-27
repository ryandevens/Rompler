/*
  ==============================================================================

    RompMenu.h
    Created: 23 Jul 2021 5:40:04pm
    Author:  ryand

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PulsarUIFeel.h"
#include "PluginProcessor.h"
#include "Artist.h"
#include "Database.h"


//==============================================================================
/*
*/
class RompMenu  : public juce::Component, juce::MouseListener
{
public:
    RompMenu(SamplerAudioProcessor& p);
    ~RompMenu() override;

    void paint (juce::Graphics&) override;
    void resized() override;

    void mouseDown(const juce::MouseEvent& e);
    void prepareMenu();

private:
    Database database;
    juce::PopupMenu menu;
    juce::DrawableText text;
    PulsarUIFeel pulsarUIFeel;
    SamplerAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (RompMenu)
};
