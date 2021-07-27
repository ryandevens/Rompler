/*
  ==============================================================================

    SamplingVoice.h
    Created: 12 Jul 2021 5:24:39pm
    Author:  ryand

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "SampleSound.h"

class SamplingVoice : public juce::SamplerVoice
{
public:
    //==============================================================================
    /** Creates a SamplerVoice. */
    SamplingVoice();

    /** Destructor. */
    ~SamplingVoice() override;

    //==============================================================================
    bool canPlaySound(juce::SynthesiserSound*) override;

    void startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound*, int pitchWheel) override;
    void stopNote(float velocity, bool allowTailOff) override;

    void pitchWheelMoved(int newValue) override;
    void controllerMoved(int controllerNumber, int newValue) override;

    void renderNextBlock(juce::AudioBuffer<float>&, int startSample, int numSamples) override;
    using juce::SynthesiserVoice::renderNextBlock;

    void sampleLoop(SampleSound* sound, juce::AudioBuffer<float>& outputBuffer);
    void channelLoop(SampleSound* sound, juce::AudioBuffer<float>& outputBuffer, int sampleIndex);

   // juce::SynthesiserSound::Ptr getCurrentlyPlayingSound() const noexcept { return currentlyPlayingSound; }

private:
    //==============================================================================
    double pitchRatio = 0;
    double sourceSamplePosition = 0;
    float lgain = 0, rgain = 0;

    juce::ADSR adsr;

    JUCE_LEAK_DETECTOR(SamplingVoice)
};