#include "TransportController.h"

#include "Icons.h"

TransportController::TransportController(te::TransportControl &tc)
: transport(tc),
  stopButton("Stop button",
             juce::DrawableButton::ButtonStyle::ImageOnButtonBackground),
  playPauseButton("Play pause button",
                  juce::DrawableButton::ButtonStyle::ImageOnButtonBackground),
  loopButton("Loop button",
             juce::DrawableButton::ButtonStyle::ImageOnButtonBackground),
  transportPosition(transport)
{
    transport.addChangeListener(this);

    playPauseButton.setImages(Icons::getIcon(Icons::IconType::Play).get(),
                              nullptr,
                              nullptr,
                              nullptr,
                              Icons::getIcon(Icons::IconType::Pause).get());
    playPauseButton.setClickingTogglesState(true);
    playPauseButton.addShortcut(juce::KeyPress(juce::KeyPress::spaceKey));
    updatePlayPauseButtonIcon();
    playPauseButton.onClick = [this] {
        togglePlayPause();
    };
    addAndMakeVisible(playPauseButton);

    stopButton.setImages(Icons::getIcon(Icons::IconType::Stop).get());
    stopButton.addShortcut(juce::KeyPress(juce::KeyPress::returnKey));
    stopButton.onClick = [this] {
        stop();
    };
    addAndMakeVisible(stopButton);

    loopButton.setImages(Icons::getIcon(Icons::IconType::Loop).get());
    loopButton.setClickingTogglesState(true);
    loopButton.addShortcut(
        juce::KeyPress('l', juce::ModifierKeys::altModifier, 0));
    loopButton.setColour(juce::TextButton::ColourIds::buttonOnColourId,
                         juce::Colour::fromFloatRGBA(0.0f, 1.0f, 0.0f, 0.5f));
    loopButton.onClick = [this] {
        auto loopingIsOn = loopButton.getToggleState();
        transport.looping = loopingIsOn;
    };
    addAndMakeVisible(loopButton);

    addAndMakeVisible(&transportPosition);

    for(auto &c : getChildren()) {
        c->setWantsKeyboardFocus(false);
    }
    setWantsKeyboardFocus(true);
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
    transportContainer.items.add(juce::FlexItem(loopButton)
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
    loopButton.setToggleState(transport.looping, juce::dontSendNotification);
}

void TransportController::updatePlayPauseButtonIcon()
{
    if(transport.isPlaying()) {
        playPauseButton.setToggleState(true, juce::dontSendNotification);
    } else {
        playPauseButton.setToggleState(false, juce::dontSendNotification);
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
    auto loopRange = transport.getLoopRange();
    if(loopRange.isEmpty()) {
        transport.setCurrentPosition(0.0);
    } else {
        transport.setCurrentPosition(loopRange.getStart());
    }

    if(onTransportStopped) {
        onTransportStopped();
    }
}

bool TransportController::handleKeyPress(const juce::KeyPress &key)
{
    if(!key.getModifiers().isAltDown()) {
        return false;
    }

    if(key.getKeyCode() == key.spaceKey) {
        togglePlayPause();
    }
    if(key.getKeyCode() == key.returnKey) {
        stop();
    }

    return true;
}
