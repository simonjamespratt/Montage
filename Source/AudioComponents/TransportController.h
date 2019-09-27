/*
  ==============================================================================

    TransportController.h
    Created: 7 May 2019 7:20:07pm
    Author:  Simon Pratt

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "../Utilities/Icons.h"
#include "./TimeDisplay.h"

namespace te = tracktion_engine;

//==============================================================================
/*
*/
class TransportController : public Component, public ChangeListener
{
public:
    TransportController(te::TransportControl &tc);
    ~TransportController();

    void paint(Graphics &) override;
    void resized() override;

private:
    te::TransportControl &transport;
    Icons icons;
    DrawablePath stopIcon;
    DrawableButton stopButton;

    DrawablePath playIcon;
    DrawablePath pauseIcon;
    DrawableButton playPauseButton;

    TimeDisplay transportPosition;

    void updatePlayPauseButtonIcon();
    void togglePlayPause();
    void stop();

    void changeListenerCallback(ChangeBroadcaster *) override;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TransportController)
};
