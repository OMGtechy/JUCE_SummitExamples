#include "MainComponent.h"

MainContentComponent::MainContentComponent()
  : myFont (Typeface::createSystemTypefaceFor (BinaryData::myFont_ttf, BinaryData::myFont_ttfSize)),
    myText (CharPointer_UTF8 ("H\xe2\x84\xae\xc5\x82\xc5\x82o W\xe2\x98\xba\xd2\x91\xc5\x82""d"))
{
    myFont.setHeight (100.0f);

    setSize (600, 400);
}

void MainContentComponent::paint (Graphics& g)
{
    g.fillAll (Colour (0xff001F36));

    g.setFont (myFont);
    g.setColour (Colours::white);
    g.drawText (myText, getLocalBounds(), Justification::centred, true);
}
