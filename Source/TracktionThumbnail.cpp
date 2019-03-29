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
    repaint();
}
