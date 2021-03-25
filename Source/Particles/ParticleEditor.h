#pragma once
#include "Cursor.h"
#include "DoubleValueEditorWithLabel.h"
#include "Particle.h"
#include "TracktionThumbnail.h"
#include "TransportController.h"
#include "TransportInteractor.h"

#include <functional>
#include <juce_gui_basics/juce_gui_basics.h>

namespace te = tracktion_engine;

class ParticleEditor : public juce::Component {
  public:
    ParticleEditor(const Particle &p, te::Engine &eng);
    void resized() override;
    void paint(juce::Graphics &g) override;
    std::function<void()> onDoneEditing;

  private:
    struct Model {
        double start;
        double end;
    };

    Particle particle;
    Model model;

    juce::Label name;

    DoubleValueEditorWithLabel startEditor;
    DoubleValueEditorWithLabel endEditor;

    te::Edit edit;
    te::TransportControl &transport;
    TracktionThumbnail thumbnail;
    Cursor cursor;
    TransportController transportController;
    TransportInteractor transportInteractor;

    juce::TextButton saveButton;
    juce::TextButton cancelButton;

    void prepAudio(te::Engine &eng);
};
