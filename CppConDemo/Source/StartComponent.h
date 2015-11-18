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

#ifndef STARTCOMPONENT_H_INCLUDED
#define STARTCOMPONENT_H_INCLUDED


//==============================================================================
class ProjucerDemoStartComponent : public Component,
                                   private Timer
{
public:
    ProjucerDemoStartComponent()
    {
        setSize (600, 600);
        startTimerHz (60);
    }

    void paint (Graphics& g) override
    {
        g.fillAll (Colour (0xff333333));  // <-- change the background colour here!

        g.setColour (Colour (0xff88e13a));
        g.setFont (Font ("Arial", 20.0f, Font::italic));   // <-- change the font here!

        g.drawMultiLineText ("If you edit the C++ code for this class, "
                             "you can change its appearance while it's running!",   // <-- change the text here!
                             20, 30, 380);  // <-- change the text position here!

        g.setColour (Colour (0xff333333));
        g.drawText ("TADA!", getLocalBounds(), Justification::centred);

        g.setColour (Colours::white.withAlpha (0.5f)); // <-- change the opacity of the spiral here!

        paintSpiral (g);
    }

    void paintSpiral (Graphics& g)
    {
        int numCircles = 9; // <-- try editing some variables or logic!
        int tailLength = 15;

        float theta = 2.0f * M_PI / (float) jmax (numCircles, 1);

        for (float i = 0.0f; i < 2.0f * M_PI; i += theta)
        {
            for (int j = 0; j < tailLength; ++j)
            {
                float radius        = jmax (tailLength, 0) + 2.0f - j;
                float mainRadius    = 80.0f + j * 5.0f;    // <-- drag these numbers with the slider!
                float frequencyX    = 0.1f;
                float frequencyY    = 0.2f;

                frequencyY = frequencyX;      // <-- comment out this line to desync X and Y

                g.fillEllipse (getWidth() * 0.5f +  mainRadius * std::sin (i + std::sin (time - j*frequencyX)) - radius * 0.5f,
                               getHeight() * 0.5f + mainRadius * std::cos (i + std::sin (time - j*frequencyY)) - radius * 0.5f,
                               radius,
                               radius);   // <-- play around with this formula!
            }
        }
    }

    void timerCallback() override
    {
        time += 0.01f;   // <-- you can change the speed here!
        repaint();
    }

    float time = 0.0f;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ProjucerDemoStartComponent)
};


#endif  // STARTCOMPONENT_H_INCLUDED
