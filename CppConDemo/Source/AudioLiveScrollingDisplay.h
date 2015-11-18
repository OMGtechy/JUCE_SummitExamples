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

#ifndef AUDIOLIVESCROLLINGDISPLAY_H_INCLUDED
#define AUDIOLIVESCROLLINGDISPLAY_H_INCLUDED


//==============================================================================
/* This component scrolls a continuous waveform showing the audio data that
   is pushed into it via its pushAudioBuffer function.
*/
class LiveScrollingAudioDisplay  : public Component,
                                   private Timer
{
public:
    LiveScrollingAudioDisplay (int refreshRateHz)
        : nextSample (0), subSample (0), accumulator (0)
    {
        setOpaque (true);
        clear();

        startTimerHz (refreshRateHz); // use a timer to keep repainting this component
    }

    void pushAudioBuffer (float** channelData,
                          int     numChannels,
                          int     numSamples)
    {
        for (int i = 0; i < numSamples; ++i)
        {
            float sample = 0;

            for (int chan = 0; chan < numChannels; ++chan)
                if (channelData[chan] != nullptr)
                    sample += std::abs (channelData[chan][i]); // find the sum of all the channels

            pushSample (10.0f * sample); // boost the level to make it more easily visible.
        }
    }

    void clear()
    {
        zeromem (samples, sizeof (samples));
        accumulator = 0;
        subSample = 0;
    }

private:
    //==========================================================================
    float samples[1024];
    int nextSample, subSample;
    float accumulator;

    void paint (Graphics& g) override
    {
        g.fillAll (Colours::black);

        const float midY = getHeight() * 0.5f;
        int samplesAgo = (nextSample + numElementsInArray (samples) - 1);

        RectangleList<float> waveform;
        waveform.ensureStorageAllocated ((int) numElementsInArray (samples));

        for (int x = jmin (getWidth(), (int) numElementsInArray (samples)); --x >= 0;)
        {
            const float sampleSize = midY * samples [samplesAgo-- % numElementsInArray (samples)];
            waveform.addWithoutMerging (Rectangle<float> ((float) x, midY - sampleSize, 1.0f, sampleSize * 2.0f));
        }

        g.setColour (Colours::lightgreen);
        g.fillRectList (waveform);
    }

    void timerCallback() override
    {
        repaint();
    }

    void pushSample (const float newSample)
    {
        accumulator += newSample;

        if (subSample == 0)
        {
            const int inputSamplesPerPixel = 200;

            samples[nextSample] = accumulator / inputSamplesPerPixel;
            nextSample = (nextSample + 1) % numElementsInArray (samples);
            subSample = inputSamplesPerPixel;
            accumulator = 0;
        }
        else
        {
            --subSample;
        }
    }

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LiveScrollingAudioDisplay);
};


#endif  // AUDIOLIVESCROLLINGDISPLAY_H_INCLUDED
