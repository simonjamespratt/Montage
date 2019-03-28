/*
  ==============================================================================

    Timeline.h
    Created: 27 Mar 2019 7:59:55pm
    Author:  Simon Pratt

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
namespace te = tracktion_engine;

//==============================================================================
/*
*/
class Timeline : public Component
{
public:
  Timeline(te::Edit &e);
  ~Timeline();

  void paint(Graphics &) override;

  void recalculate();

private:
  te::Edit &edit;
  double editLength;
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Timeline)
};
