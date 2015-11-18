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


class LiveConsoleComponent    : public Component
{
public:
    /* 
        Whatever you "print" in this function will get echoed to the GUI 
        component, so you can experiment with basic C++ functionality..
    */
    void printYourStuffHere()
    {
        print ("Hello World!");
        print ("Try changing the logic inside the printYourStuffHere() function,");
        print ("and watch the output change.");
        newline();

        print ("here are some numbers:");
        
        for (int i = 0; i < 9; ++i) // <-- try changing the loop condition!
            print (i);
        
        newline();

        print ("Here are some calculations...");
        newline();

        for (int i = 0; i < 11; ++i) // <-- try changing the loop condition!
        {
            if (i % 2 == 0)
                print (String (i) + " is even.");
            else
                print (String (i) + " is odd.");
        }
        
        newline();

        for (int i = 2; i < 50; ++i) // <-- try changing the loop condition!
            if (isPrime (i))
                print (String (i) + " is prime.");
    }

    static bool isPrime (int n)
    {
        for (int i = 2; i < n; ++i)
            if (n % i == 0)
                return false;

        return true;
    }

private:
    LiveConsoleComponent()
    {
        setSize (600, 600);
    }

    void paint (Graphics& g) override
    {
        strings.clear();
        printYourStuffHere();

        g.fillAll (Colours::black);
        
        Font f (13.0f);
        g.setFont (Font (Font::getDefaultMonospacedFontName(), 15.0f, Font::plain));
        g.setColour (Colour (0xff73ff47));

        float y = 0;
        
        for (auto& s : strings)
        {
            y += f.getHeight();
            g.drawSingleLineText (s, 8, y);
        }
    }

    void newline()
    {
        print (String());
    }

    template <typename Type>
    void print (Type value)
    {
        strings.add (String (value));
    }

private:
    StringArray strings;
};

