#pragma once
#include "Identifiers.h"

#include <juce_audio_formats/juce_audio_formats.h>
#include <juce_data_structures/juce_data_structures.h>

class Source {
  public:
    Source(const juce::ValueTree &vt);
    Source(const juce::File &f);

    juce::Uuid getId() const;
    juce::File getFile() const;
    juce::String getFileName() const;
    double getFileLengthInSeconds() const;
    juce::ValueTree getState() const;

  private:
    juce::ValueTree state;
    juce::File file;
    double fileLengthInSeconds {0};

    void processFile(juce::File file);
};
