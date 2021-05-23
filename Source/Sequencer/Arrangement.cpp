#include "Arrangement.h"

PositionableThumbnail::PositionableThumbnail(te::TransportControl &tc,
                                             te::AudioFile file,
                                             double editLength,
                                             double clipStart,
                                             double clipEnd,
                                             double offset,
                                             int trackIndex)
: thumbnail(tc), trackIndex(trackIndex)
{
    auto clipLength = clipEnd - clipStart;
    thumbnail.setFile(file, offset, clipLength);

    // Note that converting doubles to float may create precision
    // issues. Doesn't seem to be an issue at the moment
    normalisedStart = clipStart / editLength;
    normalisedEnd = clipEnd / editLength;
}

float PositionableThumbnail::getTop(float trackHeight)
{
    return trackHeight * trackIndex;
}

float PositionableThumbnail::getBottom(float trackHeight)
{
    return getTop(trackHeight) + trackHeight;
}

float PositionableThumbnail::getStart(int containerWidth)
{
    return normalisedStart * containerWidth;
}

float PositionableThumbnail::getEnd(int containerWidth)
{
    return normalisedEnd * containerWidth;
}

// ==========================================================

Arrangement::Arrangement(te::Edit &e,
                         te::TransportControl &tc,
                         float initialTrackHeight)
: edit(e), transport(tc), trackHeight(initialTrackHeight)
{
    noOfTracks = 0;
}

Arrangement::~Arrangement()
{}

void Arrangement::paint(juce::Graphics &g)
{
    if(noOfTracks > 0) {
        drawTrackDividers(g);
    }
}

void Arrangement::resized()
{
    for(auto &&t : thumbnails) {
        auto start = t->getStart(getWidth());
        auto end = t->getEnd(getWidth());
        auto top = t->getTop(trackHeight);
        auto bottom = t->getBottom(trackHeight);

        t->thumbnail.setBounds(start, top, (end - start), (bottom - top));
    }
}

void Arrangement::prepare(int noOfTracksToMake)
{
    thumbnails.clear();
    noOfTracks = noOfTracksToMake;
    resized();
    repaint();
}

void Arrangement::clear()
{
    thumbnails.clear();
    noOfTracks = 0;
    resized();
    repaint();
}

void Arrangement::addClip(
    juce::ReferenceCountedObjectPtr<tracktion_engine::WaveAudioClip> newClip,
    const int trackIndex,
    const double &clipStart,
    const double &clipEnd,
    const double &offset)
{
    auto thumbnail =
        std::make_unique<PositionableThumbnail>(transport,
                                                newClip->getPlaybackFile(),
                                                edit.getLength(),
                                                clipStart,
                                                clipEnd,
                                                offset,
                                                trackIndex);

    addAndMakeVisible(thumbnail->thumbnail);
    thumbnails.emplace_back(std::move(thumbnail));
    resized();
}

void Arrangement::setTrackHeight(float newHeight)
{
    trackHeight = newHeight;
}

// Private methods
void Arrangement::drawTrackDividers(juce::Graphics &g)
{
    g.setColour(juce::Colours::cornflowerblue);

    float currentPosition = 0;
    for(int i = 0; i < noOfTracks; i++) {
        currentPosition += trackHeight;
        g.fillRect(0.0, (currentPosition - 0.5), float(getWidth()), 0.5f);
    }
}
