/*
  ==============================================================================

    TracktionThumbnail.cpp
    Created: 25 Mar 2019 9:00:26am
    Author:  Simon Pratt

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "TracktionThumbnail.h"

//==============================================================================
TracktionThumbnail::TracktionThumbnail(tracktion_engine::TransportControl &tc) : transport(tc),
                                                                                 smartThumbnail(
                                                                                     transport.engine,
                                                                                     tracktion_engine::AudioFile(),
                                                                                     *this,
                                                                                     nullptr)
{
    cursorUpdater.setCallback([this] {
        updateCursorPosition();

        if (smartThumbnail.isGeneratingProxy() || smartThumbnail.isOutOfDate())
        {
            repaint();
        }

        cursor.setFill(findColour(Label::textColourId));
        addAndMakeVisible(cursor);
    });
}

TracktionThumbnail::~TracktionThumbnail()
{
}

void TracktionThumbnail::paint(Graphics &g)
{
    auto r = getLocalBounds();
    const auto colour = findColour(Label::textColourId);

    if (smartThumbnail.isGeneratingProxy())
    {
        g.setColour(colour.withMultipliedBrightness(0.9f));
        g.drawText("Creating proxy: " + String(roundToInt(smartThumbnail.getProxyProgress() * 100.0f)) + "%",
                   r, Justification::centred);
    }
    else
    {
        const float brightness = smartThumbnail.isOutOfDate() ? 0.4f : 0.66f;
        g.setColour(colour.withMultipliedBrightness(brightness));
        smartThumbnail.drawChannels(g, r, true, {0.0, smartThumbnail.getTotalLength()}, 1.0f);
    }
}

void TracktionThumbnail::setFile(const tracktion_engine::AudioFile &file)
{
    smartThumbnail.setNewFile(file);
    cursorUpdater.startTimerHz(25);
    repaint();
}

void TracktionThumbnail::mouseDown(const MouseEvent &e)
{
    transport.setUserDragging(true);
    mouseDrag(e);
}

void TracktionThumbnail::mouseDrag(const MouseEvent &e)
{
    jassert(getWidth() > 0);
    const float proportion = e.position.x / getWidth();
    // NB: setting tranposrt position like this may not work as it assumes that there is only one audio file in the whole sequencer!
    transport.position = proportion * transport.getLoopRange().getLength();
}

void TracktionThumbnail::mouseUp(const MouseEvent &)
{
    transport.setUserDragging(false);
}

void TracktionThumbnail::updateCursorPosition()
{
    // NB: setting this using looping may not work for similar reasons to the above
    const double loopLength = transport.getLoopRange().getLength();
    const double proportion = loopLength == 0.0 ? 0.0 : transport.getCurrentPosition() / loopLength;

    auto r = getLocalBounds().toFloat();
    const float x = r.getWidth() * float(proportion);
    cursor.setRectangle(r.withWidth(2.0f).withX(x));
};
