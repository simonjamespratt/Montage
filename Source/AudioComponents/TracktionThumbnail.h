#pragma once
#include <tracktion_engine/tracktion_engine.h>

class TracktionThumbnail : public juce::Component {
  public:
    TracktionThumbnail(tracktion_engine::TransportControl &tc);
    ~TracktionThumbnail();

    void paint(juce::Graphics &) override;
    void setFile(const tracktion_engine::AudioFile &file,
                 const double &offset,
                 const double &clipLength);

  private:
    tracktion_engine::TransportControl &transport;
    tracktion_engine::SmartThumbnail smartThumbnail;

    double timeRangeStart;
    double timeRangeEnd;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TracktionThumbnail)
};
