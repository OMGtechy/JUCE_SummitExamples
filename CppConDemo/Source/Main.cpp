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

#include "../JuceLibraryCode/JuceHeader.h"

#include "StartComponent.h"
#include "SoundComponent.h"
#include "GameOfLifeComponent.h"
#include "LiveConsoleComponent.h"

/*
   This file contains the startup code that is used when you compile and launch
   the whole app in a traditional C++ development environment.

   In the Projucer, you can also view the individual components separately.
   The Projucer can instantiate them in isolation - you don't need to
   write an app around them (which is what this file is doing).
*/

//==============================================================================
class ProjucerDemoApplication  : public JUCEApplication
{
public:
    ProjucerDemoApplication()  {}

    const String getApplicationName() override       { return ProjectInfo::projectName; }
    const String getApplicationVersion() override    { return ProjectInfo::versionString; }
    bool moreThanOneInstanceAllowed() override       { return true; }

    //==============================================================================
    void initialise (const String& commandLine) override
    {
        mainWindow = new MainWindow (getApplicationName());
    }

    void shutdown() override
    {
        mainWindow = nullptr;
    }

    //==============================================================================
    void systemRequestedQuit() override
    {
        quit();
    }

    //==============================================================================
    // This class is the desktop window for our app..
    class MainWindow    : public DocumentWindow
    {
    public:
        MainWindow (String name)  : DocumentWindow (name,
                                                    Colours::lightgrey,
                                                    DocumentWindow::allButtons)
        {
            setUsingNativeTitleBar (true);

            // You can un-comment these lines to choose which of the demo
            // pages should be displayed when the app is launched:

            Component* content = new ProjucerDemoStartComponent();
            //Component* content = new SoundComponent();
            //Component* content = new GameOfLifeComponent();

            setContentOwned (content, true);
            centreWithSize (getWidth(), getHeight());
            setVisible (true);
        }

        void closeButtonPressed() override
        {
            JUCEApplication::getInstance()->systemRequestedQuit();
        }

    private:
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainWindow)
    };

private:
    ScopedPointer<MainWindow> mainWindow;
};

//==============================================================================
// This macro generates the main() routine that launches the app.
START_JUCE_APPLICATION (ProjucerDemoApplication)
