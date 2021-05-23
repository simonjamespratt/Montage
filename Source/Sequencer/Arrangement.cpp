#include "Arrangement.h"

Arrangement::Arrangement(te::Edit &e, te::TransportControl &tc)
: edit(e), transport(tc), trackHeight(75)
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
    auto requiredHeight = trackHeight * noOfTracks;
    setSize(getWidth(), requiredHeight);
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

void Arrangement::drawTrackDividers(juce::Graphics &g)
{
    g.setColour(juce::Colours::cornflowerblue);

    float currentPosition = 0;
    for(int i = 0; i < noOfTracks; i++) {
        currentPosition += trackHeight;
        g.fillRect(0.0, (currentPosition - 0.5), float(getWidth()), 0.5f);
    }
}

void Arrangement::addClip(
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
    // NB: trackIndices must start at 0 otherwise the next line won't work
    auto trackTop = trackHeight * trackIndex;
    auto trackBottom = trackTop + trackHeight;
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
