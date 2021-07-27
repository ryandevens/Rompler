/*
  ==============================================================================

    FileDropZone.h
    Created: 10 Jul 2021 1:07:34pm
    Author:  ryand

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
//==============================================================================
/*
*/
class SampleThumbnail  : public juce::Component, public juce::FileDragAndDropTarget
{
public:
    SampleThumbnail(SamplerAudioProcessor& p);
    ~SampleThumbnail() override;

    void paint (juce::Graphics&) override;
    void paintWaveform(juce::Graphics& g);
    void paintPlayhead(juce::Graphics& g);
    void resized() override;

    bool isInterestedInFileDrag(const juce::StringArray& files) override;
    void filesDropped(const juce::StringArray& files, int x, int y) override;

private:
    std::vector<float> mAudioPoints;
    bool mShouldBePainting{ false };

    juce::String mFileName{ "" };
    juce::Font font{ "Consolas", "Regular", 20.f };



    SamplerAudioProcessor& processor;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SampleThumbnail)
};
