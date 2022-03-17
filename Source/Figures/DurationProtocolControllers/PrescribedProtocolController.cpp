#include "PrescribedProtocolController.h"

PrescribedProtocolController::PrescribedProtocolController(
    DurationProtocolParams &p)
: params(p), durationsEditor(params.prescribed.durations)
{
    viewport.setViewedComponent(&durationsEditor, false);
    viewport.setScrollBarsShown(true, false);
    addAndMakeVisible(&viewport);
}

void PrescribedProtocolController::resized()
{
    auto margin = 10;
    auto area = getLocalBounds();

    auto viewsArea = area.removeFromLeft(250);
    viewport.setBounds(viewsArea);
    durationsEditor.setBounds(viewsArea);
}
