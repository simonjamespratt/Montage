#pragma once

#include "Identifiers.h"

#include <tracktion_engine/tracktion_engine.h>

namespace te = tracktion_engine;

class FileManager {
  public:
    FileManager();
    ~FileManager();

    void chooseFile();

    bool fileIsValid(te::Engine &engine);

    juce::ValueTree addSourceToState(juce::ValueTree &sources);
    void loadExistingSourceFile(juce::ValueTree &source);
    juce::File getFile();

    te::AudioFile getAudioFile(te::Engine &engine);

  private:
    juce::File file;
    int getNewSourceId(juce::ValueTree &sources);
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FileManager)
};
