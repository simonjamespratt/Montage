#include "PeriodicProtocolController.h"

PeriodicProtocolController::PeriodicProtocolController(
    aleatoric::NumberProtocolParams initialParams)
: chanceOfRepEditor(
      chanceOfRepetition, "Chance of repetition", 0, 1.0, "", 2, 50)
{
    setParams(initialParams);
    chanceOfRepEditor.onChange = [this] {
        notifyParamsChanged();
    };
    addAndMakeVisible(&chanceOfRepEditor);
}

void PeriodicProtocolController::resized()
{
    auto area = getLocalBounds();
    chanceOfRepEditor.setBounds(area.removeFromTop(45));
}

void PeriodicProtocolController::setParams(
    aleatoric::NumberProtocolParams params)
{
    jassert(params.getActiveProtocol() ==
            aleatoric::NumberProtocol::Type::periodic);

    chanceOfRepetition = params.getPeriodic().getChanceOfRepetition();
    chanceOfRepEditor.updateValue();
}

aleatoric::NumberProtocolParams PeriodicProtocolController::getParams()
{
    return aleatoric::NumberProtocolParams(
        aleatoric::PeriodicParams(chanceOfRepetition));
}
