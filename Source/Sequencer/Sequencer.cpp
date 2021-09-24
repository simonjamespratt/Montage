#include "Sequencer.h"

void ViewportWithCallback::visibleAreaChanged(
    const juce::Rectangle<int> &newVisibleArea)
{
    if(onVisibleAreaChanged) {
        onVisibleAreaChanged(newVisibleArea);
    }
}

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
  timeline(edit),
  noOfTracks(0),
  timeScalingFactor(100),
  trackHeight(75),
  arrangement(edit, transport, trackHeight),
  transportInteractor(transport, edit),
  transportController(transport),
  xZoom(juce::Slider::SliderStyle::LinearHorizontal,
        juce::Slider::TextEntryBoxPosition::NoTextBox),
  yZoom(juce::Slider::SliderStyle::LinearVertical,
        juce::Slider::TextEntryBoxPosition::NoTextBox)
{
    transportReporter.setCallback([this] {
        cursor.updatePosition(edit.getLength(), transport.getCurrentPosition());
        if(transport.isPlaying()) {
            auto editLength = edit.getLength();
            auto transportPosition = transport.getCurrentPosition();
            if(transportPosition >= editLength) {
                transport.stop(false, false);
                transport.setCurrentPosition(0);
            }
            syncViewportToTransportPosition();
        }
    });
    transportReporter.startTimerHz(25);

    transportController.onTransportStopped = [this] {
        syncViewportToTransportPosition();
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
        };

    arrangementContainer.addAndMakeVisible(&arrangement);
    arrangementContainer.addAndMakeVisible(&cursor);
    arrangementContainer.addAndMakeVisible(&transportInteractor);

    xZoom.setRange(100, 1000); // TODO: change min to lower value and update
                               // timeline rendering logic
    xZoom.setValue(100);
    xZoom.onValueChange = [this] {
        timeScalingFactor = xZoom.getValue();
        resized();

        // set the arrangement position within the viewport to be centered
        // around the current transport position NB: do this AFTER resized()
        // where component sizes are set to correct new values
        auto centreOfViewport = arrangementContainerViewport.getWidth() * 0.5;
        auto arrangementXOffset =
            getTransportPositionWithinComponent(arrangementContainer) -
            centreOfViewport;
        auto existingYOffset =
            arrangementContainerViewport.getViewArea().getY();

        arrangementContainerViewport.setViewPosition(arrangementXOffset,
                                                     existingYOffset);
    };
    addAndMakeVisible(xZoom);

    yZoom.setRange(20, 300);
    yZoom.setValue(trackHeight);
    yZoom.onValueChange = [this] {
        trackHeight = yZoom.getValue();
        arrangement.setTrackHeight(trackHeight);
        resized();
    };
    addAndMakeVisible(yZoom);
}

Sequencer::~Sequencer()
{
    edit.getTempDirectory(false).deleteRecursively();
    transportReporter.stopTimer();
}

void Sequencer::resized()
{
    auto area = getLocalBounds();
    auto controlsArea = area.removeFromBottom(50);
    auto transportArea = controlsArea.reduced(200, 0);
    auto zoomControlsArea = controlsArea.removeFromRight(200);
    auto timelineViewportArea = area.removeFromTop(25);
    auto containerViewportArea = area;
    auto editWidth = edit.getLength() * timeScalingFactor;
    auto totalTrackHeight = noOfTracks * trackHeight;
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

    transportController.setBounds(transportArea);
    xZoom.setBounds(
        zoomControlsArea.removeFromLeft(zoomControlsArea.getWidth() / 2));
    yZoom.setBounds(zoomControlsArea);
}

void Sequencer::readFigure(const Figure &figure,
                           const ProjectState &projectState)
{
    clear();

    std::vector<ClipData> clips;

    auto eventList = projectState.getEventList(figure);
    auto particleList = projectState.getParticleList();

    prepareForNewFigure(particleList.getObjects().size());

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
}

void Sequencer::clear()
{
    transport.stop(false, false);
    transport.position = 0.0;
    clearTracks();
    arrangement.clear();
    resized();
}

// Private methods
void Sequencer::prepareForNewFigure(int noOfParticles)
{
    // set the no of tracks required for the incoming figure
    noOfTracks = noOfParticles;
    // prepare the new tracks
    prepareTracks();
    arrangement.prepare(noOfTracks);

    // NB: find a way to delete audio tracks from edit: cannot see a good way to
    // this at the moment, which means there will be empty unused tracks in the
    // ether these unused tracks will not have clips on them because clips will
    // be cleared from all previously used tracks in this method they will also
    // not be displayed as display is the result of the noOfTracks member of
    // this class but they will still just be hanging around
}
void Sequencer::clearTracks()
{
    // for each track in use from the last figure, remove all clips
    auto tracks = te::getAudioTracks(edit);
    for(auto &&track : tracks) {
        auto clipsToRemove = track->getClips();
        for(int i = clipsToRemove.size(); --i >= 0;) {
            clipsToRemove.getUnchecked(i)->removeFromParentTrack();
        }
    }
}

void Sequencer::prepareTracks()
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

double
Sequencer::getTransportPositionWithinComponent(const juce::Component &component)
{
    auto normalisedTransportPosition =
        transport.getCurrentPosition() / edit.getLength();
    return normalisedTransportPosition * component.getWidth();
}

void Sequencer::syncViewportToTransportPosition()
{
    auto viewAreaOfArrangement = arrangementContainerViewport.getViewArea();
    auto horizontalRangeOfViewArea = viewAreaOfArrangement.getHorizontalRange();

    auto transportPositionAsPointInArrangement =
        getTransportPositionWithinComponent(arrangementContainer);

    if(transportPositionAsPointInArrangement >= 0 &&
       !horizontalRangeOfViewArea.contains(
           transportPositionAsPointInArrangement)) {
        auto existingYOffset = viewAreaOfArrangement.getY();
        auto newXOffset = transportPositionAsPointInArrangement;

        arrangementContainerViewport.setViewPosition(newXOffset,
                                                     existingYOffset);
    }
}
