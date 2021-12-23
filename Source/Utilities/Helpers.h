#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

namespace Helpers {
static inline double getNormalised(double x, double xMax, double xMin = 0.0)
{
    jassert(xMax - xMin != 0);
    return (x - xMin) / (xMax - xMin);
}

static inline int timeToX(double time, double editLength, int width)
{
    auto normalisedTime =
        editLength == 0.0 ? 0.0 : getNormalised(time, editLength);
    return juce::roundToInt(normalisedTime * width);
}

static inline double xToTime(int x, double editLength, int width)
{
    auto normalisedX = x == 0 ? 0.0 : getNormalised(double(x), double(width));
    return normalisedX * editLength;
}

} // namespace Helpers

class FlaggedAsyncUpdater : public juce::AsyncUpdater {
  public:
    void markAndUpdate(bool &flag)
    {
        flag = true;
        triggerAsyncUpdate();
    }

    bool compareAndReset(bool &flag) noexcept
    {
        if(!flag)
            return false;

        flag = false;
        return true;
    }
};
