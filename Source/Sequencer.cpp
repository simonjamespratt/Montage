/*
  ==============================================================================

    Sequencer.cpp
    Created: 13 Mar 2019 7:34:49pm
    Author:  Simon Pratt

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "Sequencer.h"

//==============================================================================
Sequencer::Sequencer(te::Engine &eng) : engine(eng),
                                        edit(engine, te::createEmptyEdit(), te::Edit::forEditing, nullptr, 0),
                                        transport(edit.getTransport()),
                                        timeline(edit),
                                        arrangement(edit, transport),
                                        cursor(transport, edit),
                                        transportInteractor(transport, edit),
                                        transportController(transport)
{
    // TODO: remove this
    addAndMakeVisible(&loadFileButton);
    loadFileButton.setButtonText("Load file");
    loadFileButton.onClick = [this] { selectAudioFile(); };

    addAndMakeVisible(&timeline);
    addAndMakeVisible(&arrangement);
    addAndMakeVisible(&cursor);
    addAndMakeVisible(&transportInteractor);
    addAndMakeVisible(&transportController);
}

Sequencer::~Sequencer()
{
    edit.getTempDirectory(false).deleteRecursively();
}

void Sequencer::paint(Graphics &g)
{
}

void Sequencer::resized()
{
    // TODO: remove the loadFileButton
    // loadFileButton.setBounds(10, 10, getWidth() - 20, 20);

    auto area = getLocalBounds();
    area.removeFromTop(10);
    area.removeFromRight(10);
    area.removeFromBottom(10);
    area.removeFromLeft(10);
    auto transportArea = area.removeFromBottom(50);
    auto timelineArea = area.removeFromTop(20);
    auto arrangementArea = area;

    timeline.setBounds(timelineArea);
    arrangement.setBounds(arrangementArea);
    cursor.setBounds(arrangementArea);
    transportInteractor.setBounds(arrangementArea);
    transportController.setBounds(transportArea);
}

void Sequencer::selectAudioFile()
{
    // ======================================================================================
    // TODO: selectAudioFile(): remove this file selection logic
    // Replace it with logic for loading and validating an existing source file from appState as it is done in ParticleSelector
    auto fileChooser = std::make_shared<FileChooser>(
        "Load an audio file",
        engine.getPropertyStorage().getDefaultLoadSaveDirectory(ProjectInfo::projectName),
        "*.wav,*.aif,*.aiff");

    if (fileChooser->browseForFileToOpen())
    {
        auto file = fileChooser->getResult();

        if (file.existsAsFile())
        {
            engine.getPropertyStorage().setDefaultLoadSaveDirectory(
                ProjectInfo::projectName,
                file.getParentDirectory());
        }

        te::AudioFile audioFile(file);

        if (!audioFile.isValid())
        {
            return;
        }
        // =======================================================================================
        // TODO: Move adding clip to track and transport reset to other methods that handle creating a figure
        arrangement.addClipToTrack(file, 1, 1.0, 3.5, 0.25);

        timeline.recalculate();
        transport.position = 0.0;
        transport.play(false);
    }
}
