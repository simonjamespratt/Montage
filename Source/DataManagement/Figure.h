#pragma once

#include <functional>
#include <juce_data_structures/juce_data_structures.h>

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

    juce::ValueTree getState() const;

    std::function<void(juce::Identifier propertyChanged)> onUpdated;
    void valueTreePropertyChanged(juce::ValueTree &vt,
                                  const juce::Identifier &property) override;

  private:
    juce::ValueTree state;
};
