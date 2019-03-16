/*
  ==============================================================================

    Sequencer.h
    Created: 13 Mar 2019 7:34:49pm
    Author:  Simon Pratt

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/*
*/
class Sequencer : public Component, public ChangeListener
{
public:
  Sequencer();
  ~Sequencer();

  void resized() override;

private:
  tracktion_engine::Engine engine;
  std::unique_ptr<tracktion_engine::Edit> edit;
  TextButton settingsButton;
  TextButton playPauseButton;
  Label editNameLabel;
  Label transportPosition;
  void changeListenerCallback(ChangeBroadcaster *source) override;
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Sequencer)
};
