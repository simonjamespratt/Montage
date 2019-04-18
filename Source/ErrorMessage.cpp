/*
  ==============================================================================

    ErrorMessage.cpp
    Created: 21 Apr 2019 10:04:45pm
    Author:  Simon Pratt

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "ErrorMessage.h"

//==============================================================================
ErrorMessage::ErrorMessage(String &messageReceived) : message(messageReceived)
{
}

ErrorMessage::~ErrorMessage()
{
}

void ErrorMessage::paint (Graphics& g)
{
    g.fillAll (Colours::white);   // clear the background
    g.setColour (Colours::red);
    g.setFont (14.0f);
    g.drawText (message, getLocalBounds(),
                Justification::centred, true);
}
