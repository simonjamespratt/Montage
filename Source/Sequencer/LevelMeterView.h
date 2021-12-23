#pragma once

#include <tracktion_engine/tracktion_engine.h>

namespace te = tracktion_engine;

// Taken from:
// https://forum.juce.com/t/using-levelmeterplugin-from-tracktion-audiotrack/35890/13
class LevelMeterView : public juce::Component, private juce::Timer {
  public:
    LevelMeterView(te::LevelMeasurer &lm);
    ~LevelMeterView();

    void paint(juce::Graphics &g) override;

  private:
    void timerCallback() override;

    te::LevelMeasurer &levelMeasurer;
    te::LevelMeasurer::Client levelClient;

    juce::Range<double> dBRange {-30.0, 3.0};
    double currentLeveldB {0.0};
    double prevLeveldB {0.0};
};
