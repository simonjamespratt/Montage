/*
  ==============================================================================

    Arrangement.h
    Created: 1 Apr 2019 7:56:07pm
    Author:  Simon Pratt

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "./TracktionThumbnail.h"

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
  void addClipToTrack(const File &file, const int trackIndex, const double &clipStart, const double &clipEnd, const double &offset);

private:
  te::Edit &edit;
  te::TransportControl &transport;
  // std::vector<std::unique_ptr<TracktionThumbnail>> thumbnails;

  int noOfTracks;
  void createTracks();
  void drawTrackDividers(Graphics &g);
  TrackHeightCoOrds getTrackHeightCoOrds(const int trackIndex);
  ClipWidthCoOrds getClipWidthCoOrds(const double offset, const double clipLength);
  ClipCoOrds getClipCoOrds(const int trackIndex, const double offset, const double clipLength);
  void addThumbnail(juce::ReferenceCountedObjectPtr<tracktion_engine::WaveAudioClip> newCllip, ClipCoOrds clipCoOrds);

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Arrangement)
};
