/*
  ==============================================================================

    Cursor.cpp
    Created: 29 Mar 2019 4:54:01pm
    Author:  Simon Pratt

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "Cursor.h"

//==============================================================================
Cursor::Cursor(te::TransportControl &tc, te::Edit &e) : transport(tc), edit(e)
{
    cursorUpdater.setCallback([this] {
        updateCursorPosition();
        cursor.setFill(findColour(Label::textColourId));
        addAndMakeVisible(cursor);
    });
    cursorUpdater.startTimerHz(25);
}

Cursor::~Cursor()
{
    cursorUpdater.stopTimer();
}

void Cursor::mouseDown(const MouseEvent &e)
{
    transport.setUserDragging(true);
    mouseDrag(e);
}

void Cursor::mouseDrag(const MouseEvent &e)
{
    jassert(getWidth() > 0);
    const float proportion = e.position.x / getWidth();
    transport.position = proportion * edit.getLength();
}

void Cursor::mouseUp(const MouseEvent &)
{
    transport.setUserDragging(false);
}

void Cursor::updateCursorPosition()
{
    // NB: setting this using looping may not work for similar reasons to the above
    const double editLength = edit.getLength();
    const double proportion = editLength == 0.0 ? 0.0 : transport.getCurrentPosition() / editLength;

    auto r = getLocalBounds().toFloat();
    const float x = r.getWidth() * float(proportion);
    cursor.setRectangle(r.withWidth(2.0f).withX(x));
};
