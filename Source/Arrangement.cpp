/*
  ==============================================================================

    Arrangement.cpp
    Created: 1 Apr 2019 7:56:07pm
    Author:  Simon Pratt

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "Arrangement.h"

//==============================================================================
Arrangement::Arrangement(te::Edit &e, te::TransportControl &tc) : edit(e), transport(tc), thumbnail(transport)
{
    createTracks();
}

Arrangement::~Arrangement()
{
}

void Arrangement::paint(Graphics &g)
{
    drawTrackDividers(g);
}

void Arrangement::resized()
{
    // thumbnail.setBounds(0.0, 0.0, getWidth(), getHeight());
}

void Arrangement::addClipToTrack(const File &file, const int trackIndex, const double &clipStart, const double &clipEnd, const double &offset)
{
    auto track = edit.getOrInsertAudioTrackAt(trackIndex);

    if (!track)
    {
        return;
    }

    // Remove clips - NB: this will need revisiting as you won't always want to remove all clips!
    auto clipsToRemove = track->getClips();
    for (int i = clipsToRemove.size(); --i >= 0;)
    {
        clipsToRemove.getUnchecked(i)->removeFromParentTrack();
    }

    // TODO: ClipPosition: work out how to select a portion of an audio file as the clip instead of using all of it (i.e. a particle)
    // TODO: ClipPosition: work out how to position a clip on a track at a certain offset from the tranport start
    auto newClip = track->insertWaveClip(
        file.getFileNameWithoutExtension(),
        file,
        {{clipStart, clipEnd}, offset}, // NB. this is a ClipPosition, where: { {startClip, endClip}, offset }
        false);

    if (!newClip)
    {
        return;
    }

    ClipCoOrds clipCoOrds = getClipCoOrds(trackIndex, offset, (clipEnd - clipStart));

    addThumbnail(newClip, clipCoOrds);
}

void Arrangement::createTracks()
{
    noOfTracks = 3;
    for (int i = 0; i < noOfTracks; i++)
    {
        edit.getOrInsertAudioTrackAt(i);
    }
    repaint();
}

void Arrangement::drawTrackDividers(Graphics &g)
{
    // divide component height by number of tracks
    auto distanceBetweenDividers = getHeight() / noOfTracks;
    double currentHeight = 0.0;
    // draw line offset from top for noOfTracks - 1
    for (int i = 0; i < noOfTracks - 1; i++)
    {
        currentHeight += distanceBetweenDividers;
        // draw line
        g.setColour(Colours::darkred);
        g.fillRect(0.0, currentHeight, getWidth(), 1.0);
    }
}

TrackHeightCoOrds Arrangement::getTrackHeightCoOrds(const int trackIndex)
{
    auto containerHeight = getHeight();
    float trackHeight = containerHeight / noOfTracks;
    // NB: trackIndices must start at 0 otherwise the next line won't work
    auto trackTop = trackHeight * trackIndex;
    auto trackBottom = trackTop + trackHeight;
    // return top and bottom
    return TrackHeightCoOrds{trackTop, trackBottom};
}

ClipWidthCoOrds Arrangement::getClipWidthCoOrds(const double offset, const double clipLength)
{
    double editLength = edit.getLength();
    auto containerWidth = getWidth();
    float start = (offset / editLength) * containerWidth;
    float end = ((offset + clipLength) / editLength) * containerWidth;
    return ClipWidthCoOrds{start, end};
}

ClipCoOrds Arrangement::getClipCoOrds(const int trackIndex, const double offset, const double clipLength)
{
    auto yAxis = getTrackHeightCoOrds(trackIndex);
    auto xAxis = getClipWidthCoOrds(offset, clipLength);
    return ClipCoOrds{yAxis, xAxis};
}

void Arrangement::addThumbnail(juce::ReferenceCountedObjectPtr<tracktion_engine::WaveAudioClip> newClip, ClipCoOrds clipCoOrds)
{
    addAndMakeVisible(&thumbnail);
    thumbnail.setBounds(
        clipCoOrds.xAxis.start,
        clipCoOrds.yAxis.top,
        (clipCoOrds.xAxis.end - clipCoOrds.xAxis.start),
        (clipCoOrds.yAxis.bottom - clipCoOrds.yAxis.top));
    // thumbnail.setBounds(0.0, 0.0, getWidth(), getHeight());
    thumbnail.setFile(newClip->getPlaybackFile());

}
