#pragma once
#include <juce_gui_basics/juce_gui_basics.h>

class Icons {
  public:
    enum IconType { Cross, Dash, Stop, Play, Pause, Loop };

    static std::unique_ptr<juce::Drawable> getIcon(IconType type);

  private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Icons)
};
