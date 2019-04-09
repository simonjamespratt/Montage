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
Arrangement::Arrangement(te::Edit &e, te::TransportControl &tc) : edit(e), transport(tc)
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

    /*
        NB: ClipPosition has the following structure: { {startClip, endClip}, offset }
        Where the above mean:
        - startClip: the start position of the clip as placed on the track, e.g. 1.0 would mean the clip starts at 1 second from the beginning of the transport start
        - endClip: the end of the clip; the difference between clipEnd and clipStart gives you the length of the clip
        - offset: the start of the clip in relation to the start of the audio file, e.g. 1.0 would mean the clip start is 1 second from the beginning of the audio file
    */
    auto newClip = track->insertWaveClip(
        file.getFileNameWithoutExtension(),
        file,
        {{clipStart, clipEnd}, offset},
        false);

    if (!newClip)
    {
        return;
    }

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
    return TrackHeightCoOrds{trackTop, trackBottom};
}

ClipWidthCoOrds Arrangement::getClipWidthCoOrds(const double clipStart, const double clipEnd)
{
    double editLength = edit.getLength();
    auto containerWidth = getWidth();
    float start = (clipStart / editLength) * containerWidth;
    float end = (clipEnd / editLength) * containerWidth;
    return ClipWidthCoOrds{start, end};
}

ClipCoOrds Arrangement::getClipCoOrds(const int trackIndex, const double clipStart, const double clipEnd)
{
    auto yAxis = getTrackHeightCoOrds(trackIndex);
    auto xAxis = getClipWidthCoOrds(clipStart, clipEnd);
    return ClipCoOrds{yAxis, xAxis};
}

void Arrangement::addThumbnail(juce::ReferenceCountedObjectPtr<tracktion_engine::WaveAudioClip> newClip, ClipCoOrds clipCoOrds, double offset, double clipLength)
{
    // thumbnails.push_back(std::make_unique<TracktionThumbnail>(transport));

    // NB: This may not be the best way to do this, but it works for now!
    TracktionThumbnail *thumbnail = new TracktionThumbnail(transport);
    addAndMakeVisible(thumbnail);
    thumbnail->setBounds(
        clipCoOrds.xAxis.start,
        clipCoOrds.yAxis.top,
        (clipCoOrds.xAxis.end - clipCoOrds.xAxis.start),
        (clipCoOrds.yAxis.bottom - clipCoOrds.yAxis.top));
    thumbnail->setFile(newClip->getPlaybackFile(), offset, clipLength);
}
