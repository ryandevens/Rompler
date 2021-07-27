/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SamplerAudioProcessor::SamplerAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ), mAPVTS(*this, nullptr, "Parameters", createParameters())
#endif
{
    mFormatManager.registerBasicFormats();
    mAPVTS.state.addListener(this);

    for (int i = 0; i < mNumVoices; i++)
    {
        mSampler.addVoice(new SamplingVoice());
    }
    //loadFromDisk(0);
}

SamplerAudioProcessor::~SamplerAudioProcessor()
{
    mAPVTS.state.removeListener(this);
}

//==============================================================================
const juce::String SamplerAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool SamplerAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool SamplerAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool SamplerAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double SamplerAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int SamplerAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int SamplerAudioProcessor::getCurrentProgram()
{
    return 0;
}

void SamplerAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String SamplerAudioProcessor::getProgramName (int index)
{
    return {};
}

void SamplerAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void SamplerAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    mSampler.setCurrentPlaybackSampleRate(sampleRate);
    updateADSR();
}

void SamplerAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool SamplerAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void SamplerAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    if (mShouldUpdate.get())
    {
        updateADSR();
        updateLoop();
        mShouldUpdate = false;
    }

    juce::MidiMessage m;
    juce::MidiBuffer::Iterator it{ midiMessages };
    int sample;

    while (it.getNextEvent(m, sample))
    {
        if (m.isNoteOn())
            mIsNotePlayed = true;
        else if (m.isNoteOff())
            mIsNotePlayed = false;
    }

    sampleLoop(buffer);
    // sampleCount iterates by blocks.  Starts with a note on message and ends with note off

    mSampler.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());
}

void SamplerAudioProcessor::sampleLoop(juce::AudioBuffer<float>& buffer)
{
    for (int sampleIndex = 0; sampleIndex < buffer.getNumSamples(); sampleIndex++)
    {
        if (mIsNotePlayed.get())
        {
            if (transportSampleIndex > mSampleEnd.get())
            {
                transportSampleIndex = mSampleStart.get();
            }
            transportSampleIndex++;
        }
        else
        {
            transportSampleIndex = mSampleStart.get();
        }   
    }
}

void SamplerAudioProcessor::channelLoop(juce::AudioBuffer<float>& buffer, int sampleIndex)
{
    for (int channel = 0; channel < buffer.getNumChannels(); channel++)
    {

    }
}

//==============================================================================
bool SamplerAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* SamplerAudioProcessor::createEditor()
{
    return new SamplerAudioProcessorEditor (*this);
}

//==============================================================================
void SamplerAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void SamplerAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

void SamplerAudioProcessor::loadFile()
{
    mSampler.clearSounds();

    juce::FileChooser chooser{ "Please load a file" };

    if (chooser.browseForFileToOpen())
    {
        auto file = chooser.getResult();
        // the reader can be a local variable here since it's not needed by the SamplerSound after this
        std::unique_ptr<juce::AudioFormatReader> reader{ mFormatManager.createReaderFor(file) };
        if (reader)
        {
            juce::BigInteger range;
            range.setRange(0, 128, true);
            mSampler.addSound(new SampleSound("Sample", *reader, range, 60, 0.1, 0.1, 10.0));
            resetLoop(reader->lengthInSamples);

        }

    }
}

void SamplerAudioProcessor::loadFile(const juce::String& path)
{
    mSampler.clearSounds();

    auto file= juce::File(path);
    // the reader can be a local variable here since it's not needed by the other classes after this
    std::unique_ptr<juce::AudioFormatReader> reader{ mFormatManager.createReaderFor(file) };
    if (reader)
    {
        juce::BigInteger range;
        range.setRange(0, 128, true);
        mSampler.addSound(new SampleSound("Sample", *reader, range, 60, 0.1, 0.1, 10.0));
        updateADSR();
        resetLoop(reader->lengthInSamples);

    }

}

juce::AudioBuffer<float>& SamplerAudioProcessor::getWaveForm()
{
    // get the last added synth sound as a SamplerSound*
    auto sound = dynamic_cast<SampleSound*>(mSampler.getSound(mSampler.getNumSounds() - 1).get());
    if (sound)
    {
        return *sound->getAudioData();
    }
    // just in case it somehow happens that the sound doesn't exist or isn't a SamplerSound,
    // return a static instance of an empty AudioBuffer here...
    static juce::AudioBuffer<float> dummybuffer;

    return dummybuffer;
}

void SamplerAudioProcessor::updateADSR()
{
    mADSRParams.attack = mAPVTS.getRawParameterValue("ATTACK")->load();
    mADSRParams.decay = mAPVTS.getRawParameterValue("DECAY")->load();
    mADSRParams.sustain = mAPVTS.getRawParameterValue("SUSTAIN")->load();
    mADSRParams.release = mAPVTS.getRawParameterValue("RELEASE")->load();

    for (int i = 0; i < mSampler.getNumSounds(); ++i)
    {
        if (auto sound = dynamic_cast<SampleSound*>(mSampler.getSound(i).get()))
        {
            sound->setEnvelopeParameters(mADSRParams);
        }
    }
}

void SamplerAudioProcessor::updateLoop()
{
    auto start = mAPVTS.getRawParameterValue("Loop Start")->load() * ;

    mSampleStart = mAPVTS.getRawParameterValue("Loop Start")->load();
    mSampleEnd = mAPVTS.getRawParameterValue("Loop End")->load();

}

void SamplerAudioProcessor::resetLoop(int numSamples)
{
    mSampleStart = 0;
    mSampleEnd = numSamples;
}

void SamplerAudioProcessor::loadFromDisk(juce::String& filePath)
{
    auto parent = juce::File::File();
    parent = juce::File::getCurrentWorkingDirectory();
    auto file = parent.getChildFile(filePath);
    mSampler.clearSounds();

    std::unique_ptr<juce::AudioFormatReader> reader{ mFormatManager.createReaderFor(file) };
    if (reader)
    {
        juce::BigInteger range;
        range.setRange(0, 128, true);
        mSampler.addSound(new SampleSound("Sample", *reader, range, 60, 0.1, 0.1, 10.0));
        updateADSR();
        resetLoop(reader->lengthInSamples);
    }

}

void SamplerAudioProcessor::loadFromIndex(int index)
{
    auto parent = juce::File::File();
    parent = juce::File::getCurrentWorkingDirectory();
    auto file = parent.getChildFile(filePaths[index]);
    mSampler.clearSounds();

    std::unique_ptr<juce::AudioFormatReader> reader{ mFormatManager.createReaderFor(file) };
    if (reader)
    {
        juce::BigInteger range;
        range.setRange(0, 128, true);
        mSampler.addSound(new SampleSound("Sample", *reader, range, 60, 0.1, 0.1, 10.0));
        updateADSR();
        resetLoop(reader->lengthInSamples);
    }

}


juce::AudioProcessorValueTreeState::ParameterLayout SamplerAudioProcessor::createParameters()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;

    params.push_back(std::make_unique<juce::AudioParameterFloat>("ATTACK", "Attack", 0.0f, 5.0f, 0.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("DECAY", "Decay", 0.0f, 5.0f, 2.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("SUSTAIN", "Sustain", 0.0f, 1.0f, 1.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("RELEASE", "Release", 0.0f, 5.0f, 0.0f));

    params.push_back(std::make_unique<juce::AudioParameterFloat>("Loop Start", "Loop Start", 0.0f, 99.0f, 50.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("Loop End", "Loop End", 1.0f, 100.0f, 60.0f));

    return { params.begin(), params.end() };
}

void SamplerAudioProcessor::valueTreePropertyChanged(juce::ValueTree& treeWhosePropertyHasChanged, const juce::Identifier& property)
{
    mShouldUpdate = true;
}



//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SamplerAudioProcessor();
}
