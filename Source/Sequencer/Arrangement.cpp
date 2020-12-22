#include "Arrangement.h"

Arrangement::Arrangement(te::Edit &e, te::TransportControl &tc)
: edit(e), transport(tc)
{
    noOfTracks = 0;
}

Arrangement::~Arrangement()
{}

void Arrangement::paint(juce::Graphics &g)
{
    if(noOfTracks == 0) {
        g.setColour(juce::Colours::white);
        g.drawText("No figure selected at present",
                   getBounds(),
                   juce::Justification(36));
    } else {
        drawTrackDividers(g);
    }
}

void Arrangement::prepareArrangement(int noOfTracksToMake)
{
    thumbnails.clear();
    noOfTracks = noOfTracksToMake;
    repaint();
}

void Arrangement::drawTrackDividers(juce::Graphics &g)
{
    // divide component height by number of tracks
    auto distanceBetweenDividers = getHeight() / noOfTracks;
    double currentHeight = 0.0;
    // draw line offset from top for noOfTracks - 1
    for(int i = 0; i < noOfTracks - 1; i++) {
        currentHeight += distanceBetweenDividers;
        // draw line
        g.setColour(juce::Colours::darkred);
        g.fillRect(0.0, currentHeight, getWidth(), 1.0);
    }
}

void Arrangement::addClipToArrangement(
    juce::ReferenceCountedObjectPtr<tracktion_engine::WaveAudioClip> newClip,
    const int trackIndex,
    const double &clipStart,
    const double &clipEnd,
    const double &offset)
{
    ClipCoOrds clipCoOrds = getClipCoOrds(trackIndex, clipStart, clipEnd);

    addThumbnail(newClip, clipCoOrds, offset, (clipEnd - clipStart));
}

TrackHeightCoOrds Arrangement::getTrackHeightCoOrds(const int trackIndex)
{
    auto containerHeight = getHeight();
    float trackHeight = containerHeight / noOfTracks;
    // NB: trackIndices must start at 0 otherwise the next line won't work
    auto trackTop = trackHeight * trackIndex;
    auto trackBottom = trackTop + trackHeight;
    // return top and bottom
    return TrackHeightCoOrds {trackTop, trackBottom};
}

ClipWidthCoOrds Arrangement::getClipWidthCoOrds(const double clipStart,
                                                const double clipEnd)
{
    double editLength = edit.getLength();
    auto containerWidth = getWidth();
    float start = (clipStart / editLength) * containerWidth;
    float end = (clipEnd / editLength) * containerWidth;
    return ClipWidthCoOrds {start, end};
}

ClipCoOrds Arrangement::getClipCoOrds(const int trackIndex,
                                      const double clipStart,
                                      const double clipEnd)
{
    auto yAxis = getTrackHeightCoOrds(trackIndex);
    auto xAxis = getClipWidthCoOrds(clipStart, clipEnd);
    return ClipCoOrds {yAxis, xAxis};
}

void Arrangement::addThumbnail(
    juce::ReferenceCountedObjectPtr<tracktion_engine::WaveAudioClip> newClip,
    ClipCoOrds clipCoOrds,
    double offset,
    double clipLength)
{
    std::shared_ptr<TracktionThumbnail> thumbnail =
        std::make_shared<TracktionThumbnail>(transport);
    // NB: not sure it's worth putting these in a vector as the items in the
    // array are never accessed
    thumbnails.emplace_back(thumbnail);
    addAndMakeVisible(*thumbnail);
    thumbnail->setBounds(clipCoOrds.xAxis.start,
                         clipCoOrds.yAxis.top,
                         (clipCoOrds.xAxis.end - clipCoOrds.xAxis.start),
                         (clipCoOrds.yAxis.bottom - clipCoOrds.yAxis.top));
    thumbnail->setFile(newClip->getPlaybackFile(), offset, clipLength);
}
