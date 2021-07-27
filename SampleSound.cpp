/*
  ==============================================================================

    SampleSound.cpp
    Created: 12 Jul 2021 5:14:57pm
    Author:  ryand

  ==============================================================================
*/

#include "SampleSound.h"

SampleSound::SampleSound(const juce::String& soundName, juce::AudioFormatReader& source, 
    const juce::BigInteger& notes, int midiNoteForNormalPitch, double attackTimeSecs, 
    double releaseTimeSecs, double maxSampleLengthSeconds) 
    : name(soundName),
    sourceSampleRate(source.sampleRate),
    midiNotes(notes),
    midiRootNote(midiNoteForNormalPitch)
    

{
    if (sourceSampleRate > 0 && source.lengthInSamples > 0)
    {
        length = juce::jmin((int)source.lengthInSamples, (int)(maxSampleLengthSeconds * sourceSampleRate));

        data.reset(new juce::AudioBuffer<float>(juce::jmin(2, (int)source.numChannels), length + 4));

        source.read(data.get(), 0, length + 4, 0, true, true);

        params.attack = static_cast<float> (attackTimeSecs);
        params.release = static_cast<float> (releaseTimeSecs);
    }
}

SampleSound::~SampleSound()
{
}

juce::AudioBuffer<float>* SampleSound::getAudioData() const noexcept
{
    return data.get();
}

bool SampleSound::appliesToNote(int midiNoteNumber)
{
    return true;
}

bool SampleSound::appliesToChannel(int midiChannel)
{
    return true;
}

double SampleSound::getStartPosition()
{
    return startPosition;
}

double SampleSound::getEndPosition()
{

    return endPosition;
}

double SampleSound::getSampleDur()
{
    return endPosition - startPosition;
}
