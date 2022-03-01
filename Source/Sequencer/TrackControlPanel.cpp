#include "TrackControlPanel.h"

#include "AutoParamValueSource.h"
#include "Icons.h"
#include "PluginsModal.h"

Panel::Panel(te::AudioTrack::Ptr t)
: track(t),
  clearAutomationBtn(
      "Clear automation button",
      juce::DrawableButton::ButtonStyle::ImageOnButtonBackground),
  levelMeterView(track->getLevelMeterPlugin()->measurer),
  volumeSlider(juce::Slider::SliderStyle::LinearBarVertical,
               juce::Slider::TextEntryBoxPosition::TextBoxBelow),
  panSlider(juce::Slider::SliderStyle::LinearHorizontal,
            juce::Slider::TextEntryBoxPosition::NoTextBox)
{
    track->pluginList.state.addListener(this);

    number.setText(juce::String(track->getAudioTrackNumber()),
                   juce::dontSendNotification);
    number.setColour(juce::Label::ColourIds::backgroundColourId,
                     juce::Colours::grey);
    addAndMakeVisible(number);

    name.setText(track->getName(), juce::dontSendNotification);
    name.setMinimumHorizontalScale(1.0f);
    addAndMakeVisible(name);

    fxButton.setColour(juce::TextButton::ColourIds::buttonColourId,
                       juce::Colours::cornflowerblue);
    fxButton.onClick = [this] {
        showPluginsModal();
    };
    addAndMakeVisible(fxButton);

    autoParamSelector.setTextWhenNothingSelected("Automate");
    populateAutomationParams();
    autoParamSelector.onChange = [this] {
        auto id = autoParamSelector.getSelectedId();
        if(id == 0) {
            return;
        }

        if(id == 9999) {
            if(auto currentParam = track->getCurrentlyShownAutoParam()) {
                track->hideAutomatableParametersForSource(
                    currentParam->getOwnerID());
            }
            return;
        }

        auto index = id - 100;
        track->setCurrentlyShownAutoParam(
            track->getAllAutomatableParams()[index]);
    };
    addAndMakeVisible(autoParamSelector);

    clearAutomationBtn.setImages(Icons::getIcon(Icons::IconType::Cancel).get());
    clearAutomationBtn.onClick = [this] {
        auto currentParam = track->getCurrentlyShownAutoParam();
        if(currentParam && currentParam->hasAutomationPoints()) {
            currentParam->getCurve().clear();
        }
    };
    addChildComponent(clearAutomationBtn);

    addAndMakeVisible(levelMeterView);

    auto plug = track->getVolumePlugin();
    AutoParamValueSource::bindSliderToParameter(
        volumeSlider,
        *plug->getAutomatableParameterByID("volume"));
    volumeSlider.setTextValueSuffix("dB");
    volumeSlider.setNumDecimalPlacesToDisplay(2);
    AutoParamValueSource::bindSliderToParameter(
        panSlider,
        *plug->getAutomatableParameterByID("pan"));
    addAndMakeVisible(volumeSlider);
    addAndMakeVisible(panSlider);

    muteButton.setColour(juce::TextButton::buttonOnColourId,
                         juce::Colours::red);
    soloButton.setColour(juce::TextButton::buttonOnColourId,
                         juce::Colours::green);
    addAndMakeVisible(muteButton);
    addAndMakeVisible(soloButton);
    muteButton.onClick = [this] {
        track->setMute(!track->isMuted(false));
    };
    soloButton.onClick = [this] {
        track->setSolo(!track->isSolo(false));
    };

    valueTreePropertyChanged(track->state, te::IDs::mute);
    valueTreePropertyChanged(track->state, te::IDs::solo);
}

Panel::~Panel()
{
    track->pluginList.state.removeListener(this);
}

void Panel::resized()
{
    // Could do with breakpoints in here to cater for user resizing of the panel

    juce::Grid grid;

    using Track = juce::Grid::TrackInfo;
    using Px = juce::Grid::Px;
    using Fr = juce::Grid::Fr;
    using Margin = juce::GridItem::Margin;

    auto rowHeight = 30;
    auto trackNumberWidth = 20;
    auto levelWidth = 25;
    auto volPanWidth = 50;

    grid.templateRows = {Track(Px(rowHeight)),
                         Track(Px(rowHeight)),
                         Track(Fr(1))};
    grid.templateColumns = {
        Track(Px(trackNumberWidth)), // trackNo
        Track(Fr(1)),                // fluid area
        Track(Px(rowHeight)),        // button 1
        Track(Px(rowHeight)),        // button 2
        Track(Px(volPanWidth)),      // vol and pan
        Track(Px(levelWidth))        // level meter
    };
    juce::String rowAreas1 = "trackNo name mute solo pan level";
    juce::String rowAreas2 = "trackNo autoParam clearAuto fx vol level";
    juce::String rowAreas3 = "trackNo . . . vol level";
    grid.templateAreas = {rowAreas1, rowAreas2, rowAreas3};

    grid.items = {
        juce::GridItem(number).withArea("trackNo"),
        juce::GridItem(name).withArea("name"),
        juce::GridItem(muteButton)
            .withArea("mute")
            .withMargin(Margin(5, 2.5, 2.5, 2.5)),
        juce::GridItem(soloButton)
            .withArea("solo")
            .withMargin(Margin(5, 2.5, 2.5, 2.5)),
        juce::GridItem(panSlider).withArea("pan").withMargin(
            Margin(5, 0, 0, 0)),
        juce::GridItem(volumeSlider)
            .withArea("vol")
            .withMargin(Margin(0, 2.5, 5, 2.5)),
        juce::GridItem(levelMeterView)
            .withArea("level")
            .withMargin(Margin(5, 5, 5, 2.5)),
        juce::GridItem(autoParamSelector)
            .withArea("autoParam")
            .withMargin(Margin(2.5, 0, 5, 5)),
        juce::GridItem(fxButton).withArea("fx").withMargin(
            Margin(2.5, 2.5, 5, 2.5)),
    };

    if(clearAutomationBtn.isVisible()) {
        grid.items.add(juce::GridItem(clearAutomationBtn)
                           .withArea("clearAuto")
                           .withMargin(Margin(2.5, 2.5, 5, 0)));
    }

    grid.performLayout(getLocalBounds());
}

void Panel::paint(juce::Graphics &g)
{
    g.setColour(juce::Colours::cornflowerblue);

    // horizontal divider
    g.fillRect(0.0, (getHeight() - 0.5), float(getWidth()), 0.5f);
}

// Private methods
void Panel::handleAsyncUpdate()
{
    populateAutomationParams();

    auto currentParam = track->getCurrentlyShownAutoParam();
    if(currentParam) {
        if(currentParam->hasAutomationPoints()) {
            clearAutomationBtn.setVisible(true);
            clearAutomationBtn.setTooltip("clear all automation points");
        } else {
            clearAutomationBtn.setVisible(false);
        }

    } else {
        clearAutomationBtn.setVisible(false);
    }

    resized();
}

void Panel::valueTreeChildAdded(juce::ValueTree &p, juce::ValueTree &c)
{
    if(c.hasType(te::IDs::PLUGIN) || c.hasType(te::IDs::AUTOMATIONCURVE) ||
       c.hasType(te::IDs::POINT)) {
        triggerAsyncUpdate();
    }
}

void Panel::valueTreeChildRemoved(juce::ValueTree &p, juce::ValueTree &c, int)
{
    if(c.hasType(te::IDs::PLUGIN) || c.hasType(te::IDs::AUTOMATIONCURVE) ||
       c.hasType(te::IDs::POINT)) {
        triggerAsyncUpdate();
    }
}

void Panel::valueTreePropertyChanged(juce::ValueTree &tree,
                                     const juce::Identifier &prop)
{
    if(te::TrackList::isTrack(tree)) {
        if(prop == te::IDs::mute) {
            muteButton.setToggleState((bool)tree[prop],
                                      juce::dontSendNotification);
        }
        if(prop == te::IDs::solo) {
            soloButton.setToggleState((bool)tree[prop],
                                      juce::dontSendNotification);
        }
    }

    if(prop == te::IDs::currentAutoParamTag) {
        triggerAsyncUpdate();
    }
}

void Panel::showPluginsModal()
{
    juce::DialogWindow::LaunchOptions options;
    options.content.setOwned(new PluginsModal(track));
    options.content->setSize(600, 400);
    options.dialogBackgroundColour = juce::Colours::darkgrey;
    options.dialogTitle = "Plugins for Track " +
                          juce::String(track->getAudioTrackNumber()) + ": " +
                          track->getName();
    options.resizable = true;
    options.escapeKeyTriggersCloseButton = true;
    options.launchAsync();
}

void Panel::populateAutomationParams()
{
    auto currentParam = track->getCurrentlyShownAutoParam();

    autoParamSelector.clear(juce::dontSendNotification);

    juce::String currentName;

    auto params = track->getAllAutomatableParams();
    for(int i = 0; i < params.size(); i++) {
        auto param = params[i];
        auto ownerName = param->getPlugin()->getName();
        if(ownerName != currentName) {
            autoParamSelector.addSeparator();
            autoParamSelector.addSectionHeading(ownerName);
            currentName = ownerName;
        }
        auto itemText = param->getParameterName();
        if(param->hasAutomationPoints()) {
            itemText += " (Automated)";
        }

        autoParamSelector.addItem(itemText, i + 100);
        if(currentParam &&
           currentParam->getParameterName() == param->getParameterName()) {
            autoParamSelector.setSelectedId(i + 100);
        }
    }

    // hide automation
    if(currentParam) {
        autoParamSelector.addSeparator();
        autoParamSelector.addItem("No selection (clear)", 9999);
    }
}

// ======================================================================

TrackControlPanel::TrackControlPanel(te::Edit &e) : edit(e)
{
    edit.state.addListener(this);
    triggerAsyncUpdate();
}

TrackControlPanel::~TrackControlPanel()
{
    edit.state.removeListener(this);
}

void TrackControlPanel::resized()
{
    auto area = getLocalBounds();
    auto h = getHeight() / panels.size();
    for(auto panel : panels) {
        panel->setBounds(area.removeFromTop(h));
    }
}

void TrackControlPanel::paint(juce::Graphics &g)
{
    if(!panels.isEmpty()) {
        g.setColour(juce::Colours::darkgrey);
        g.fillRect(0, 0, getWidth(), getHeight());
    }
}

// Private methods
void TrackControlPanel::handleAsyncUpdate()
{
    createPanels();
}

void TrackControlPanel::valueTreeChildAdded(juce::ValueTree &p,
                                            juce::ValueTree &c)
{
    if(te::TrackList::isTrack(c)) {
        triggerAsyncUpdate();
    }
}

void TrackControlPanel::valueTreeChildRemoved(juce::ValueTree &p,
                                              juce::ValueTree &c,
                                              int)
{
    if(te::TrackList::isTrack(c)) {
        triggerAsyncUpdate();
    }
}

void TrackControlPanel::createPanels()
{
    panels.clear();

    for(auto t : te::getAudioTracks(edit)) {
        Panel *p = new Panel(t);
        panels.add(p);
        addAndMakeVisible(p);
    }

    resized();
}
