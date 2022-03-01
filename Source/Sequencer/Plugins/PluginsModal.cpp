#include "PluginsModal.h"

#include "AutoParamValueSource.h"

PluginDisplay::PluginDisplay(te::Plugin::Ptr p) : plugin(p)
{
    plugin->state.addListener(this);
    addAndMakeVisible(enabledBtn);
    enabledBtn.setToggleState(plugin->isEnabled(), juce::dontSendNotification);
    enabledBtn.onClick = [this] {
        plugin->setEnabled(enabledBtn.getToggleState());
    };

    for(auto param : plugin->getAutomatableParameters()) {
        auto paramName = param->getParameterName();
        labels.emplace_back(
            std::make_unique<juce::Label>(paramName, paramName));

        auto slider = std::make_unique<juce::Slider>();
        AutoParamValueSource::bindSliderToParameter(*slider, *param);
        sliders.emplace_back(std::move(slider));
    }

    for(auto &label : labels) {
        addAndMakeVisible(*label);
    }

    for(auto &slider : sliders) {
        addAndMakeVisible(*slider);
    }
}

PluginDisplay::~PluginDisplay()
{
    plugin->state.removeListener(this);
}

void PluginDisplay::resized()
{
    auto area = getLocalBounds();
    area.reduce(20, 20);
    auto enabledArea = area.removeFromTop(50);
    auto labelsArea = area.removeFromLeft(100);
    auto slidersArea = area;

    enabledBtn.setBounds(enabledArea);

    for(auto &label : labels) {
        label->setBounds(labelsArea.removeFromTop(50));
    }

    for(auto &slider : sliders) {
        slider->setBounds(slidersArea.removeFromTop(50));
    }
}

// Private methods
void PluginDisplay::valueTreePropertyChanged(juce::ValueTree &,
                                             const juce::Identifier &prop)
{
    if(prop == te::IDs::enabled) {
        enabledBtn.setToggleState(plugin->isEnabled(),
                                  juce::dontSendNotification);
    }
}

// ======================================================================

PluginsModal::PluginsModal(te::AudioTrack::Ptr t)
: track(t), pluginsList({}, this)
{
    track->pluginList.state.addListener(this);
    loadPlugins();
    pluginsList.setMultipleSelectionEnabled(false);
    addAndMakeVisible(pluginsList);

    addPluginBtn.setTextWhenNothingSelected("Add");
    addPluginBtn.addItem("Chorus", SelectablePlugins::chorus);
    addPluginBtn.addItem("Compressor", SelectablePlugins::compressor);
    addPluginBtn.addItem("Delay", SelectablePlugins::delay);
    addPluginBtn.addItem("Equaliser", SelectablePlugins::eq);
    addPluginBtn.addItem("LPF/HPF", SelectablePlugins::lpf);
    addPluginBtn.addItem("Phaser", SelectablePlugins::phaser);
    addPluginBtn.addItem("Pitch shift", SelectablePlugins::pitchShift);
    addPluginBtn.addItem("Reverb", SelectablePlugins::reverb);
    addPluginBtn.onChange = [this] {
        auto id = addPluginBtn.getSelectedId();
        if(id == 0) {
            return;
        }
        createPlugin(id);
        addPluginBtn.setSelectedId(0);
    };
    addAndMakeVisible(addPluginBtn);

    deletePluginBtn.onClick = [this] {
        auto index = pluginsList.getLastRowSelected();
        deletePlugin(index);
    };
    addAndMakeVisible(deletePluginBtn);
}

PluginsModal::~PluginsModal()
{
    track->pluginList.state.removeListener(this);
}

void PluginsModal::resized()
{
    auto area = getLocalBounds();
    auto pluginsListArea = area.removeFromLeft(200);
    auto controlsArea = pluginsListArea.removeFromBottom(50);

    pluginsList.setBounds(pluginsListArea);
    addPluginBtn.setBounds(controlsArea.removeFromLeft(100).reduced(5));
    deletePluginBtn.setBounds(controlsArea.reduced(5));

    if(displayedPlugin != nullptr) {
        displayedPlugin->setBounds(area);
    }
}

int PluginsModal::getNumRows()
{
    return plugins.size();
}

void PluginsModal::paintListBoxItem(
    int rowNumber, juce::Graphics &g, int width, int height, bool rowIsSelected)
{
    auto plugin = plugins[rowNumber];
    if(plugin == nullptr) {
        return;
    }

    // background
    auto alternateRowColour =
        getLookAndFeel()
            .findColour(juce::ListBox::backgroundColourId)
            .interpolatedWith(
                getLookAndFeel().findColour(juce::ListBox::textColourId),
                0.03f);

    if(rowIsSelected) {
        g.fillAll(juce::Colours::lightblue);
    } else if(rowNumber % 2) {
        g.fillAll(alternateRowColour);
    }

    // text
    auto text = plugin->getName();
    if(!plugin->isEnabled()) {
        text += " (bypassed)";
    }

    g.setColour(rowIsSelected
                    ? juce::Colours::darkblue
                    : getLookAndFeel().findColour(juce::ListBox::textColourId));
    g.setFont(14.0f);
    g.drawText(text, 2, 0, width - 4, height, juce::Justification::centredLeft);
}

void PluginsModal::listBoxItemClicked(int row, const juce::MouseEvent &)
{
    auto plug = plugins[row];
    if(plug == nullptr) {
        return;
    }

    displayPlugin(plug);
}

void PluginsModal::backgroundClicked(const juce::MouseEvent &)
{
    pluginsList.deselectAllRows();
    displayedPlugin = nullptr;
    resized();
}

void PluginsModal::valueTreeChildAdded(juce::ValueTree &p, juce::ValueTree &c)
{
    if(c.hasType(te::IDs::PLUGIN)) {
        triggerAsyncUpdate();
    }
}

void PluginsModal::valueTreeChildRemoved(juce::ValueTree &p,
                                         juce::ValueTree &c,
                                         int)
{
    if(c.hasType(te::IDs::PLUGIN)) {
        triggerAsyncUpdate();
    }
}

void PluginsModal::valueTreePropertyChanged(juce::ValueTree &,
                                            const juce::Identifier &prop)
{
    if(prop == te::IDs::enabled) {
        triggerAsyncUpdate();
    }
}

void PluginsModal::handleAsyncUpdate()
{
    loadPlugins();
    resized();
}

void PluginsModal::loadPlugins()
{
    plugins = track->pluginList.getPlugins();
    pluginsList.updateContent();
    pluginsList.repaint();
}

void PluginsModal::displayPlugin(te::Plugin::Ptr plugin)
{
    displayedPlugin = std::make_unique<PluginDisplay>(plugin);
    addAndMakeVisible(*displayedPlugin);
    resized();
}

void PluginsModal::createPlugin(int id)
{
    juce::String type;
    switch(id) {
    case SelectablePlugins::chorus:
        type = te::ChorusPlugin::xmlTypeName;
        break;
    case SelectablePlugins::compressor:
        type = te::CompressorPlugin::xmlTypeName;
        break;
    case SelectablePlugins::delay:
        type = te::DelayPlugin::xmlTypeName;
        break;
    case SelectablePlugins::eq:
        type = te::EqualiserPlugin::xmlTypeName;
        break;
    case SelectablePlugins::lpf:
        type = te::LowPassPlugin::xmlTypeName;
        break;
    case SelectablePlugins::phaser:
        type = te::PhaserPlugin::xmlTypeName;
        break;
    case SelectablePlugins::pitchShift:
        type = te::PitchShiftPlugin::xmlTypeName;
        break;
    case SelectablePlugins::reverb:
        type = te::ReverbPlugin::xmlTypeName;
        break;

    default:
        break;
    }

    auto newPlugin = track->edit.getPluginCache().createNewPlugin(type, {});
    track->pluginList.insertPlugin(newPlugin, -1, nullptr);
}

void PluginsModal::deletePlugin(int index)
{
    auto plugin = plugins[index];

    // don't allow vol/pan or level to be deleted!
    if(plugin == nullptr || plugin->getIdentifierString() == "volume" ||
       plugin->getIdentifierString() == "level") {
        return;
    }
    displayedPlugin = nullptr;
    pluginsList.deselectAllRows();
    plugin->deleteFromParent();
}
