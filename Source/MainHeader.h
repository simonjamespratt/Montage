/*
  ==============================================================================

    MainHeader.h
    Created: 1 May 2019 7:26:23pm
    Author:  Simon Pratt

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

namespace te = tracktion_engine;

//==============================================================================
/*
*/
class MainHeader : public Component
{
public:
    MainHeader(te::Engine &e);
    ~MainHeader();

    void paint(Graphics &) override;
    void resized() override;

private:
    te::Engine &engine;
    Label appName;
    TextButton settingsButton;

    void showAudioDeviceSettings(te::Engine &engine);
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainHeader)
};
