#include "GroupedRepetitionProtocolController.h"

GroupedRepetitionProtocolController::GroupedRepetitionProtocolController(
    aleatoric::NumberProtocolParams initialParams)
: groupingsEditor(groupings)
{
    setParams(initialParams);
    groupingsEditor.onChange = [this] {
        notifyParamsChanged();
    };
    groupingsEditorViewport.setViewedComponent(&groupingsEditor, false);
    groupingsEditorViewport.setScrollBarsShown(true, false);
    addAndMakeVisible(&groupingsEditorViewport);
}

void GroupedRepetitionProtocolController::resized()
{
    auto area = getLocalBounds();
    groupingsEditorViewport.setBounds(area);
    groupingsEditor.setBounds(area);
}

void GroupedRepetitionProtocolController::setParams(
    aleatoric::NumberProtocolParams params)
{
    jassert(params.getActiveProtocol() ==
            aleatoric::NumberProtocol::Type::groupedRepetition);

    auto groupedRepetitionParams = params.getGroupedRepetition();
    groupings = groupedRepetitionParams.getGroupings();
    groupingsEditor.redraw();
}

aleatoric::NumberProtocolParams GroupedRepetitionProtocolController::getParams()
{
    return aleatoric::NumberProtocolParams(
        aleatoric::GroupedRepetitionParams(groupings));
}
