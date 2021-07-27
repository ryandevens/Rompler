/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "SampleSound.h"
#include "SamplingVoice.h"


//==============================================================================
/**
*/
class SamplerAudioProcessor  : public juce::AudioProcessor,
    public juce::ValueTree::Listener
{
public:
    //==============================================================================
    SamplerAudioProcessor();
    ~SamplerAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;
    void sampleLoop(juce::AudioBuffer<float>&);
    void channelLoop(juce::AudioBuffer<float>&, int sampleIndex);
    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    void loadFile();
    void loadFile (const juce::String& path);
    
    int getNumSamplerSounds() { return mSampler.getNumSounds(); }
    juce::AudioBuffer<float>& getWaveForm();
    
    void updateADSR();
    juce::ADSR::Parameters& getADSRParams() { return mADSRParams; }
    
    juce::AudioProcessorValueTreeState& getValueTree() { return mAPVTS; }
    juce::Atomic<bool>& isNotePlayed() { return mIsNotePlayed; }
    int getSampleCount() { return transportSampleIndex; }

    void updateLoop();
    void resetLoop(int numSamples);

    // loads a sample from the install location based on selection
    void loadFromDisk(juce::String& filePath);
    void loadFromIndex(int index);
    
private:
    juce::Synthesiser mSampler;
    const int mNumVoices { 3 };
    juce::File mFile;

    juce::ADSR::Parameters mADSRParams;
    
    juce::AudioFormatManager mFormatManager;
    
    
    juce::AudioProcessorValueTreeState mAPVTS;
    juce::AudioProcessorValueTreeState::ParameterLayout createParameters();
    void valueTreePropertyChanged (juce::ValueTree &treeWhosePropertyHasChanged, const juce::Identifier &property) override;
    
    juce::Atomic<bool> mShouldUpdate { false };
    juce::Atomic<bool> mIsNotePlayed { false };

    int transportSampleIndex = 0;
    juce::Atomic<int> mSampleStart   { 0 };
    juce::Atomic<int> mSampleEnd     { 96000 };

    std::atomic<float> startPosition { 0.f };

    

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SamplerAudioProcessor)
};
