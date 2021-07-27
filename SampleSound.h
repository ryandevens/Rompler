/*
  ==============================================================================

    SampleSound.h
    Created: 12 Jul 2021 5:09:22pm
    Author:  ryand

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"

class SampleSound : public juce::SynthesiserSound
{
public:

    SampleSound(const juce::String& name, juce::AudioFormatReader& source, const juce::BigInteger& midiNotes, int midiNoteForNormalPitch,
        double attackTimeSecs, double releaseTimeSecs, double maxSampleLengthSeconds);

    /** Destructor. */
    ~SampleSound() override;

    //==============================================================================
    /** Returns the sample's name */
    const juce::String& getName() const noexcept { return name; }

    /** Returns the audio sample data.
        This could return nullptr if there was a problem loading the data.
    */
    juce::AudioBuffer<float>* getAudioData() const noexcept;
    

    //==============================================================================
    /** Changes the parameters of the ADSR envelope which will be applied to the sample. */
    void setEnvelopeParameters(juce::ADSR::Parameters parametersToUse) { params = parametersToUse; }

    //==============================================================================
    bool appliesToNote(int midiNoteNumber) override;
    bool appliesToChannel(int midiChannel) override;

    double getStartPosition();
    double getEndPosition();
    double getSampleDur();

private:
    //==============================================================================
    friend class SamplingVoice;

    juce::String name;
    std::unique_ptr<juce::AudioBuffer<float>> data;
    double sourceSampleRate;

    juce::BigInteger midiNotes;
    int length = 0, midiRootNote = 0;
    double startPosition = 48000;
    double endPosition = 96000;
    juce::ADSR::Parameters params;



    JUCE_LEAK_DETECTOR(SampleSound)
};
