#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "./Indentifiers.h"
#include "./ContentContainer.h"

/*
ValueTree type Identifier: AppState
    child ValueTree type Identifier: Sources
        child ValueTree type Identifier:Source
            id: int
            filePath: string
            fileName: string

    child ValueTree type Identifier: Particles
        child ValueTree type Identifier: Particle
            id: int
            name: String (optional)
            sourceId: int
            rangeStart:double
            rangeEnd: double
*/

/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent : public Component
{
public:
    // constructor and deconstructor
    MainComponent();
    ~MainComponent();

    void resized() override;

    ValueTree appState;

private:
    ContentContainer contentContainer;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};
