#include "SequencerControlsView.h"

#include "AudioRenderer.h"
#include "ErrorMessageModal.h"

SequencerControlsView::SequencerControlsView(SequencerViewState &s,
                                             te::Edit &e,
                                             ProjectState &ps)
: sequencerViewState(s),
  edit(e),
  projectState(ps),
  xZoom(juce::Slider::SliderStyle::LinearHorizontal,
        juce::Slider::TextEntryBoxPosition::NoTextBox),
  yZoom(juce::Slider::SliderStyle::LinearVertical,
        juce::Slider::TextEntryBoxPosition::NoTextBox),
  trackControlPanelWidthAdjuster(juce::Slider::SliderStyle::LinearHorizontal,
                                 juce::Slider::TextEntryBoxPosition::NoTextBox),
  transportController(edit.getTransport())
{
    xZoom.setRange(sequencerViewState.timeScalingFactorMin,
                   sequencerViewState.timeScalingFactorMax);
    xZoom.getValueObject().referTo(
        sequencerViewState.timeScalingFactorCurrent.getPropertyAsValue());
    addAndMakeVisible(xZoom);

    yZoom.setRange(sequencerViewState.trackHeightMin,
                   sequencerViewState.trackHeightMax);
    yZoom.getValueObject().referTo(
        sequencerViewState.trackHeight.getPropertyAsValue());
    addAndMakeVisible(yZoom);

    trackControlPanelWidthAdjuster.setRange(sequencerViewState.panelsWidthMin,
                                            sequencerViewState.panelsWidthMax);
    trackControlPanelWidthAdjuster.getValueObject().referTo(
        sequencerViewState.panelsWidth.getPropertyAsValue());
    addAndMakeVisible(trackControlPanelWidthAdjuster);

    transportController.onTransportStopped = [this] {
        sequencerViewState.viewportSyncToTransportRequired = true;
    };
    addAndMakeVisible(transportController);

    renderButton.onClick = [this] {
        if(edit.getLength() < 0) {
            std::make_shared<ErrorMessageModal>(
                "Cannot render because no figure is currently selected.");
            return;
        }

        AudioRenderer::renderFigureToFile(edit,
                                          projectState,
                                          sequencerViewState.figureName);
    };
    addAndMakeVisible(renderButton);
}

void SequencerControlsView::resized()
{
    auto area = getLocalBounds();
    auto transportArea = area.reduced(200, 0);
    auto zoomControlsArea = area.removeFromRight(200);
    auto tcpWidthAdjusterArea = area.removeFromLeft(100);
    auto renderArea = area.removeFromRight(100);

    xZoom.setBounds(
        zoomControlsArea.removeFromLeft(zoomControlsArea.getWidth() / 2));
    yZoom.setBounds(zoomControlsArea);
    trackControlPanelWidthAdjuster.setBounds(tcpWidthAdjusterArea);
    transportController.setBounds(transportArea);
    renderButton.setBounds(renderArea.reduced(10));
}

// Private methods
void SequencerControlsView::valueTreePropertyChanged(
    juce::ValueTree &tree, const juce::Identifier &prop)
{
    if(prop == IDs::sequencerLoaded) {
        transportController.grabKeyboardFocus();
    }
}
