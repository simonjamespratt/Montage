#pragma once
#include <juce_gui_basics/juce_gui_basics.h>
#include <tracktion_engine/tracktion_engine.h>

namespace te = tracktion_engine;

class Cursor : public juce::Component,
               private juce::Timer,
               juce::ValueTree::Listener,
               juce::AsyncUpdater {
  public:
    Cursor(te::Edit &e);
    ~Cursor();

    void paint(juce::Graphics &g) override;

  private:
    void timerCallback() override;

    void valueTreePropertyChanged(juce::ValueTree &tree,
                                  const juce::Identifier &prop) override;

    void handleAsyncUpdate() override;

    te::Edit &edit;
    int xPosition = 0;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Cursor)
};
