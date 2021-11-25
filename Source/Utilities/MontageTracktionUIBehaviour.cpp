#include "MontageTracktionUIBehaviour.h"

void MontageTracktionUIBehaviour::runTaskWithProgressBar(
    te::ThreadPoolJobWithProgress &t)
{
    TaskRunner runner(t);

    juce::AlertWindow window("Rendering", {}, juce::AlertWindow::NoIcon);
    double progress {0};
    window.addProgressBarComponent(progress);
    window.setVisible(true);

    while(runner.isThreadRunning()) {
        progress = t.getCurrentTaskProgress();
        if(!juce::MessageManager::getInstance()->runDispatchLoopUntil(10)) {
            break;
        }
    }
}

MontageTracktionUIBehaviour::TaskRunner::TaskRunner(
    te::ThreadPoolJobWithProgress &t)
: juce::Thread(t.getJobName()), task(t)
{
    startThread();
}

MontageTracktionUIBehaviour::TaskRunner::~TaskRunner()
{
    task.signalJobShouldExit();
    waitForThreadToExit(10000);
}

void MontageTracktionUIBehaviour::TaskRunner::run()
{
    while(!threadShouldExit()) {
        if(task.runJob() == juce::ThreadPoolJob::jobHasFinished) {
            break;
        }
    }
}
