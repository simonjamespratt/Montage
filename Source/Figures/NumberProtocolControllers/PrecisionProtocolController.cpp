#include "PrecisionProtocolController.h"

PrecisionProtocolController::PrecisionProtocolController(
    aleatoric::NumberProtocolParams initialParams)
{
    setParams(initialParams);
    editorsViewport.setViewedComponent(&editorsContainer, false);
    editorsViewport.setScrollBarsShown(true, false);
    addAndMakeVisible(&editorsViewport);
}

void PrecisionProtocolController::resized()
{
    auto area = getLocalBounds();
    editorsViewport.setBounds(area);
    editorsContainer.setBounds(area);

    int componentHeight = 45;

    auto totalContainerHeight =
        editorsContainer.getNumChildComponents() * componentHeight;
    editorsContainer.setSize(editorsContainer.getWidth(), totalContainerHeight);

    auto editorsContainerArea = editorsContainer.getLocalBounds();
    for(auto &editor : editorsContainer.getChildren()) {
        editor->setBounds(editorsContainerArea.removeFromTop(componentHeight));
    }
}

void PrecisionProtocolController::setParams(
    aleatoric::NumberProtocolParams params)
{
    jassert(params.getActiveProtocol() ==
            aleatoric::NumberProtocol::Type::precision);

    editorsContainer.removeAllChildren();
    editors.clear();
    distribution = params.getPrecision().getDistribution();

    int labelNum = 1;
    for(auto &value : distribution) {
        auto editor = std::make_unique<SliderWithLabel>(value,
                                                        juce::String(labelNum),
                                                        0,
                                                        1.0,
                                                        "",
                                                        2,
                                                        50);

        editors.emplace_back(std::move(editor));
        labelNum++;
    }

    for(auto &editor : editors) {
        editor->onChange = [this] {
            notifyParamsChanged();
        };
        editorsContainer.addAndMakeVisible(*editor);
    }

    resized();
}

aleatoric::NumberProtocolParams PrecisionProtocolController::getParams()
{
    return aleatoric::NumberProtocolParams(
        aleatoric::PrecisionParams(distribution));
}
