#pragma once
#include "EventList.h"
#include "FigureList.h"
#include "ParticleList.h"
#include "SourceList.h"

#include <functional>
#include <juce_data_structures/juce_data_structures.h>

class ProjectState : public juce::ValueTree::Listener {
  public:
    struct Status {
        bool hasUnsavedChanges;
        bool hasProjectDirectory;
        bool figureEditHasUnsavedChanges;
    };

    enum Action {
        NoAction,
        StateChange,
        SaveProject,
        LoadProject,
        CreateProject
    };

    ProjectState();
    ProjectState(juce::ValueTree vt);
    ProjectState(const ProjectState &ps);
    ~ProjectState();

    ParticleList getParticleList() const;
    SourceList getSourceList() const;
    FigureList getFigureList() const;
    EventList getEventList() const;
    EventList getEventList(const Figure &f) const;

    void create(const juce::File &directory);
    void save();
    void load(const juce::File &directory);

    Status getStatus() const;
    std::function<void(Status s, Action a)> onStatusChanged;

    juce::File getProjectDirectory() const;

    juce::File getFileForFigure(const Figure &f) const;
    void deleteFileForFigure(const Figure &f);

    void setFigureEditHasUnsavedChanges(bool newValue);

    void valueTreeChildAdded(juce::ValueTree &parent,
                             juce::ValueTree &child) override;

    void valueTreeChildRemoved(juce::ValueTree &parent,
                               juce::ValueTree &child,
                               int) override;

    void valueTreePropertyChanged(juce::ValueTree &vt,
                                  const juce::Identifier &property) override;

  private:
    juce::ValueTree state;
    juce::ValueTree statusVt;
    void saveStateToFile(const juce::File &file) const;
    void loadStateFromFile(const juce::File &file);
    bool loadedStateIsValid(const juce::ValueTree &vt);
};
