/*
  ==============================================================================

    TransportController.cpp
    Created: 7 May 2019 7:20:07pm
    Author:  Simon Pratt

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "TransportController.h"

//==============================================================================
TransportController::TransportController(te::TransportControl &tc) : transport(tc),
                                                                     stopIcon(icons.getIcon(Icons::IconType::Stop)),
                                                                     stopButton("Stop button", DrawableButton::ButtonStyle::ImageOnButtonBackground),
                                                                     playIcon(icons.getIcon(Icons::IconType::Play)),
                                                                     pauseIcon(icons.getIcon(Icons::IconType::Pause)),
                                                                     playPauseButton("Play pause button", DrawableButton::ButtonStyle::ImageOnButtonBackground),
                                                                     transportPosition(transport)
{
    transport.addChangeListener(this);

    addAndMakeVisible(&playPauseButton);
    updatePlayPauseButtonIcon();
    playPauseButton.onClick = [this] { togglePlayPause(); };

    addAndMakeVisible(&stopButton);
    stopButton.setImages(&stopIcon);
    stopButton.onClick = [this] { stop(); };

    addAndMakeVisible(&transportPosition);
}

TransportController::~TransportController()
{
}

void TransportController::paint(Graphics &g)
{
    g.setColour(Colours::black);
    g.fillRect(getLocalBounds());
}

void TransportController::resized()
{
    FlexBox transportContainer;
    transportContainer.justifyContent = FlexBox::JustifyContent::center;
    transportContainer.alignContent = FlexBox::AlignContent::center;
    transportContainer.items.add(FlexItem(playPauseButton).withHeight(30.0f).withWidth(30.0f).withMargin(FlexItem::Margin(5.0f)));
    transportContainer.items.add(FlexItem(stopButton).withHeight(30.0f).withWidth(30.0f).withMargin(FlexItem::Margin(5.0f)));
    transportContainer.items.add(FlexItem(transportPosition).withHeight(30.0f).withWidth(100.0f).withMargin(FlexItem::Margin(5.0f)));
    transportContainer.performLayout(getLocalBounds().toFloat());
}

void TransportController::changeListenerCallback(ChangeBroadcaster *)
{
    updatePlayPauseButtonIcon();
}

void TransportController::updatePlayPauseButtonIcon()
{
    if (transport.isPlaying())
    {
        playPauseButton.setImages(&pauseIcon);
    }
    else
    {
        playPauseButton.setImages(&playIcon);
    }
}

void TransportController::togglePlayPause()
{
    if (transport.isPlaying())
    {
        transport.stop(false, false);
    }
    else
    {
        transport.play(false);
    }
}

void TransportController::stop()
{
    transport.stop(false, false);
    transport.setCurrentPosition(0.0);
}
