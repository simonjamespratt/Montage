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
    delete[] tracks;
}

void Arrangement::paint(Graphics &g)
{
    drawTrackDividers(g);
}

void Arrangement::createTracks()
{
    noOfTracks = 3;
    tracks = new Track[noOfTracks];
    for (int i = 0; i < noOfTracks; i++)
    {
        edit.getOrInsertAudioTrackAt(i);
        Track track;
        track.id = i;
        track.yTop = 0;
        track.yBottom = 0;
        tracks[i] = track;
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
