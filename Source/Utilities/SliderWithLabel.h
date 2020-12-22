#pragma once

#include <functional>
#include <juce_gui_basics/juce_gui_basics.h>

class SliderWithLabel : public juce::Component {
  public:
    SliderWithLabel(double &value,
                    juce::String labelText,
                    double rangeMin,
                    double rangeMax,
                    juce::String unit = "",
                    int decimalPlacesToDisplay = -1,
                    int textBoxWidth = -1);
    void resized() override;

    std::function<void()> onChange;

    void updateValue();

  private:
    double &m_value;
    juce::Slider slider;
    juce::Label label;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SliderWithLabel)
};
