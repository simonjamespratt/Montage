#pragma once
#include "Icons.h"
#include "TimeDisplay.h"

#include <functional>

namespace te = tracktion_engine;

class TransportController : public juce::Component,
                            public juce::ChangeListener {
  public:
    TransportController(te::TransportControl &tc);
    ~TransportController();

    void paint(juce::Graphics &) override;
    void resized() override;

    std::function<void()> onTransportStopped;

  private:
    te::TransportControl &transport;
    Icons icons;
    juce::DrawablePath stopIcon;
    juce::DrawableButton stopButton;

    juce::DrawablePath playIcon;
    juce::DrawablePath pauseIcon;
    juce::DrawableButton playPauseButton;

    TimeDisplay transportPosition;

    void updatePlayPauseButtonIcon();
    void togglePlayPause();
    void stop();

    void changeListenerCallback(juce::ChangeBroadcaster *) override;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TransportController)
};
