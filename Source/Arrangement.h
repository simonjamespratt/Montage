/*
  ==============================================================================

    Arrangement.h
    Created: 1 Apr 2019 7:56:07pm
    Author:  Simon Pratt

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

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
  Arrangement(te::Edit &e);
  ~Arrangement();

  void paint(Graphics &) override;
  void addClipToTrack(const int trackIndex, const File &file);

private:
  te::Edit &edit;
  int noOfTracks;
  void createTracks();
  void drawTrackDividers(Graphics &g);
  TrackHeightCoOrds getTrackHeightCoOrds(const int trackIndex);
  ClipWidthCoOrds getClipWidthCoOrds(const double offset, const double clipLength);
  ClipCoOrds getClipCoOrds(const int trackIndex, const double offset, const double clipLength);
  void drawClip(Graphics &g, ClipCoOrds clip);

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Arrangement)
};
