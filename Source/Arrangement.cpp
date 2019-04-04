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
Arrangement::Arrangement(te::Edit &e) : edit(e)
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

void Arrangement::addClipToTrack(const int trackIndex, const File &file)
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

    auto newClip = track->insertWaveClip(
        file.getFileNameWithoutExtension(),
        file, );
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

void Arrangement::drawClip(Graphics &g, ClipCoOrds clip)
{
    g.setColour(Colours::lavenderblush);
    g.fillRect(clip.xAxis.start, clip.yAxis.top, clip.xAxis.end, clip.yAxis.bottom);
}
