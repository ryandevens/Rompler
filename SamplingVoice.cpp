/*
  ==============================================================================

    SamplingVoice.cpp
    Created: 12 Jul 2021 5:24:39pm
    Author:  ryand

  ==============================================================================
*/

#include "SamplingVoice.h"

SamplingVoice::SamplingVoice()
{
}

SamplingVoice::~SamplingVoice()
{
}

bool SamplingVoice::canPlaySound(juce::SynthesiserSound* sound)
{
    return dynamic_cast<const SampleSound*> (sound) != nullptr;
    
}

void SamplingVoice::startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound* s, int pitchWheel)
{
    if (auto* sound = dynamic_cast<SampleSound*> (s))
    {
        pitchRatio = std::pow(2.0, (midiNoteNumber - sound->midiRootNote) / 12.0)
            * sound->sourceSampleRate / getSampleRate();

        sourceSamplePosition = sound->getStartPosition();
        lgain = velocity;
        rgain = velocity;

        adsr.setSampleRate(sound->sourceSampleRate);
        adsr.setParameters(sound->params);

        adsr.noteOn();
    }
    else
    {
        jassertfalse; // this object can only play SamplerSounds!
    }
}

void SamplingVoice::stopNote(float velocity, bool allowTailOff)
{
    if (allowTailOff)
    {
        adsr.noteOff();
    }
    else
    {
        clearCurrentNote();
        adsr.reset();
    }
}

void SamplingVoice::pitchWheelMoved(int newValue)
{
}

void SamplingVoice::controllerMoved(int controllerNumber, int newValue)
{
}

void SamplingVoice::renderNextBlock(juce::AudioBuffer<float>& outputBuffer, int startSample, int numSamples)
{
    if (auto* playingSound = static_cast<SampleSound*> (getCurrentlyPlayingSound().get()))
    {
        sampleLoop(playingSound, outputBuffer);
    }
}

void SamplingVoice::sampleLoop(SampleSound* playingSound, juce::AudioBuffer<float>& outputBuffer)
{
    for (int sampleIndex = 0; sampleIndex < outputBuffer.getNumSamples(); sampleIndex++)
    {
        channelLoop(playingSound, outputBuffer, sampleIndex);
    }


}

void SamplingVoice::channelLoop(SampleSound* playingSound, juce::AudioBuffer<float>& outputBuffer, int sampleIndex)
{
    auto buffRead = playingSound->getAudioData()->getArrayOfReadPointers();
    auto buffWrite = outputBuffer.getArrayOfWritePointers();

    auto intSamplePos = (int)sourceSamplePosition;
    auto nextIndexCoef = (float)(sourceSamplePosition - intSamplePos);
    auto currentIndexCoef = 1.0f - nextIndexCoef;

    auto envVal = adsr.getNextSample();

    for (int channel = 0; channel < outputBuffer.getNumChannels(); channel++)
    {
        float sample = (buffRead[channel][(int)sourceSamplePosition] * currentIndexCoef) + (buffRead[channel][(int)sourceSamplePosition] * nextIndexCoef);
        sample *= envVal;

        buffWrite[channel][sampleIndex] = sample;
    }

    sourceSamplePosition += pitchRatio;

    if (sourceSamplePosition > playingSound->getEndPosition())
    {
        sourceSamplePosition = playingSound->getStartPosition();
    } 
}
