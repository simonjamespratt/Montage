#pragma once
#include "TimeDisplay.h"

#include <functional>

namespace te = tracktion_engine;

class TransportController : public juce::Component,
                            private juce::ChangeListener {
  public:
    TransportController(te::TransportControl &tc);
    ~TransportController();

    void paint(juce::Graphics &) override;
    void resized() override;

    std::function<void()> onTransportStopped;

  private:
    void changeListenerCallback(juce::ChangeBroadcaster *) override;
    void updatePlayPauseButtonIcon();
    void togglePlayPause();
    void stop();
    bool handleKeyPress(const juce::KeyPress &key);

    te::TransportControl &transport;

    juce::DrawableButton stopButton;
    juce::DrawableButton playPauseButton;
    juce::DrawableButton loopButton;

    TimeDisplay transportPosition;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TransportController)
};
