#pragma once
#include <tracktion_engine/tracktion_engine.h>
namespace te = tracktion_engine;

class MainHeader : public juce::Component {
  public:
    MainHeader(te::Engine &e);
    ~MainHeader();

    void paint(juce::Graphics &) override;
    void resized() override;

  private:
    te::Engine &engine;
    juce::TextButton settingsButton;

    void showAudioDeviceSettings(te::Engine &engine);
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainHeader)
};
