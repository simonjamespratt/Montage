#include "Sequencer.h"

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
  arrangement(edit, transport),
  cursor(transport, edit),
  transportInteractor(transport, edit),
  transportController(transport)
{
    noOfTracks = 0;

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

void Sequencer::resized()
{
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
