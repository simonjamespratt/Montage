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
struct Track
{
  int id;
  int yTop;
  int yBottom;
};

class Arrangement : public Component
{
public:
  Arrangement(te::Edit &e);
  ~Arrangement();

  void paint(Graphics &) override;

private:
  te::Edit &edit;
  // NB: This may need reworking - consider using std:vector
  Track *tracks;
  int noOfTracks;
  void createTracks();
  void drawTrackDividers(Graphics &g);
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Arrangement)
};
