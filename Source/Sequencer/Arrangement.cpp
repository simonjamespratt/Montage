#include "Arrangement.h"

#include "Helpers.h"

ArrangementTrack::ArrangementTrack(te::AudioTrack::Ptr t) : track(t)
{
    track->state.addListener(this);
    markAndUpdate(updateClips);
    markAndUpdate(updateAutoParamGraph);
    setInterceptsMouseClicks(false, true); // allow events through to interactor
}

ArrangementTrack::~ArrangementTrack()
{
    track->state.removeListener(this);
}

void ArrangementTrack::resized()
{
    for(auto cv : clipViews) {
        auto &clip = cv->getClip();
        auto clipPosition = clip.getPosition();
        auto editLength = track->edit.getLength();
        auto x = Helpers::getNormalised(clipPosition.getStart(), editLength) *
                 getWidth();
        auto y = 0;
        auto w = Helpers::getNormalised(clipPosition.getLength(), editLength) *
                 getWidth();
        auto h = getHeight();
        cv->setBounds(x, y, w, h);
    }

    if(autoParamGraph) {
        autoParamGraph->setBounds(getLocalBounds());
    }
}

void ArrangementTrack::paint(juce::Graphics &g)
{
    // draw track divider
    g.setColour(juce::Colours::cornflowerblue);
    g.fillRect(0.0, (getHeight() - 0.5), float(getWidth()), 0.5f);
}

// Private methods
void ArrangementTrack::valueTreeChildAdded(juce::ValueTree &p,
                                           juce::ValueTree &c)
{
    if(te::Clip::isClipState(c)) {
        markAndUpdate(updateClips);
    }
}

void ArrangementTrack::valueTreeChildRemoved(juce::ValueTree &p,
                                             juce::ValueTree &c,
                                             int)
{
    if(te::Clip::isClipState(c)) {
        markAndUpdate(updateClips);
    }
}

void ArrangementTrack::valueTreePropertyChanged(juce::ValueTree &tree,
                                                const juce::Identifier &prop)
{
    if(prop == te::IDs::currentAutoParamTag) {
        markAndUpdate(updateAutoParamGraph);
    }
}

void ArrangementTrack::handleAsyncUpdate()
{
    if(compareAndReset(updateClips)) {
        createThumbnails();
    }

    if(compareAndReset(updateAutoParamGraph)) {
        auto autoParam = track->getCurrentlyShownAutoParam();
        if(autoParam) {
            autoParamGraph = std::make_unique<AutoParamGraph>(autoParam);
            addAndMakeVisible(*autoParamGraph);
        } else {
            autoParamGraph = nullptr;
        }

        resized();
    }
}

void ArrangementTrack::createThumbnails()
{
    clipViews.clear();

    for(auto clip : track->getClips()) {
        if(dynamic_cast<te::WaveAudioClip *>(clip)) {
            auto thumb = new AudioClipView(clip);
            clipViews.add(thumb);
            addAndMakeVisible(thumb);
            thumb->setInterceptsMouseClicks(
                false,
                false); // allow events through to interactor
        }
    }

    resized();
}

// ==========================================================

Arrangement::Arrangement(te::Edit &e, SequencerViewState &vs)
: edit(e), sequencerViewState(vs), cursor(edit), transportInteractor(edit)
{
    edit.state.addListener(this);
    sequencerViewState.state.addListener(this);
    triggerAsyncUpdate();

    transportInteractor.onSelectionChangeInProgress =
        [this](const juce::MouseEvent) {
            sequencerViewState.viewportSyncToMouseRequired = true;
        };

    addAndMakeVisible(transportInteractor);
    addAndMakeVisible(cursor);
}

Arrangement::~Arrangement()
{
    edit.state.removeListener(this);
    sequencerViewState.state.removeListener(this);
}

void Arrangement::resized()
{
    auto area = getLocalBounds();

    transportInteractor.setBounds(area);
    cursor.setBounds(area);

    for(auto track : tracks) {
        track->setBounds(area.removeFromTop(sequencerViewState.trackHeight));
    }
}

// Private methods
void Arrangement::valueTreePropertyChanged(juce::ValueTree &,
                                           const juce::Identifier &prop)
{
    if(prop == IDs::trackHeight) {
        resized();
    }
}

void Arrangement::valueTreeChildAdded(juce::ValueTree &parent,
                                      juce::ValueTree &child)
{
    if(te::TrackList::isTrack(child)) {
        triggerAsyncUpdate();
    }
}

void Arrangement::valueTreeChildRemoved(juce::ValueTree &parent,
                                        juce::ValueTree &child,
                                        int)
{
    if(te::TrackList::isTrack(child)) {
        triggerAsyncUpdate();
    }
}

void Arrangement::handleAsyncUpdate()
{
    createArrangementTracks();
}

void Arrangement::createArrangementTracks()
{
    tracks.clear();

    for(auto t : te::getAudioTracks(edit)) {
        ArrangementTrack *at = new ArrangementTrack(t);
        tracks.add(at);
        addAndMakeVisible(at);
    }

    cursor.toFront(false);
    transportInteractor.toBack(); // ensure it's behind the AutoParamGraphs in
                                  // ArrangementTracks

    resized();
}
