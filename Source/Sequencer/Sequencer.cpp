#include "Sequencer.h"

Sequencer::Sequencer(std::unique_ptr<te::Edit> e, Figure f, ProjectState &ps)
: edit(std::move(e)),
  projectState(ps),
  sequencerViewState(*edit, f),
  transport(edit->getTransport()),
  transportManager(*edit),
  timeline(*edit),
  arrangement(*edit, sequencerViewState),
  panels(*edit),
  controlsView(sequencerViewState, *edit, ps)
{
    sequencerViewState.state.addListener(this);

    edit->state.addListener(this);

    projectState.onStatusChanged = [this](auto status, auto action) {
        if(action == ProjectState::Action::SaveProject) {
            save();
        }
    };

    transportManager.onChange = [this] {
        arrangementViewport.syncToTransportPositionWhenPlaying(
            transport.getCurrentPosition(),
            edit->getLength());
    };
    transportManager.startManager();

    timelineViewport.setViewedComponent(&timeline, false);
    addAndMakeVisible(&timelineViewport);
    timelineViewport.setScrollBarsShown(false, false, false, true);
    timelineViewport.onVisibleAreaChanged =
        [this](const juce::Rectangle<int> &newVisibleArea) {
            arrangementViewport.setViewPosition(
                newVisibleArea.getX(),
                arrangementViewport.getViewPositionY());
        };

    arrangementViewport.setViewedComponent(&arrangement, false);
    addAndMakeVisible(&arrangementViewport);
    arrangementViewport.setScrollBarsShown(true, true);
    arrangementViewport.onVisibleAreaChanged =
        [this](const juce::Rectangle<int> &newVisibleArea) {
            timelineViewport.setViewPosition(newVisibleArea.getX(),
                                             newVisibleArea.getY());

            panelsViewport.setViewPosition(newVisibleArea.getX(),
                                           newVisibleArea.getY());
        };

    panelsViewport.setViewedComponent(&panels, false);
    addAndMakeVisible(&panelsViewport);
    panelsViewport.setScrollBarsShown(false, false, true, false);
    panelsViewport.onVisibleAreaChanged =
        [this](const juce::Rectangle<int> &newVisibleArea) {
            arrangementViewport.setViewPosition(
                arrangementViewport.getViewPositionX(),
                newVisibleArea.getY());
        };

    addAndMakeVisible(controlsView);

    sequencerViewState.sequencerIsLoaded = true;
}

Sequencer::~Sequencer()
{
    transport.stop(false, false);
    edit->getTempDirectory(false).deleteRecursively();
    sequencerViewState.state.removeListener(this);
    edit->state.removeListener(this);
    jassert(te::EditFileOperations(*edit).save(true, false, true));
    projectState.setFigureEditHasUnsavedChanges(false);
}

void Sequencer::resized()
{
    auto bottomMargin = 8; // avoids vertical scrollbar when arrangement is
                           // shorter than viewport

    // areas
    auto area = getLocalBounds();
    auto controlsArea = area.removeFromBottom(50);
    auto panelsViewportArea =
        area.removeFromLeft(area.getWidth() * sequencerViewState.panelsWidth);
    panelsViewportArea.removeFromTop(25); // timeline height removed
    panelsViewportArea.removeFromBottom(bottomMargin);
    auto timelineViewportArea = area.removeFromTop(25);
    auto arrangementViewportArea = area;

    // calculations for edit length and no of tracks
    auto editLength = edit->getLength();
    auto editWidth = editLength * sequencerViewState.timeScalingFactorCurrent;
    auto totalTrackHeight =
        (te::getAudioTracks(*edit).size() * sequencerViewState.trackHeight) +
        1; // add 1 to avoid bottom divider in arrangement being cut off
    auto arrangementHeight =
        totalTrackHeight > arrangementViewportArea.getHeight()
            ? totalTrackHeight
            : arrangementViewportArea.getHeight() - bottomMargin;

    // NB: This is a bit wonky since transitioning to SequencerViewState
    // if(editLength > 0) {
    //     sequencerViewState.timeScalingFactorMin =
    //         arrangementViewportArea.getWidth() / editLength;
    //     xZoom.setRange(sequencerViewState.timeScalingFactorMin,
    //                    sequencerViewState.timeScalingFactorMax);
    // }

    timelineViewport.setBounds(timelineViewportArea);
    timeline.setSize(editWidth, timelineViewportArea.getHeight());

    arrangementViewport.setBounds(arrangementViewportArea);
    arrangement.setSize(editWidth, arrangementHeight);

    panelsViewport.setBounds(panelsViewportArea);
    panels.setSize(panelsViewportArea.getWidth(), totalTrackHeight);

    controlsView.setBounds(controlsArea);
}

// Static public methods
std::unique_ptr<te::Edit> Sequencer::createEdit(Figure figure,
                                                const ProjectState &ps,
                                                juce::File file,
                                                te::Engine &e)
{
    if(file.getSize() == 0) {
        auto edit = te::createEmptyEdit(e, file);

        auto eventList = ps.getEventList(figure);
        auto particleList = ps.getParticleList();

        // prep tracks
        auto particles = particleList.getObjects();
        edit->ensureNumberOfAudioTracks(particles.size());
        auto tracks = te::getAudioTracks(*edit);
        for(size_t i = 0; i < particles.size(); i++) {
            tracks[i]->setName(particles[i].getName());
        }

        // add events as clips
        for(auto &event : eventList.getObjects()) {
            auto particle = event.getParticle();

            int trackIndex = particleList.getIndex(particle);
            jassert(trackIndex != -1);
            double clipStart = event.getOnset() * 0.001; // convert from ms to s
            double clipEnd =
                (particle.getEnd() - particle.getStart()) + clipStart;
            double offset = particle.getStart();
            auto file = particle.getSource().getFile();

            /*
            NB: ClipPosition has the following structure: { {startClip,
           endClip}, offset } Where the above mean:
            - startClip: the start position of the clip as placed on the track,
           e.g. 1.0 would mean the clip starts at 1 second from the beginning of
           the transport start
            - endClip: the end of the clip; the difference between clipEnd and
           clipStart gives you the length of the clip
            - offset: the start of the clip in relation to the start of the
           audio file, e.g. 1.0 would mean the clip start is 1 second from the
           beginning of the audio file

            Mapping figure and particle value trees to a ClipPosition:
            - particle.rangeStart -> ClipPosition.offset
            - figure.figureEvent.onset -> ClipPosition.clipStart
            - (particle.rangeEnd - particle.rangeStart) + ClipPosition.clipStart
           -> ClipPosition.clipEnd
           */

            edit->ensureNumberOfAudioTracks(trackIndex + 1);
            auto track = te::getAudioTracks(*edit)[trackIndex];

            track->insertWaveClip(file.getFileNameWithoutExtension(),
                                  file,
                                  {{clipStart, clipEnd}, offset},
                                  false);
        }

        // this returns a boolean re. success/failure of save so could be used
        // to make decisions on failure if needed
        te::EditFileOperations(*edit).save(true, true, false);

        return edit;
    }

    return te::loadEditFromFile(e, file);
}

// Private methods
void Sequencer::valueTreePropertyChanged(juce::ValueTree &tree,
                                         const juce::Identifier &prop)
{
    if(prop == IDs::trackHeight || prop == IDs::panelsWidth ||
       prop == IDs::timeScalingFactorCurrent) {
        resized();

        // NB: do this AFTER resized() where component sizes are set to correct
        // new values
        if(prop == IDs::timeScalingFactorCurrent) {
            arrangementViewport.syncToTransportPositionOnResize(
                transport.getCurrentPosition(),
                edit->getLength());
        }
    }

    if(prop == IDs::viewportSyncToMouseRequired) {
        if(sequencerViewState.viewportSyncToMouseRequired) {
            auto mousePos = arrangement.getMouseXYRelative();
            arrangementViewport.syncToMouseDrag(mousePos);
            sequencerViewState.viewportSyncToMouseRequired = false;
        }
    }

    if(prop == IDs::viewportSyncToTransportRequired &&
       sequencerViewState.viewportSyncToTransportRequired) {
    }
    {
        arrangementViewport.syncToTransportPositionWhenPlaying(
            transport.getCurrentPosition(),
            edit->getLength());
        sequencerViewState.viewportSyncToTransportRequired = false;
    }

    if(prop == te::IDs::lastSignificantChange) {
        projectState.setFigureEditHasUnsavedChanges(true);
    }
}

void Sequencer::save()
{
    if(projectState.getStatus().figureEditHasUnsavedChanges) {
        jassert(te::EditFileOperations(*edit).save(true, false, false));
        projectState.setFigureEditHasUnsavedChanges(false);
    }
}
