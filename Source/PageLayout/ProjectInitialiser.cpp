#include "ProjectInitialiser.h"

#include "ErrorMessageModal.h"

ProjectInitialiser::ProjectInitialiser(ProjectState &ps) : projectState(ps)
{
    explanationText.setText("No project is currently loaded. Choose a project "
                            "to load or create a new one.",
                            juce::dontSendNotification);
    addAndMakeVisible(explanationText);

    createButton.setButtonText("New project");
    createButton.onClick = [this] {
        create();
    };
    addAndMakeVisible(createButton);

    loadButton.setButtonText("Load project");
    loadButton.onClick = [this] {
        if(PRE_POPULATE_DUMMY_DATA) {
            auto presetPath = juce::String(ASSETS_DIR) + "/presets/preset-1";
            juce::File presetDirectory(presetPath);
            projectState.load(presetDirectory);
            return;
        }
        load();
    };
    addAndMakeVisible(loadButton);
}

void ProjectInitialiser::resized()
{
    auto area = getLocalBounds();
    explanationText.setBounds(area.removeFromTop(100).reduced(20));
    auto ctaArea = area.reduced(100);
    createButton.setBounds(
        ctaArea.removeFromLeft(ctaArea.getWidth() / 2).reduced(30));
    loadButton.setBounds(ctaArea.reduced(30));
}

void ProjectInitialiser::load()
{
    juce::FileChooser fc(
        "Load a project",
        juce::File::getSpecialLocation(juce::File::userHomeDirectory));

    if(fc.browseForDirectory()) {
        auto selectedDirectory = fc.getResult();
        jassert(selectedDirectory.isDirectory());

        try {
            projectState.load(selectedDirectory);
        } catch(const std::exception &e) {
            std::make_shared<ErrorMessageModal>(juce::String(e.what()));
            return;
        }
    }
}

void ProjectInitialiser::create()
{
    juce::FileChooser fc(
        "Create a new project",
        juce::File::getSpecialLocation(juce::File::userHomeDirectory));

    if(fc.browseForDirectory()) {
        auto selectedDirectory = fc.getResult();
        jassert(selectedDirectory.isDirectory());

        try {
            projectState.create(selectedDirectory);
        } catch(const std::exception &e) {
            std::make_shared<ErrorMessageModal>(juce::String(e.what()));
            return;
        }
    }
}
