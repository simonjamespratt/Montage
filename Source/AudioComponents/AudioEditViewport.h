#pragma once

#include <functional>
#include <tracktion_engine/tracktion_engine.h>

class AudioEditViewport : public juce::Viewport {
  public:
    // NB: Provides solution for keeping two viewports aligned as per
    // https://forum.juce.com/t/how-to-fix-a-region-in-viewport/36095/11
    void
    visibleAreaChanged(const juce::Rectangle<int> &newVisibleArea) override;

    std::function<void(const juce::Rectangle<int> &newVisibleArea)>
        onVisibleAreaChanged;

    void syncToTransportPositionWhenPlaying(double transportPosition,
                                            double editLength);
    void syncToTransportPositionOnResize(double transportPosition,
                                         double editLength);
    void syncToMouseDrag(const juce::MouseEvent event);

  private:
    double
    getTransportPositionAsPointInViewedComponent(double transportPosition,
                                                 double editLength);
};
