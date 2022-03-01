#include "Cursor.h"

#include "Helpers.h"

Cursor::Cursor(te::Edit &e) : edit(e)
{
    // ensures that components underneath this one get mouse events. This
    // component doesn't need interactive functionality
    setInterceptsMouseClicks(false, false);

    startTimerHz(30);
    edit.state.addListener(this);
}

Cursor::~Cursor()
{
    stopTimer();
    edit.state.removeListener(this);
}

void Cursor::paint(juce::Graphics &g)
{
    // range
    auto loopRange = edit.getTransport().getLoopRange();
    if(!loopRange.isEmpty()) {
        auto editLength = edit.getLength();
        auto x1 =
            Helpers::timeToX(loopRange.getStart(), editLength, getWidth());
        auto x2 = Helpers::timeToX(loopRange.getEnd(), editLength, getWidth());

        g.setColour(juce::Colour::fromFloatRGBA(0.0f, 1.0f, 0.0f, 0.5f));
        g.fillRect(x1, 0, (x2 - x1), getHeight());
    }

    // playhead
    g.setColour(juce::Colours::white);
    g.drawRect(xPosition, 0, 2, getHeight());
}

// Private methods
void Cursor::timerCallback()
{
    auto newX = Helpers::timeToX(edit.getTransport().getCurrentPosition(),
                                 edit.getLength(),
                                 getWidth());
    if(newX != xPosition) {
        repaint(juce::jmin(newX, xPosition) - 1,
                0,
                juce::jmax(newX, xPosition) - juce::jmin(newX, xPosition) + 3,
                getHeight());
        xPosition = newX;
    }
}

void Cursor::valueTreePropertyChanged(juce::ValueTree &tree,
                                      const juce::Identifier &prop)
{
    if(prop == te::IDs::loopPoint1 || prop == te::IDs::loopPoint2) {
        triggerAsyncUpdate();
    }
}

void Cursor::handleAsyncUpdate()
{
    repaint();
}
