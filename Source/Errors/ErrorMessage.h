/*
  ==============================================================================

    ErrorMessage.h
    Created: 21 Apr 2019 10:04:45pm
    Author:  Simon Pratt

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/*
*/
class ErrorMessage    : public Component
{
public:
    ErrorMessage(String &messageReceived);
    ~ErrorMessage();

    void paint (Graphics&) override;

private:
    String &message;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ErrorMessage)
};
