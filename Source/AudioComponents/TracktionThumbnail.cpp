#include "TracktionThumbnail.h"

TracktionThumbnail::TracktionThumbnail(tracktion_engine::TransportControl &tc)
: transport(tc),
  smartThumbnail(transport.engine,
                 tracktion_engine::AudioFile(transport.engine),
                 *this,
                 nullptr)
{
    timeRangeStart = 0.0;
    timeRangeEnd = 0.0;
}

TracktionThumbnail::~TracktionThumbnail()
{}

void TracktionThumbnail::paint(juce::Graphics &g)
{
    if(timeRangeStart < timeRangeEnd) {
        auto r = getLocalBounds();
        const auto colour = findColour(juce::Label::textColourId);

        if(smartThumbnail.isGeneratingProxy()) {
            g.setColour(colour.withMultipliedBrightness(0.9f));
            g.drawText("Creating proxy: " +
                           juce::String(juce::roundToInt(
                               smartThumbnail.getProxyProgress() * 100.0f)) +
                           "%",
                       r,
                       juce::Justification::centred);
        } else {
            const float brightness =
                smartThumbnail.isOutOfDate() ? 0.4f : 0.66f;
            g.setColour(colour.withMultipliedBrightness(brightness));
            smartThumbnail.drawChannels(g,
                                        r,
                                        true,
                                        {timeRangeStart, timeRangeEnd},
                                        1.0f);
        }
    }
}

void TracktionThumbnail::setFile(const tracktion_engine::AudioFile &file,
                                 const double &offset,
                                 const double &clipLength)
{
    timeRangeStart = offset;
    timeRangeEnd = offset + clipLength;
    smartThumbnail.setNewFile(file);
    repaint();
}
