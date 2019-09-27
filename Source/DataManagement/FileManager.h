/*
  ==============================================================================

    FileManager.h
    Created: 23 Apr 2019 7:58:42pm
    Author:  Simon Pratt

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "../Utilities/Identifiers.h"

namespace te = tracktion_engine;

class FileManager
{
public:
    FileManager();
    ~FileManager();

    void chooseFile();
    bool fileIsValid();
    ValueTree addSourceToState(ValueTree &sources);
    void loadExistingSourceFile(ValueTree &source);
    File getFile();
    te::AudioFile getAudioFile();

private:
    File file;
    te::AudioFile audioFile;
    int getNewSourceId(ValueTree &sources);
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FileManager)
};
