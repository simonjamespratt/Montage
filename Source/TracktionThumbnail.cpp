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
    timeRangeStart = 0.0;
    timeRangeEnd = 0.0;
}

TracktionThumbnail::~TracktionThumbnail()
{
}

void TracktionThumbnail::paint(Graphics &g)
{
    if (timeRangeStart < timeRangeEnd)
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
            smartThumbnail.drawChannels(g, r, true, {timeRangeStart, timeRangeEnd}, 1.0f);
        }
    }
}

void TracktionThumbnail::setFile(const tracktion_engine::AudioFile &file, const double &offset, const double &clipLength)
{
    timeRangeStart = offset;
    timeRangeEnd = offset + clipLength;
    smartThumbnail.setNewFile(file);
    repaint();
}
