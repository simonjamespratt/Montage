#pragma once
#include <tracktion_engine/tracktion_engine.h>

namespace te = tracktion_engine;

class TracktionThumbnail : public juce::Component {
  public:
    TracktionThumbnail(te::TransportControl &tc);

    void paint(juce::Graphics &) override;
    void setFile(const tracktion_engine::AudioFile &file,
                 const double &offset,
                 const double &clipLength);

  private:
    te::TransportControl &transport;
    te::SmartThumbnail smartThumbnail;

    double timeRangeStart;
    double timeRangeEnd;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TracktionThumbnail)
};

class ClipView : public juce::Component {
  public:
    ClipView(te::Clip::Ptr c);

    te::Clip &getClip()
    {
        return *clip;
    }

  protected:
    te::Clip::Ptr clip;
};

class AudioClipView : public ClipView {
  public:
    AudioClipView(te::Clip::Ptr c);

    void paint(juce::Graphics &g) override;

    te::WaveAudioClip *getWaveAudioClip()
    {
        return dynamic_cast<te::WaveAudioClip *>(clip.get());
    }

  private:
    void updateThumbnail();
    void drawWaveform(juce::Graphics &g,
                      te::AudioClipBase &c,
                      te::SmartThumbnail &thumb,
                      juce::Colour colour,
                      int left,
                      int right,
                      int y,
                      int h,
                      int xOffset);

    std::unique_ptr<te::SmartThumbnail> thumbnail;
};
