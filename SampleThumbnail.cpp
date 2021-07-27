/*
  ==============================================================================

    SampleThumbnail.cpp
    Created: 10 Jul 2021 1:07:34pm
    Author:  ryand

  ==============================================================================
*/

#include <JuceHeader.h>
#include "SampleThumbnail.h"

//==============================================================================
SampleThumbnail::SampleThumbnail(SamplerAudioProcessor& p) : processor(p)
{
}

SampleThumbnail::~SampleThumbnail()
{
}

void SampleThumbnail::paint (juce::Graphics& g)
{
    
    juce::Rectangle<float> frame = getLocalBounds().toFloat();
    g.setColour(juce::Colours::dimgrey.darker(0.9f));
    g.fillRoundedRectangle(frame, 5.f);
    g.setColour(juce::Colours::grey.brighter(0.4f));
    g.drawRoundedRectangle(frame, 5.f, 5.f);


    if (processor.getWaveForm().getNumSamples() > 0)
    {
        paintWaveform(g);
        paintPlayhead(g);
    }

    if (processor.getWaveForm().getNumSamples() == 0)
    {
        g.setFont(font);
        g.setColour(juce::Colours::white);
        g.drawFittedText("Drop an Audio File to Load", getLocalBounds(), juce::Justification::centred, 1);
    }
}

void SampleThumbnail::paintWaveform(juce::Graphics& g)
{
    juce::Path p;
    mAudioPoints.clear();
    auto numSamples = processor.getWaveForm().getNumSamples();

    auto ratio = numSamples / getWidth();
    auto buffRead = processor.getWaveForm().getReadPointer(0);

    //scale audio file to window on x axis
    for (int sample = 0; sample < numSamples; sample += ratio)
    {
        mAudioPoints.push_back(buffRead[sample]);
    }

    g.setColour(juce::Colours::white);
    p.startNewSubPath(0, getHeight() / 2);

    //scale on y axis
    for (int sample = 0; sample < mAudioPoints.size(); ++sample)
    {
        auto point = juce::jmap<float>(mAudioPoints[sample], -1.0f, 1.0f, getHeight(), 0);
        p.lineTo(sample, point);
    }

    g.strokePath(p, juce::PathStrokeType(2));

    g.setColour(juce::Colours::black);
    g.setFont(15.0f);
    auto textBounds = getLocalBounds().reduced(10, 10);
    g.drawFittedText(mFileName, textBounds, juce::Justification::topRight, 1);
}


void SampleThumbnail::paintPlayhead(juce::Graphics& g)
{

    // TODO, scale playhead position between start and end of loop rather than 0 -> getWidth()
    auto playHeadPosition = juce::jmap<int>(processor.getSampleCount(), 0, processor.getWaveForm().getNumSamples(), 0, getWidth());

    g.setColour(juce::Colours::white);
    g.drawLine(playHeadPosition, 0, playHeadPosition, getHeight(), 2.0f);

    g.setColour(juce::Colours::black.withAlpha(0.2f));
    g.fillRect(0, 0, playHeadPosition, getHeight());
}

void SampleThumbnail::resized()
{

}

bool SampleThumbnail::isInterestedInFileDrag(const juce::StringArray& files)
{
    for (auto file : files)
    {
        if (file.contains(".wav") || file.contains(".mp3") || file.contains(".aif"))
        {
            return true;
        }
    }

    return false;
}

void SampleThumbnail::filesDropped(const juce::StringArray& files, int x, int y)
{
    for (auto file : files)
    {
        if (isInterestedInFileDrag(file))
        {
            auto myFile = std::make_unique<juce::File>(file);
            mFileName = myFile->getFileNameWithoutExtension();

            processor.loadFile(file);
        }
    }

    repaint();
}


