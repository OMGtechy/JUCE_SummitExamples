/*
  ==============================================================================

   This file is part of the Projucer live coding demo created for
   the JUCE Summit 2015.
   Copyright (c) 2015 - ROLI Ltd.

   Use this code at your own risk & have some fun with C++ live coding!

   This code is distributed in the hope that it will be useful, but WITHOUT ANY
   WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
   A PARTICULAR PURPOSE.

  ==============================================================================
*/

#ifndef SOUNDCOMPONENT_H_INCLUDED
#define SOUNDCOMPONENT_H_INCLUDED

//==============================================================================
class MainComponent   : public Component,
                        public AudioIODeviceCallback,
                        public MidiInputCallback
{
public:
    //==========================================================================
    MainComponent()
        : midiKeyboardState(),
          midiKeyboardComponent (midiKeyboardState, MidiKeyboardComponent::Orientation::horizontalKeyboard)
    {
        audioDeviceManager.initialiseWithDefaultDevices (0, 2);
        audioDeviceManager.addMidiInputCallback (String::empty, this);
        audioDeviceManager.addAudioCallback (this);
        
        StringArray devices = MidiInput::getDevices();
        for (int i = 0; i < devices.size(); ++i)
            audioDeviceManager.setMidiInputEnabled (devices[i], true);
        
        midiKeyboardState.addListener (&midiCollector);
        
        synth.addSound (new SineWaveSound());
        for (int i = 0; i < maxNumVoices; ++i)
            synth.addVoice (new SineWaveVoice());

        addAndMakeVisible (oscilloscope);
        addAndMakeVisible (midiKeyboardComponent);
        setSize (500, 400);
    }

    //==========================================================================
    ~MainComponent()
    {
        audioDeviceManager.removeAudioCallback (this);
        audioDeviceManager.removeMidiInputCallback (String::empty, this);
    }
    
    //==========================================================================
    void resized() override
    {
        Rectangle<int> r = getLocalBounds();
        midiKeyboardComponent.setBounds (r.removeFromBottom (80));
        oscilloscope.setBounds (r);
    }

    //==========================================================================
    void audioDeviceIOCallback (const float** /*inputChannelData*/,
                                int           /*numInputChannels*/,
                                float**       outputChannelData,
                                int           numOutputChannels,
                                int           numSamples) override
    {
        AudioBuffer<float> buffer (outputChannelData, numOutputChannels, numSamples);
        buffer.clear();
        
        MidiBuffer incomingMidi;
        midiCollector.removeNextBlockOfMessages (incomingMidi, numSamples);
        synth.renderNextBlock (buffer, incomingMidi, 0, numSamples);
        
        oscilloscope.pushBuffer (outputChannelData[0], numSamples);
    }

    //==========================================================================
    void audioDeviceAboutToStart (AudioIODevice* device) override
    {
        const double sampleRate = device->getCurrentSampleRate();
        midiCollector.reset (sampleRate);
        synth.setCurrentPlaybackSampleRate (sampleRate);
    }

    //==========================================================================
    virtual void audioDeviceStopped() override
    {
    }
    
    //==========================================================================
    void handleIncomingMidiMessage (MidiInput* /*source*/,
                                    const MidiMessage& message) override
    {
        midiCollector.addMessageToQueue (message);
    }

private:
    //==========================================================================
    AudioDeviceManager audioDeviceManager;
    Oscilloscope oscilloscope;
    Synthesiser synth;
    MidiMessageCollector midiCollector;
    MidiKeyboardState midiKeyboardState;
    MidiKeyboardComponent midiKeyboardComponent;
    const int maxNumVoices = 16;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};



#endif  // SOUNDCOMPONENT_H_INCLUDED
