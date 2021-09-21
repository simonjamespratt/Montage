#pragma once
#include <juce_gui_basics/juce_gui_basics.h>

class Cursor : public juce::Component {
  public:
    Cursor();
    void updatePosition(const double editLength,
                        const double transportPosition);

  private:
    juce::DrawableRectangle cursor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Cursor)
};
