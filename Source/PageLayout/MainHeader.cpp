/*
  ==============================================================================

    MainHeader.cpp
    Created: 1 May 2019 7:26:23pm
    Author:  Simon Pratt

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "MainHeader.h"

//==============================================================================
MainHeader::MainHeader(te::Engine &e) : engine(e)
{
    appName.setText("Montage", dontSendNotification);
    appName.setFont(Font(32.0f, Font::bold));
    addAndMakeVisible(&appName);

    addAndMakeVisible(&settingsButton);
    settingsButton.setButtonText("Settings");
    settingsButton.onClick = [this] { showAudioDeviceSettings(engine); };
}

MainHeader::~MainHeader()
{
}

void MainHeader::paint(Graphics &g)
{
}

void MainHeader::resized()
{
    auto area = getLocalBounds();
    area.removeFromTop(10);
    area.removeFromRight(10);
    area.removeFromBottom(10);
    area.removeFromLeft(10);
    FlexBox fb;
    fb.justifyContent = FlexBox::JustifyContent::spaceBetween;
    fb.alignContent = FlexBox::AlignContent::flexStart;
    fb.items.add(FlexItem(appName).withMinHeight(50.0f).withMinWidth(200.0f));
    fb.items.add(FlexItem(settingsButton).withHeight(20.0f).withWidth(100.0f));
    fb.performLayout(area.toFloat());
}

void MainHeader::showAudioDeviceSettings(te::Engine &engine)
{
    DialogWindow::LaunchOptions o;
    o.dialogTitle = TRANS("Audio Settings");
    o.dialogBackgroundColour = LookAndFeel::getDefaultLookAndFeel().findColour(ResizableWindow::backgroundColourId);
    o.content.setOwned(new AudioDeviceSelectorComponent(engine.getDeviceManager().deviceManager,
                                                        0, 512, 1, 512, false, false, true, true));
    o.content->setSize(400, 600);
    o.launchAsync();
}
