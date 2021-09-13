/*
  ==============================================================================

    Artist.h
    Created: 24 Jul 2021 5:06:59pm
    Author:  ryand

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>


class Artist
{
public:
    Artist() {}
    Artist(juce::String& name) : artistName(name) {} 
    ~Artist() {}

    void setArtistName(const juce::String& name) { artistName = name; }
    
    void addRomple(juce::String& name) { rompleNames.add(name); }

    juce::String& getArtistName() { return artistName; }

    juce::String getRompleName(int index) { return rompleNames[index]; }

    int numberOfRomples() { return rompleNames.size(); }

private:
    juce::String artistName;
    juce::StringArray rompleNames;
};

