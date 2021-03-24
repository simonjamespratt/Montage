#include "WalkProtocolController.h"

WalkProtocolController::WalkProtocolController(
    aleatoric::NumberProtocolParams initialParams)
: maxStepEditor(maxStep, "Max step")
{
    setParams(initialParams);
    maxStepEditor.onChange = [this] {
        // NB: There is no check for exceeding the range size here. If the user
        // enters a value > aleatoric::NumberProtocol range.size, then aleatoric
        // will throw an exception. The way the code is designed currently,
        // there is no way of knowing the range at this Controller level. Either
        // it needs to be passed down (which includes when things change and
        // setParams() is called), or the Producer needs to be passed down
        // (things changing still applies), or aleatoric needs to make the range
        // available via CollectionsProducer and DurationsProducer. For
        // instance, they could both return NumberProtocolConfig (which includes
        // the range), instead of NumberProtocolParams. They would probably
        // still both want to receive NumberProtocolParams when they have their
        // setParams() methods called

        // NB: avoids annoying situation where when user deletes the value
        // (which sets maxStep to 0) you get an error because maxStep must not
        // be < 1
        if(maxStep < 1) {
            maxStep = 1;
        }

        notifyParamsChanged();
    };
    addAndMakeVisible(&maxStepEditor);
}

void WalkProtocolController::setParams(aleatoric::NumberProtocolParams params)
{
    jassert(params.getActiveProtocol() ==
            aleatoric::NumberProtocol::Type::walk);

    maxStep = params.getWalk().getMaxStep();
    maxStepEditor.update();
}

aleatoric::NumberProtocolParams WalkProtocolController::getParams()
{
    return aleatoric::NumberProtocolParams(aleatoric::WalkParams(maxStep));
}

void WalkProtocolController::resized()
{
    maxStepEditor.setBounds(getLocalBounds().removeFromTop(45));
}
