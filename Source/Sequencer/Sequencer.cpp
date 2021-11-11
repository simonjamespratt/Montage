#include "Sequencer.h"

#include "AudioRenderer.h"
#include "ErrorMessageModal.h"

Sequencer::Sequencer(te::Engine &eng)
: engine(eng),
  edit(engine,
       /* TODO: TRACKTION: method signature for te::createEmptyEdit() is a
          legacy signature. Update to newer version. See
          tracktion_EditFileOperations.h */
       te::createEmptyEdit(engine),
       te::Edit::forEditing,
       nullptr,
       0),
  transport(edit.getTransport()),
  transportManager(edit, cursor),
  timeline(edit),
  timeScalingFactor {100, 100, 0, 1000},
  trackHeight(75),
  arrangement(edit, transport, trackHeight),
  trackControlPanel(trackHeight),
  transportInteractor(transport, edit),
  transportController(transport),
  xZoom(juce::Slider::SliderStyle::LinearHorizontal,
        juce::Slider::TextEntryBoxPosition::NoTextBox),
  yZoom(juce::Slider::SliderStyle::LinearVertical,
        juce::Slider::TextEntryBoxPosition::NoTextBox),
  trackControlPanelWidth(0.1),
  trackControlPanelWidthAdjuster(juce::Slider::SliderStyle::LinearHorizontal,
                                 juce::Slider::TextEntryBoxPosition::NoTextBox)
{
    transportManager.onChange = [this] {
        arrangementContainerViewport.syncToTransportPositionWhenPlaying(
            transport.getCurrentPosition(),
            edit.getLength());
    };
    transportManager.startManager();

    transportController.onTransportStopped = [this] {
        arrangementContainerViewport.syncToTransportPositionWhenPlaying(
            transport.getCurrentPosition(),
            edit.getLength());
    };

    transportInteractor.onSelectionChangeInProgress =
        [this](const juce::MouseEvent event) {
            arrangementContainerViewport.syncToMouseDrag(event);
        };

    addAndMakeVisible(&transportController);

    timelineViewport.setViewedComponent(&timeline, false);
    addAndMakeVisible(&timelineViewport);
    timelineViewport.setScrollBarsShown(false, false, false, true);
    timelineViewport.onVisibleAreaChanged =
        [this](const juce::Rectangle<int> &newVisibleArea) {
            arrangementContainerViewport.setViewPosition(
                newVisibleArea.getX(),
                arrangementContainerViewport.getViewPositionY());
        };

    arrangementContainerViewport.setViewedComponent(&arrangementContainer,
                                                    false);
    addAndMakeVisible(&arrangementContainerViewport);
    arrangementContainerViewport.setScrollBarsShown(true, true);
    arrangementContainerViewport.onVisibleAreaChanged =
        [this](const juce::Rectangle<int> &newVisibleArea) {
            timelineViewport.setViewPosition(newVisibleArea.getX(),
                                             newVisibleArea.getY());

            trackControlPanelViewPort.setViewPosition(newVisibleArea.getX(),
                                                      newVisibleArea.getY());
        };

    arrangementContainer.addAndMakeVisible(&arrangement);
    arrangementContainer.addAndMakeVisible(&cursor);
    arrangementContainer.addAndMakeVisible(&transportInteractor);

    trackControlPanelViewPort.setViewedComponent(&trackControlPanel, false);
    addAndMakeVisible(&trackControlPanelViewPort);
    trackControlPanelViewPort.setScrollBarsShown(false, false, true, false);
    trackControlPanelViewPort.onVisibleAreaChanged =
        [this](const juce::Rectangle<int> &newVisibleArea) {
            arrangementContainerViewport.setViewPosition(
                arrangementContainerViewport.getViewPositionX(),
                newVisibleArea.getY());
        };

    xZoom.setRange(timeScalingFactor.min, timeScalingFactor.max);
    xZoom.setValue(timeScalingFactor.current);
    xZoom.onValueChange = [this] {
        timeScalingFactor.current = xZoom.getValue();
        resized();

        // NB: do this AFTER resized() where component sizes are set to correct
        // new values
        arrangementContainerViewport.syncToTransportPositionOnResize(
            transport.getCurrentPosition(),
            edit.getLength());
    };
    addAndMakeVisible(xZoom);

    yZoom.setRange(20, 300);
    yZoom.setValue(trackHeight);
    yZoom.onValueChange = [this] {
        trackHeight = yZoom.getValue();
        arrangement.setTrackHeight(trackHeight);
        trackControlPanel.setTrackHeight(trackHeight);
        resized();
    };
    addAndMakeVisible(yZoom);

    trackControlPanelWidthAdjuster.setRange(0.05, 0.4);
    trackControlPanelWidthAdjuster.setValue(trackControlPanelWidth);
    trackControlPanelWidthAdjuster.onValueChange = [this] {
        trackControlPanelWidth = trackControlPanelWidthAdjuster.getValue();
        resized();
    };
    addAndMakeVisible(trackControlPanelWidthAdjuster);

    renderButton.setButtonText("Render");
    renderButton.onClick = [this] {
        if(currentFigure == nullptr) {
            std::make_shared<ErrorMessageModal>(
                "Cannot render because no figure is currently selected.");
            return;
        }

        AudioRenderer::renderFigureToFile(edit, currentFigure->getName());
    };
    addAndMakeVisible(renderButton);
}

Sequencer::~Sequencer()
{
    edit.getTempDirectory(false).deleteRecursively();
}

void Sequencer::resized()
{
    auto area = getLocalBounds();

    auto controlsArea = area.removeFromBottom(50);
    auto transportArea = controlsArea.reduced(200, 0);
    auto zoomControlsArea = controlsArea.removeFromRight(200);
    auto tcpWidthAdjusterArea = controlsArea.removeFromLeft(100);
    auto renderArea = controlsArea.removeFromRight(100);

    auto trackControlPanelArea =
        area.removeFromLeft(area.getWidth() * trackControlPanelWidth);
    trackControlPanelArea.removeFromTop(25); // timeline height removed

    auto timelineViewportArea = area.removeFromTop(25);
    auto containerViewportArea = area;

    if(edit.getLength() > 0) {
        timeScalingFactor.min =
            containerViewportArea.getWidth() / edit.getLength();
        xZoom.setRange(timeScalingFactor.min, timeScalingFactor.max);
    }

    auto editWidth = edit.getLength() * timeScalingFactor.current;
    auto totalTrackHeight =
        (te::getAudioTracks(edit).size() * trackHeight) +
        1; // add 1 to avoid bottom divider in arrangement being cut off
    auto bottomMargin = 8; // avoids vertical scrollbar when arrangement is
                           // shorter than viewport
    auto arrangementHeight =
        totalTrackHeight > containerViewportArea.getHeight()
            ? totalTrackHeight
            : containerViewportArea.getHeight() - bottomMargin;

    timelineViewport.setBounds(timelineViewportArea);
    arrangementContainerViewport.setBounds(containerViewportArea);

    timeline.setSize(editWidth, timelineViewportArea.getHeight());
    arrangementContainer.setSize(editWidth, arrangementHeight);

    auto arrangementArea = arrangementContainer.getBounds();
    cursor.setSize(arrangementArea.getWidth(), arrangementArea.getHeight());
    transportInteractor.setSize(arrangementArea.getWidth(),
                                arrangementArea.getHeight());
    arrangement.setSize(editWidth, totalTrackHeight);

    trackControlPanelArea.removeFromBottom(bottomMargin);
    trackControlPanelViewPort.setBounds(
        trackControlPanelArea); // this will become area of viewport
    trackControlPanel.setSize(trackControlPanelArea.getWidth(),
                              totalTrackHeight);

    transportController.setBounds(transportArea);
    xZoom.setBounds(
        zoomControlsArea.removeFromLeft(zoomControlsArea.getWidth() / 2));
    yZoom.setBounds(zoomControlsArea);

    trackControlPanelWidthAdjuster.setBounds(tcpWidthAdjusterArea);

    renderButton.setBounds(renderArea.reduced(10));
}

void Sequencer::readFigure(const Figure &figure,
                           const ProjectState &projectState)
{
    clear();

    currentFigure = std::make_unique<Figure>(figure);

    std::vector<ClipData> clips;

    auto eventList = projectState.getEventList(figure);
    auto particleList = projectState.getParticleList();

    prepareForNewFigure(particleList);

    for(auto &event : eventList.getObjects()) {
        auto particle = event.getParticle();

        int trackIndex = particleList.getIndex(particle);
        jassert(trackIndex != -1);
        double clipStart = event.getOnset() * 0.001; // convert from ms to s
        double clipEnd = (particle.getEnd() - particle.getStart()) + clipStart;
        double offset = particle.getStart();
        auto file = particle.getSource().getFile();

        auto clip =
            addClipToTrack(file, trackIndex, clipStart, clipEnd, offset);
        clips.push_back({clip, trackIndex, clipStart, clipEnd, offset});
    }

    // do this after we know the edit length but before adding clips to the
    // arrangement as arrangement needs to be resized first which this will do
    resized();

    for(auto &&entry : clips) {
        arrangement.addClip(entry.clip,
                            entry.trackIndex,
                            entry.clipStart,
                            entry.clipEnd,
                            entry.offset);
    }

    timeline.recalculate();
    transportController.grabKeyboardFocus();
}

void Sequencer::clear()
{
    currentFigure = nullptr;
    transport.stop(false, false);
    transport.position = 0.0;
    transport.setLoopRange({});
    transportInteractor.clearSelectionRange();
    clearTracks();
    arrangement.clear();
    trackControlPanel.clear();
    timeScalingFactor.current = timeScalingFactor.initial;
    resized();
}

// Private methods
void Sequencer::prepareForNewFigure(ParticleList particleList)
{
    auto noOfTracks = particleList.getObjects().size();
    prepareTracks(noOfTracks);
    arrangement.prepare(noOfTracks);
    trackControlPanel.createPanels(particleList);
}

void Sequencer::clearTracks()
{
    // remove all clips from each track and delete the track
    auto tracks = te::getAudioTracks(edit);
    for(auto &track : tracks) {
        auto clipsToRemove = track->getClips();
        for(int i = clipsToRemove.size(); --i >= 0;) {
            clipsToRemove.getUnchecked(i)->removeFromParentTrack();
        }
        edit.deleteTrack(track);
    }
}

void Sequencer::prepareTracks(int noOfTracks)
{
    edit.ensureNumberOfAudioTracks(noOfTracks);
    repaint();
}

juce::ReferenceCountedObjectPtr<tracktion_engine::WaveAudioClip>
Sequencer::addClipToTrack(const juce::File &file,
                          const int trackIndex,
                          const double &clipStart,
                          const double &clipEnd,
                          const double &offset)
{
    /*
        NB: ClipPosition has the following structure: { {startClip, endClip},
       offset } Where the above mean:
        - startClip: the start position of the clip as placed on the track,
       e.g. 1.0 would mean the clip starts at 1 second from the beginning of the
       transport start
        - endClip: the end of the clip; the difference between clipEnd and
       clipStart gives you the length of the clip
        - offset: the start of the clip in relation to the start of the audio
       file, e.g. 1.0 would mean the clip start is 1 second from the beginning
       of the audio file

        Mapping figure and particle value trees to a ClipPosition:
        - particle.rangeStart -> ClipPosition.offset
        - figure.figureEvent.onset -> ClipPosition.clipStart
        - (particle.rangeEnd - particle.rangeStart) + ClipPosition.clipStart ->
       ClipPosition.clipEnd
    */

    edit.ensureNumberOfAudioTracks(trackIndex + 1);
    auto track = te::getAudioTracks(edit)[trackIndex];

    auto newClip = track->insertWaveClip(file.getFileNameWithoutExtension(),
                                         file,
                                         {{clipStart, clipEnd}, offset},
                                         false);

    return newClip;
}
