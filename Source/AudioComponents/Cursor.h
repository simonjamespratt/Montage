#pragma once
#include <tracktion_engine/tracktion_engine.h>
namespace te = tracktion_engine;

class Cursor : public juce::Component {
  public:
    Cursor(te::TransportControl &tc, te::Edit &e);
    ~Cursor();

  private:
    juce::DrawableRectangle cursor;
    te::LambdaTimer cursorUpdater;
    te::TransportControl &transport;
    te::Edit &edit;

    void updateCursorPosition();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Cursor)
};
