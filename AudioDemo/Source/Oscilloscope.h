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

#ifndef OSCILLOSCOPE_H_INCLUDED
#define OSCILLOSCOPE_H_INCLUDED

class Oscilloscope : public Component, private Timer
{
public:
    //==========================================================================
    Oscilloscope()
        : writePos (0)
    {
        startTimer (40);
    }
    
    //==========================================================================
    void pushBuffer (const float* data, int numSamples)
    {
        for (int i = 0; i < numSamples; ++i)
            buffer[++writePos % buffer.size()] = data[i];
    }
    
    //==========================================================================
    void paint (Graphics& g) override
    {
        g.fillAll (Colours::black);
        Rectangle<int> r = getLocalBounds();
        Path path;
        path.startNewSubPath (0, 0.5 * r.getHeight());
        const float bufferYscale = 3.0;
        
        int paintPos = 1;
        
        while (paintPos < buffer.size())
        {
            if (buffer[paintPos] > buffer[paintPos - 1] && buffer[paintPos] > 0 && buffer[paintPos - 1] < 0)
                break;
            
            ++paintPos;
        }

        const int posOffset = paintPos;
        
        while (paintPos < buffer.size())
        {
            Point<float> p ((paintPos - posOffset) * r.getWidth() / paintSize, 0.5 * ((bufferYscale * buffer[paintPos]) + 1) * r.getHeight());
            path.lineTo (p);
            ++paintPos;
        }
        
        g.setColour (Colours::lime);
        g.strokePath (path, PathStrokeType (2.0f));
    }
    
private:
    //==========================================================================
    void timerCallback() override
    {
        repaint();
    }
    
    //==========================================================================
    std::array<float, 1024> buffer;
    std::size_t writePos;
    
    const int bufferSize = 16384;
    const int paintSize = 256;
};



#endif  // OSCILLOSCOPE_H_INCLUDED
