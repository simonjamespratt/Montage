#include "TransportController.h"

TransportController::TransportController(te::TransportControl &tc)
: transport(tc),
  stopIcon(icons.getIcon(Icons::IconType::Stop)),
  stopButton("Stop button",
             juce::DrawableButton::ButtonStyle::ImageOnButtonBackground),
  playIcon(icons.getIcon(Icons::IconType::Play)),
  pauseIcon(icons.getIcon(Icons::IconType::Pause)),
  playPauseButton("Play pause button",
                  juce::DrawableButton::ButtonStyle::ImageOnButtonBackground),
  transportPosition(transport)
{
    transport.addChangeListener(this);

    addAndMakeVisible(&playPauseButton);
    updatePlayPauseButtonIcon();
    playPauseButton.onClick = [this] {
        togglePlayPause();
    };

    addAndMakeVisible(&stopButton);
    stopButton.setImages(&stopIcon);
    stopButton.onClick = [this] {
        stop();
    };

    addAndMakeVisible(&transportPosition);
}

TransportController::~TransportController()
{}

void TransportController::paint(juce::Graphics &g)
{
    g.setColour(juce::Colours::black);
    g.fillRect(getLocalBounds());
}

void TransportController::resized()
{
    juce::FlexBox transportContainer;
    transportContainer.justifyContent = juce::FlexBox::JustifyContent::center;
    transportContainer.alignContent = juce::FlexBox::AlignContent::center;
    transportContainer.items.add(juce::FlexItem(playPauseButton)
                                     .withHeight(30.0f)
                                     .withWidth(30.0f)
                                     .withMargin(juce::FlexItem::Margin(5.0f)));
    transportContainer.items.add(juce::FlexItem(stopButton)
                                     .withHeight(30.0f)
                                     .withWidth(30.0f)
                                     .withMargin(juce::FlexItem::Margin(5.0f)));
    transportContainer.items.add(juce::FlexItem(transportPosition)
                                     .withHeight(30.0f)
                                     .withWidth(100.0f)
                                     .withMargin(juce::FlexItem::Margin(5.0f)));
    transportContainer.performLayout(getLocalBounds().toFloat());
}

void TransportController::changeListenerCallback(juce::ChangeBroadcaster *)
{
    updatePlayPauseButtonIcon();
}

void TransportController::updatePlayPauseButtonIcon()
{
    if(transport.isPlaying()) {
        playPauseButton.setImages(&pauseIcon);
    } else {
        playPauseButton.setImages(&playIcon);
    }
}

void TransportController::togglePlayPause()
{
    if(transport.isPlaying()) {
        transport.stop(false, false);
    } else {
        transport.play(false);
    }
}

void TransportController::stop()
{
    transport.stop(false, false);
    transport.setCurrentPosition(0.0);
}
