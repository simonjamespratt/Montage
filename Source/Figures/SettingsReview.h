#pragma once

#include "Figure.h"

#include <juce_gui_basics/juce_gui_basics.h>

class SettingsReview : public juce::Component {
  public:
    SettingsReview(juce::String headingText = "",
                   juce::Colour textColour = juce::Colours::grey);
    void paint(juce::Graphics &g) override;

    void setMessage(const DurationProtocolSettings &settings);
    void setMessage(const NumberProtocolSettings &settings);
    void clear();

  private:
    juce::String getCollectionValuesAsText(std::vector<int> &collection);
    juce::String getCollectionValuesAsText(std::vector<double> &collection);
    juce::String message;
    juce::String heading;
    juce::Colour txtColour;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SettingsReview)
};
