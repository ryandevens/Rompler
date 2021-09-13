/*
  ==============================================================================

    WaveThumbnail.cpp
    Created: 4 Apr 2020 5:47:38pm
    Author:  Joshua Hodge

  ==============================================================================
*/

#include <JuceHeader.h>
#include "WaveThumbnail.h"

//==============================================================================
WaveThumbnail::WaveThumbnail (RomplerAudioProcessor& p) : processor (p)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

}

WaveThumbnail::~WaveThumbnail()
{
}

void WaveThumbnail::paint (Graphics& g)
{
    auto color = Colour{ 0.001f, 0.f, 0.0005f, 1.f };
    g.fillAll (color);
    
    auto waveform = processor.getWaveForm();
    
    if (waveform.getNumSamples() > 0)
    {
        if (waveform.getNumChannels() > 1)
        {
            drawStereo(g, waveform);
        }
        else
        {
            drawMono(g, waveform);
        }

        g.setColour(Colours::white);
        g.setFont(15.0f);
        auto textBounds = getLocalBounds().reduced(10, 10);
        g.drawFittedText(mFileName, textBounds, Justification::topRight, 1);

        auto playHeadPosition = jmap<int>(processor.getSampleCount(), 0, processor.getWaveForm().getNumSamples(), 0, getWidth());

        g.setColour(Colours::white);
        g.drawLine(playHeadPosition, 0, playHeadPosition, getHeight(), 2.0f);

        g.setColour(Colours::black.withAlpha(0.2f));
        g.fillRect(0, 0, playHeadPosition, getHeight());
    }
    else
    {
        g.setColour (Colours::white);
        g.setFont (40.0f);
        g.drawFittedText ("Drop an Audio File to Load", getLocalBounds(), Justification::centred, 1);
    }
}

void WaveThumbnail::drawMono(Graphics& g, AudioBuffer<float>& waveform)
{
    Path p;
    mAudioPoints.clear();

    auto ratio = waveform.getNumSamples() / getWidth();
    auto buffer = waveform.getReadPointer(0);

    //scale audio file to window on x axis
    for (int sample = 0; sample < waveform.getNumSamples(); sample += ratio)
    {
        mAudioPoints.push_back(buffer[sample]);
    }

    g.setColour(Colours::lightgreen);
    p.startNewSubPath(0, getHeight() / 2);

    //scale on y axis
    for (int sample = 0; sample < mAudioPoints.size(); ++sample)
    {
        auto point = jmap<float>(mAudioPoints[sample], -1.0f, 1.0f, getHeight(), 0);
        p.lineTo(sample, point);
    }

    g.strokePath(p, PathStrokeType(0.5f));

   
}

void WaveThumbnail::drawStereo(Graphics& g, AudioBuffer<float>& waveform)
{
    for (int channel = 0; channel < waveform.getNumChannels(); channel++)
    {
        Path p;
        mAudioPoints.clear();

        auto ratio = waveform.getNumSamples() / getWidth();
        auto buffer = waveform.getReadPointer(channel);

        //scale audio file to window on x axis
        for (int sample = 0; sample < waveform.getNumSamples(); sample += ratio)
        {
            mAudioPoints.push_back(buffer[sample]);
        }

        g.setColour(Colours::lightgreen);

        float height = 0.f;

        if (channel == 0)
        {
            height = getHeight() * 0.2f;
        }
        if (channel == 1)
        {
            height = getHeight() * 0.7f;
        }

        p.startNewSubPath(0, height);

        //scale on y axis
        for (int sample = 0; sample < mAudioPoints.size(); ++sample)
        {
            auto point = jmap<float>(mAudioPoints[sample], -1.0f, 1.0f, height + (getHeight()/4), height - (getHeight()/4));
            p.lineTo(sample, point);
        }

        g.strokePath(p, PathStrokeType(0.5f));
    }

}

void WaveThumbnail::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

}

bool WaveThumbnail::isInterestedInFileDrag (const StringArray& files)
{
    for (auto file : files)
    {
        if (file.contains (".wav") || file.contains (".mp3") || file.contains (".aif"))
        {
            return true;
        }
    }
    
    return false;
}

void WaveThumbnail::filesDropped (const StringArray& files, int x, int y)
{
    for (auto file : files)
    {
        if (isInterestedInFileDrag (file))
        {
            auto myFile = std::make_unique<File>(file);
            mFileName = myFile->getFileNameWithoutExtension();
            
            processor.loadFile (file);
        }
    }
    
    repaint();
}
