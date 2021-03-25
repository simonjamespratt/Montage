#pragma once
#include "Identifiers.h"
#include "ParticleList.h"

#include <array>
#include <juce_gui_basics/juce_gui_basics.h>

class ParticlesManifest : public juce::Component,
                          public juce::TableListBoxModel {
  public:
    ParticlesManifest(ParticleList pl);
    ~ParticlesManifest();

    void resized() override;

    // TableListBoxModel overrides
    int getNumRows() override;
    void paintRowBackground(juce::Graphics &g,
                            int rowNumber,
                            int /*width*/,
                            int /*height*/,
                            bool rowIsSelected) override;
    void paintCell(juce::Graphics &g,
                   int rowNumber,
                   int columnId,
                   int width,
                   int height,
                   bool rowIsSelected) override;

  private:
    enum Columns : int {
        id = 1,
        sourceId = 2,
        sourceFileName = 3,
        start = 4,
        end = 5
    };

    ParticleList particleList;

    juce::TableListBox table;
    int numRows = 0;

    juce::Label heading;

    void refreshView();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ParticlesManifest)
};
