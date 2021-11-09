#pragma once
#include "AudioEditViewport.h"
#include "Cursor.h"
#include "DoubleValueEditorWithLabel.h"
#include "Particle.h"
#include "StringEditorWithLabel.h"
#include "TimeScalingFactor.h"
#include "TracktionThumbnail.h"
#include "TransportController.h"
#include "TransportInteractor.h"
#include "TransportManager.h"

#include <functional>
#include <juce_gui_basics/juce_gui_basics.h>

namespace te = tracktion_engine;

class ParticleEditor : public juce::Component {
  public:
    ParticleEditor(const Particle &p, te::Engine &eng);
    void resized() override;
    void paint(juce::Graphics &g) override;
    void setFocus();
    std::function<void()> onDoneEditing;

  private:
    struct Model {
        double start;
        double end;
        juce::String name;
    };

    Particle particle;
    Model model;

    DoubleValueEditorWithLabel startEditor;
    DoubleValueEditorWithLabel endEditor;
    StringEditorWithLabel nameEditor;

    te::Edit edit;
    te::TransportControl &transport;
    TransportManager transportManager;

    TimeScalingFactor timeScalingFactor;
    AudioEditViewport thumbnailViewport;
    juce::Component thumbnailContainer;
    TracktionThumbnail thumbnail;
    Cursor cursor;
    TransportInteractor transportInteractor;

    juce::Slider xZoom;

    TransportController transportController;

    juce::TextButton saveButton;
    juce::TextButton cancelButton;

    void prepAudio(te::Engine &eng);
};
