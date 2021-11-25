#pragma once

#include <tracktion_engine/tracktion_engine.h>

namespace te = tracktion_engine;

/*
Implementation based on the following juce forum threads:
https://forum.juce.com/t/how-to-render-an-edit-to-a-new-file-audio-file/35136/6
https://forum.juce.com/t/display-progress-bar-during-render/41947
https://forum.juce.com/t/problem-implementing-runtaskwithprogressbar-te-threadpooljobwithprogress-job/37405
https://github.com/Tracktion/tracktion_engine/blob/develop/examples/TestRunner.h
*/
class MontageTracktionUIBehaviour : public te::UIBehaviour {
  public:
    MontageTracktionUIBehaviour() = default;

    void runTaskWithProgressBar(te::ThreadPoolJobWithProgress &t) override;

  private:
    struct TaskRunner : public juce::Thread {
        TaskRunner(te::ThreadPoolJobWithProgress &t);
        ~TaskRunner() override;
        void run() override;

        te::ThreadPoolJobWithProgress &task;
    };
};
