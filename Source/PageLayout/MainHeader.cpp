#include "MainHeader.h"

#include "DecisionModal.h"
#include "ErrorMessageModal.h"

MainHeader::MainHeader(te::Engine &e, const ProjectState &ps)
: engine(e), state(ps)
{
    state.onStatusChanged = [this](auto status, auto action) {
        refreshView(status);
    };

    settingsButton.setButtonText("Settings");
    settingsButton.onClick = [this] {
        showAudioDeviceSettings(engine);
    };
    addAndMakeVisible(&settingsButton);

    saveButton.setButtonText("Save");
    saveButton.onClick = [this] {
        try {
            state.save();
        } catch(const std::exception &e) {
            std::make_shared<ErrorMessageModal>(juce::String(e.what()));
            return;
        }
    };
    addChildComponent(saveButton);

    loadButton.setButtonText("Load project");
    loadButton.onClick = [this] {
        if(state.getStatus().hasUnsavedChanges) {
            std::make_shared<DecisionModal>(
                "The project has unsaved changes. Loading a new project will "
                "mean these changes will be lost. Do you wish to proceed?",
                [this](bool decision) {
                    if(decision) {
                        load();
                    }
                });
            return;
        }
        load();
    };
    addAndMakeVisible(loadButton);

    createButton.setButtonText("New project");
    createButton.onClick = [this] {
        if(state.getStatus().hasUnsavedChanges) {
            std::make_shared<DecisionModal>(
                "The project has unsaved changes. Creating a new project will "
                "mean these changes will be lost. Do you wish to proceed?",
                [this](bool decision) {
                    if(decision) {
                        create();
                    }
                });
            return;
        }
        create();
    };
    addAndMakeVisible(createButton);

    refreshView(state.getStatus());
}

MainHeader::~MainHeader()
{}

void MainHeader::resized()
{
    auto margin = 10;
    auto area = getLocalBounds();
    juce::FlexBox fb;
    fb.justifyContent = juce::FlexBox::JustifyContent::flexEnd;
    fb.alignContent = juce::FlexBox::AlignContent::flexStart;

    if(saveButton.isVisible()) {
        fb.items.add(juce::FlexItem(saveButton)
                         .withHeight(20.0f)
                         .withWidth(100.0f)
                         .withMargin(juce::FlexItem::Margin(0, 10, 0, 0)));
    }

    fb.items.add(juce::FlexItem(loadButton)
                     .withHeight(20.0f)
                     .withWidth(100.0f)
                     .withMargin(juce::FlexItem::Margin(0, 10, 0, 0)));

    fb.items.add(juce::FlexItem(createButton)
                     .withHeight(20.0f)
                     .withWidth(100.0f)
                     .withMargin(juce::FlexItem::Margin(0, 10, 0, 0)));

    fb.items.add(
        juce::FlexItem(settingsButton).withHeight(20.0f).withWidth(100.0f));
    fb.performLayout(area.reduced(margin).toFloat());
}

// Private methods
void MainHeader::showAudioDeviceSettings(te::Engine &engine)
{
    juce::DialogWindow::LaunchOptions o;
    o.dialogTitle = TRANS("Audio Settings");
    o.dialogBackgroundColour =
        juce::LookAndFeel::getDefaultLookAndFeel().findColour(
            juce::ResizableWindow::backgroundColourId);
    o.content.setOwned(new juce::AudioDeviceSelectorComponent(
        engine.getDeviceManager().deviceManager,
        0,
        512,
        1,
        512,
        false,
        false,
        true,
        true));
    o.content->setSize(400, 600);
    o.launchAsync();
}

void MainHeader::refreshView(ProjectState::Status status)
{
    saveButton.setVisible(status.hasUnsavedChanges &&
                          status.hasProjectDirectory);
    resized();
}

void MainHeader::load()
{
    juce::FileChooser fc(
        "Load a project",
        juce::File::getSpecialLocation(juce::File::userHomeDirectory));

    if(fc.browseForDirectory()) {
        auto selectedDirectory = fc.getResult();
        jassert(selectedDirectory.isDirectory());

        try {
            state.load(selectedDirectory);
        } catch(const std::exception &e) {
            std::make_shared<ErrorMessageModal>(juce::String(e.what()));
            return;
        }
    }
}

void MainHeader::create()
{
    juce::FileChooser fc(
        "Create a new project",
        juce::File::getSpecialLocation(juce::File::userHomeDirectory));

    if(fc.browseForDirectory()) {
        auto selectedDirectory = fc.getResult();
        jassert(selectedDirectory.isDirectory());

        try {
            state.create(selectedDirectory);
        } catch(const std::exception &e) {
            std::make_shared<ErrorMessageModal>(juce::String(e.what()));
            return;
        }
    }
}
