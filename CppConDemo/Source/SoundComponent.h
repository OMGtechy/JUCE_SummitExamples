/*
  ==============================================================================

   This file is part of the Projucer live coding demo created for CppCon 2015.
   Copyright (c) 2015 - ROLI Ltd.

   Use this code at your own risk & have some fun with C++ live coding!

   This code is distributed in the hope that it will be useful, but WITHOUT ANY
   WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
   A PARTICULAR PURPOSE.

  ==============================================================================
*/

#ifndef SOUNDCOMPONENT_H_INCLUDED
#define SOUNDCOMPONENT_H_INCLUDED

#include "AudioLiveScrollingDisplay.h"


//==============================================================================
class SoundComponent   : public Component,
                         public AudioIODeviceCallback
{
public:
    SoundComponent()
      : liveAudioDisplayComp (75),
        sampleRate (0.0),
        frequency (440.0),  // <-- this determines the pitch of the sound!
        phase (0.0),
        phaseDelta (0.0),
        level (0.05)   // <-- this determines the overall volume!
    {
        // this connects the component to the default audio output of the computer
        deviceManager.initialiseWithDefaultDevices (0, 2);
        deviceManager.addAudioCallback (this);

        addAndMakeVisible (liveAudioDisplayComp);
        addAndMakeVisible (forumLinkButton);

        forumLinkButton.setColour (HyperlinkButton::textColourId, Colours::white);

        setSize (650, 300);
    }

    ~SoundComponent()
    {
        deviceManager.removeAudioCallback (this);
    }

    void paint (Graphics& g) override
    {
        g.fillAll (Colour (0xff274630));

        g.setFont (Font ("Arial", 20, Font::italic));
        g.setColour (Colours::yellow);
        g.drawText ("Modify the C++ code that generates the sound -- while it is playing!",
                    getLocalBounds(), Justification::centred);

        g.setFont (Font ("Arial", 16, Font::italic));
        g.drawText ("-- have a look at SoundComponent::audioDeviceIOCallback()",
                    Rectangle<int> {0, 0, getWidth(), getHeight() + 60},
                    Justification::centred);
    }

    void resized() override
    {
        Rectangle<int> r (getLocalBounds().reduced (8));
        liveAudioDisplayComp.setBounds (r.removeFromTop (64));
        forumLinkButton.setBounds (r.removeFromBottom (24));
    }

    void audioDeviceIOCallback (const float** /*inputChannelData*/,
                                int           /*numInputChannels*/,
                                float**       outputChannelData,
                                int           numOutputChannels,
                                int           numSamples) override
    {
        // this function performs the actual audio processing and writes the generated
        // audio data to the sound output buffer.
        //
        // --> feel free to play around with the numbers and formulae here
        //     and notice how the sound changes as you tweak the code!

        float tune = std::floor (2.5f + 1.5f * sin (time * 0.5f));

        for (int sample = 0; sample < numSamples; ++sample)
        {
            float currentSample = (float) std::sin (phase);
            currentSample += (float) std::sin (phase * 1.20f) * cos (time * 0.5f);

            for (float i = 0.0f; i < (4.0f + 4.0f * std::sin (time * 2.0f));)
            {
                currentSample += (float) std::sin (phase * i);
                i += jmax (tune, 0.1f);
            }

            phase += phaseDelta;
            outputChannelData[0][sample] = currentSample * level * (0.5f + 0.5 * std::sin (time));
            outputChannelData[1][sample] = currentSample * level * (0.5f + 0.5 * std::cos (time));
        }

        time += 0.02f;

        // update the visual scrolling audio display:
        liveAudioDisplayComp.pushAudioBuffer (outputChannelData, numOutputChannels, numSamples);
    }

    void audioDeviceAboutToStart (AudioIODevice* device) override
    {
        sampleRate = device->getCurrentSampleRate();
        updatePhaseDelta();
        liveAudioDisplayComp.clear();
    }

    virtual void audioDeviceStopped() override
    {
        liveAudioDisplayComp.clear();
    }


private:
    //==========================================================================
    void updatePhaseDelta()
    {
        const double cyclesPerSample = frequency / sampleRate;
        phaseDelta = cyclesPerSample * 2.0 * double_Pi;
    }

    AudioDeviceManager deviceManager;
    LiveScrollingAudioDisplay liveAudioDisplayComp;

    double sampleRate, frequency, phase, phaseDelta, level;
    float time;

    HyperlinkButton forumLinkButton { "If you want to discuss the Projucer with us, click here to visit the JUCE forum!",
                                      URL ("http://www.juce.com/forum") };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SoundComponent)
};



#endif  // SOUNDCOMPONENT_H_INCLUDED
