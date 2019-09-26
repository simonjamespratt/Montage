/*
  ==============================================================================

    Cursor.h
    Created: 29 Mar 2019 4:54:01pm
    Author:  Simon Pratt

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
namespace te = tracktion_engine;

//==============================================================================
/*
*/
class Cursor : public Component
{
public:
  Cursor(te::TransportControl &tc, te::Edit &e);
  ~Cursor();

private:
  DrawableRectangle cursor;
  te::LambdaTimer cursorUpdater;
  te::TransportControl &transport;
  te::Edit &edit;

  void updateCursorPosition();

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Cursor)
};
