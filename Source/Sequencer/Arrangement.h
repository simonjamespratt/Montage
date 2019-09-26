/*
  ==============================================================================

    Arrangement.h
    Created: 1 Apr 2019 7:56:07pm
    Author:  Simon Pratt

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "../AudioComponents/TracktionThumbnail.h"

namespace te = tracktion_engine;

//==============================================================================
/*
*/

struct TrackHeightCoOrds
{
    float top;
    float bottom;
};

struct ClipWidthCoOrds
{
    float start;
    float end;
};

struct ClipCoOrds
{
    TrackHeightCoOrds yAxis;
    ClipWidthCoOrds xAxis;
};

class Arrangement : public Component
{
public:
    Arrangement(te::Edit &e, te::TransportControl &tc);
    ~Arrangement();

    void paint(Graphics &) override;
    void prepareArrangement(int noOfTracksToMake);
    void addClipToArrangement(juce::ReferenceCountedObjectPtr<tracktion_engine::WaveAudioClip> newClip, const int trackIndex, const double &clipStart, const double &clipEnd, const double &offset);

private:
    te::Edit &edit;
    te::TransportControl &transport;
    // std::vector<std::unique_ptr<TracktionThumbnail>> thumbnails;

    int noOfTracks;
    void drawTrackDividers(Graphics &g);
    TrackHeightCoOrds getTrackHeightCoOrds(const int trackIndex);
    ClipWidthCoOrds getClipWidthCoOrds(const double clipStart, const double clipEnd);
    ClipCoOrds getClipCoOrds(const int trackIndex, const double clipStart, const double clipEnd);
    std::vector<std::shared_ptr<TracktionThumbnail>> thumbnails;
    void addThumbnail(juce::ReferenceCountedObjectPtr<tracktion_engine::WaveAudioClip> newClip, ClipCoOrds clipCoOrds, double offset, double clipLength);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Arrangement)
};
