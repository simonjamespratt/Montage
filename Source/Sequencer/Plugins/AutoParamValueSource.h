#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include <tracktion_engine/tracktion_engine.h>

namespace te = tracktion_engine;

class AutoParamValueSource : public juce::Value::ValueSource,
                             private te::AutomatableParameter::Listener {
  public:
    AutoParamValueSource(te::AutomatableParameter::Ptr p);
    ~AutoParamValueSource() override;
    juce::var getValue() const override;
    void setValue(const juce::var &newValue) override;

    static void bindSliderToParameter(juce::Slider &s,
                                      te::AutomatableParameter &p);

  private:
    te::AutomatableParameter::Ptr param;

    void curveHasChanged(te::AutomatableParameter &) override;
    void currentValueChanged(te::AutomatableParameter &,
                             float /*newValue*/) override;
};
