/*
  ==============================================================================

    Icons.h
    Created: 5 May 2019 7:05:01pm
    Author:  Simon Pratt

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/*
*/
class Icons
{
public:
    Icons();
    ~Icons();

    enum IconType
    {
        Cross,
        Dash,
        Stop,
        Play,
        Pause
    };

    DrawablePath getIcon(IconType iconType);

private:
    // ICONS
    Path crossPath;
    DrawablePath cross;
    Path dashPath;
    DrawablePath dash;
    Path stopPath;
    DrawablePath stop;
    Path playPath;
    DrawablePath play;
    Path pausePath;
    DrawablePath pause;

    // DRAWING FUNCTIONS
    void drawCross();
    void drawDash();
    void drawStop();
    void drawPlay();
    void drawPause();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Icons)
};
