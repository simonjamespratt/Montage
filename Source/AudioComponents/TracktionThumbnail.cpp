#include "TracktionThumbnail.h"

TracktionThumbnail::TracktionThumbnail(te::TransportControl &tc)
: transport(tc),
  smartThumbnail(
      transport.engine, te::AudioFile(transport.engine), *this, nullptr)
{
    timeRangeStart = 0.0;
    timeRangeEnd = 0.0;
}

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

// ========================================================================

ClipView::ClipView(te::Clip::Ptr c) : clip(c)
{}

// ========================================================================

AudioClipView::AudioClipView(te::Clip::Ptr c) : ClipView(c)
{
    updateThumbnail();
}

void AudioClipView::paint(juce::Graphics &g)
{
    // NB: There is a much more elaborate example of drawing the waveform for
    // the smart thumbnail in tracktion-src/examples/common/Components.cpp
    // AutioClipComponent

    if(thumbnail == nullptr) {
        return;
    }

    const auto clipPosition = getWaveAudioClip()->getPosition();
    auto timeRangeStart = clipPosition.getOffset();
    auto timeRangeEnd = clipPosition.getOffset() + clipPosition.getLength();
    auto r = getLocalBounds();
    const auto colour = findColour(juce::Label::textColourId);

    if(thumbnail->isGeneratingProxy()) {
        g.setColour(colour.withMultipliedBrightness(0.9f));
        g.drawText("Creating proxy: " +
                       juce::String(juce::roundToInt(
                           thumbnail->getProxyProgress() * 100.0f)) +
                       "%",
                   r,
                   juce::Justification::centred);
    } else {
        const float brightness = thumbnail->isOutOfDate() ? 0.4f : 0.66f;
        g.setColour(colour.withMultipliedBrightness(brightness));
        thumbnail->drawChannels(g,
                                r,
                                true,
                                {timeRangeStart, timeRangeEnd},
                                1.0f);
    }
}

// Private methods
void AudioClipView::updateThumbnail()
{
    if(auto *wac = getWaveAudioClip()) {
        te::AudioFile af(wac->getPlaybackFile());

        if(af.getFile().existsAsFile() && af.isValid()) {
            if(thumbnail == nullptr) {
                thumbnail =
                    std::make_unique<te::SmartThumbnail>(wac->edit.engine,
                                                         af,
                                                         *this,
                                                         &wac->edit);
            } else {
                thumbnail->setNewFile(af);
            }

        } else {
            thumbnail = nullptr;
        }
    }
}
