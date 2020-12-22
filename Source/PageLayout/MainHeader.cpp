#include "MainHeader.h"

MainHeader::MainHeader(te::Engine &e) : engine(e)
{
    addAndMakeVisible(&settingsButton);
    settingsButton.setButtonText("Settings");
    settingsButton.onClick = [this] {
        showAudioDeviceSettings(engine);
    };
}

MainHeader::~MainHeader()
{}

void MainHeader::paint(juce::Graphics &g)
{}

void MainHeader::resized()
{
    auto area = getLocalBounds();
    area.removeFromTop(10);
    area.removeFromRight(10);
    area.removeFromBottom(10);
    area.removeFromLeft(10);
    juce::FlexBox fb;
    fb.justifyContent = juce::FlexBox::JustifyContent::flexEnd;
    fb.alignContent = juce::FlexBox::AlignContent::flexStart;
    fb.items.add(
        juce::FlexItem(settingsButton).withHeight(20.0f).withWidth(100.0f));
    fb.performLayout(area.toFloat());
}

void MainHeader::showAudioDeviceSettings(te::Engine &engine)
{
    juce::DialogWindow::LaunchOptions o;
    o.dialogTitle = TRANS("Audio Settings");
    o.dialogBackgroundColour =
        juce::LookAndFeel::getDefaultLookAndFeel().findColour(
            juce::ResizableWindow::backgroundColourId);
    o.content.setOwned(new juce::AudioDeviceSelectorComponent(
        engine.getDeviceManager().deviceManager,
        0,
        512,
        1,
        512,
        false,
        false,
        true,
        true));
    o.content->setSize(400, 600);
    o.launchAsync();
}
