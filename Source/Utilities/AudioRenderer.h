#pragma once

#include <tracktion_engine/tracktion_engine.h>

namespace te = tracktion_engine;

class AudioRenderer {
  public:
    /*
    Implementation based on the following juce forum threads:
    https://forum.juce.com/t/render-example/37979
    https://forum.juce.com/t/how-to-render-an-edit-to-a-new-file-audio-file/35136
    */
    static void renderFigureToFile(te::Edit &edit, juce::String filename);
};
