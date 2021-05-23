#pragma once
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

    void togglePlayPause();
    void stop();
    bool handleKeyPress(const juce::KeyPress &key);
    std::function<void()> onTransportStopped;

  private:
    te::TransportControl &transport;

    juce::DrawableButton stopButton;
    juce::DrawableButton playPauseButton;
    juce::DrawableButton loopButton;

    TimeDisplay transportPosition;

    void updatePlayPauseButtonIcon();

    void changeListenerCallback(juce::ChangeBroadcaster *) override;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TransportController)
};
