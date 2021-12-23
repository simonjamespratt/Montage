#include "LevelMeterView.h"

LevelMeterView::LevelMeterView(te::LevelMeasurer &lm) : levelMeasurer(lm)
{
    setOpaque(true);
    levelMeasurer.addClient(levelClient);
    startTimerHz(30);
}

LevelMeterView::~LevelMeterView()
{
    levelMeasurer.removeClient(levelClient);
    stopTimer();
}

void LevelMeterView::paint(juce::Graphics &g)
{
    g.fillAll(juce::Colours::black);

    const auto meterHeight {double(getHeight())};
    const auto meterWidth {double(getWidth())};
    const auto offset {fabs(dBRange.getStart())};
    const auto scaleFactor {meterHeight / (dBRange.getEnd() + offset)};

    // draw 0dB line
    g.setColour(juce::Colours::lightgrey);
    g.fillRect(0.0f,
               float(meterHeight - (offset * scaleFactor)),
               float(meterWidth),
               1.0f);

    // draw meter gain bar
    g.setColour(juce::Colours::green);
    auto displayBarHeight = ((currentLeveldB + offset) * scaleFactor);
    if(displayBarHeight > 0) {
        g.fillRect(0.0f,
                   float(meterHeight - displayBarHeight),
                   float(meterWidth),
                   float(displayBarHeight));
    }
}

void LevelMeterView::timerCallback()
{
    prevLeveldB = currentLeveldB;

    currentLeveldB = levelClient.getAndClearAudioLevel(0).dB;

    // Now we give the level bar fading charcteristics.
    // And, the below conversions, decibelsToGain and gainToDecibels,
    // take care of 0dB, which will never fade!...but a gain of 1.0 (0dB) will.

    const auto prevLevel {juce::Decibels::decibelsToGain(prevLeveldB)};

    if(prevLeveldB > currentLeveldB) {
        currentLeveldB = juce::Decibels::gainToDecibels(prevLevel * 0.94);
    }

    // the test below may save some unnecessary paints
    if(currentLeveldB != prevLeveldB) {
        repaint();
    }
}
