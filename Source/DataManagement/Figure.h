#pragma once

#include "DurationProtocolParams.h"
#include "NumberProtocolSettings.h"

#include <functional>
#include <juce_data_structures/juce_data_structures.h>

struct FigureCreationSettings {
    FigureCreationSettings(const DurationProtocolSettings &d,
                           const NumberProtocolSettings &ds,
                           const NumberProtocolSettings &ps)
    : durations(d), durationSelection(ds), particleSelection(ps)
    {}
    DurationProtocolSettings durations;
    NumberProtocolSettings durationSelection;
    NumberProtocolSettings particleSelection;
};

class Figure : public juce::ValueTree::Listener {
  public:
    Figure();
    Figure(const juce::ValueTree &v);
    ~Figure();

    juce::Uuid getId() const;

    juce::String getName() const;
    void setName(juce::String newName);

    bool getIsGenerated() const;
    void setIsGenerated(bool isGenerated);

    std::unique_ptr<FigureCreationSettings> getCreationSettings() const;
    void setCreationSettings(const FigureCreationSettings &settings);

    juce::ValueTree getState() const;

    std::function<void(juce::Identifier propertyChanged)> onUpdated;
    void valueTreePropertyChanged(juce::ValueTree &vt,
                                  const juce::Identifier &property) override;

  private:
    juce::ValueTree state;
};
