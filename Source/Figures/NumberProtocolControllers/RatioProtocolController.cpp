#include "RatioProtocolController.h"

RatioProtocolController::RatioProtocolController(
    aleatoric::NumberProtocolParams initialParams)
: ratiosEditor(ratios, true)
{
    setParams(initialParams);
    ratiosEditor.onChange = [this] {
        notifyParamsChanged();
    };
    ratiosEditorViewport.setViewedComponent(&ratiosEditor, false);
    ratiosEditorViewport.setScrollBarsShown(true, false);
    addAndMakeVisible(&ratiosEditorViewport);
}

void RatioProtocolController::resized()
{
    auto area = getLocalBounds();
    ratiosEditorViewport.setBounds(area);
    ratiosEditor.setBounds(area);
}

void RatioProtocolController::setParams(aleatoric::NumberProtocolParams params)
{
    jassert(params.getActiveProtocol() ==
            aleatoric::NumberProtocol::Type::ratio);

    ratios = params.getRatio().getRatios();
    ratiosEditor.redraw();
}

aleatoric::NumberProtocolParams RatioProtocolController::getParams()
{
    // TODO: delete this
    for(auto &value : ratios) {
        DBG("value in ratios: " << value);
    }

    return aleatoric::NumberProtocolParams(aleatoric::RatioParams(ratios));
}
