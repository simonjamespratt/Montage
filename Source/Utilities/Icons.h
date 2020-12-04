#pragma once
#include <juce_gui_basics/juce_gui_basics.h>

class Icons {
  public:
    Icons();
    ~Icons();

    enum IconType { Cross, Dash, Stop, Play, Pause };

    juce::DrawablePath getIcon(IconType iconType);

  private:
    // ICONS
    juce::Path crossPath;
    juce::DrawablePath cross;
    juce::Path dashPath;
    juce::DrawablePath dash;
    juce::Path stopPath;
    juce::DrawablePath stop;
    juce::Path playPath;
    juce::DrawablePath play;
    juce::Path pausePath;
    juce::DrawablePath pause;

    // DRAWING FUNCTIONS
    void drawCross();
    void drawDash();
    void drawStop();
    void drawPlay();
    void drawPause();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Icons)
};
