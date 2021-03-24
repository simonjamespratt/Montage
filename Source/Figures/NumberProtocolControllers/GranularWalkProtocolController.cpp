#include "GranularWalkProtocolController.h"

GranularWalkProtocolController::GranularWalkProtocolController(
    aleatoric::NumberProtocolParams initialParams)
: devFactorEditor(deviationFactor, "Deviation factor", 0, 1.0, "", 2, 50)
{
    setParams(initialParams);
    devFactorEditor.onChange = [this] {
        notifyParamsChanged();
    };
    addAndMakeVisible(&devFactorEditor);
}

void GranularWalkProtocolController::resized()
{
    auto area = getLocalBounds();
    devFactorEditor.setBounds(area.removeFromTop(45));
}

void GranularWalkProtocolController::setParams(
    aleatoric::NumberProtocolParams params)
{
    jassert(params.getActiveProtocol() ==
            aleatoric::NumberProtocol::Type::granularWalk);

    auto granularWalkParams = params.getGranularWalk();
    deviationFactor = granularWalkParams.getDeviationFactor();
    devFactorEditor.updateValue();
}

aleatoric::NumberProtocolParams GranularWalkProtocolController::getParams()
{
    return aleatoric::NumberProtocolParams(
        aleatoric::GranularWalkParams(deviationFactor));
}
