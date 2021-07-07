#pragma once

#include "TracktionThumbnail.h"

#include <memory>

namespace te = tracktion_engine;

struct PositionableThumbnail {
    PositionableThumbnail(te::TransportControl &tc,
                          te::AudioFile file,
                          double editLength,
                          double clipStart,
                          double clipEnd,
                          double offset,
                          int trackIndex);

    TracktionThumbnail thumbnail;

    float getTop(float trackHeight);
    float getBottom(float trackHeight);
    float getStart(int containerWidth);
    float getEnd(int containerWidth);

  private:
    int trackIndex;
    float normalisedStart;
    float normalisedEnd;
};

class Arrangement : public juce::Component {
  public:
    Arrangement(te::Edit &e,
                te::TransportControl &tc,
                float initialTrackHeight);
    ~Arrangement();

    void paint(juce::Graphics &) override;
    void resized() override;
    void prepare(int noOfTracksToMake);
    void clear();
    void
    addClip(juce::ReferenceCountedObjectPtr<tracktion_engine::WaveAudioClip>
                newClip,
            const int trackIndex,
            const double &clipStart,
            const double &clipEnd,
            const double &offset);
    void setTrackHeight(float newHeight);

  private:
    te::Edit &edit;
    te::TransportControl &transport;
    float trackHeight;
    int noOfTracks;
    std::vector<std::unique_ptr<PositionableThumbnail>> thumbnails;

    void drawTrackDividers(juce::Graphics &g);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Arrangement)
};
