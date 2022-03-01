#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include <tracktion_engine/tracktion_engine.h>

namespace te = tracktion_engine;

class PluginDisplay : public juce::Component,
                      private juce::ValueTree::Listener {
  public:
    PluginDisplay(te::Plugin::Ptr p);
    ~PluginDisplay();
    void resized() override;

  private:
    void valueTreePropertyChanged(juce::ValueTree &,
                                  const juce::Identifier &prop) override;

    te::Plugin::Ptr plugin;
    juce::ToggleButton enabledBtn {"Active"};
    std::vector<std::unique_ptr<juce::Label>> labels;
    std::vector<std::unique_ptr<juce::Slider>> sliders;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PluginDisplay)
};

class PluginsModal : public juce::Component,
                     juce::ListBoxModel,
                     juce::ValueTree::Listener,
                     juce::AsyncUpdater {
  public:
    PluginsModal(te::AudioTrack::Ptr t);
    ~PluginsModal();
    void resized() override;

    // ListBoxModel
    int getNumRows() override;
    void paintListBoxItem(int rowNumber,
                          juce::Graphics &g,
                          int width,
                          int height,
                          bool rowIsSelected) override;
    void listBoxItemClicked(int row, const juce::MouseEvent &) override;
    void backgroundClicked(const juce::MouseEvent &) override;

    // ValueTree::Listener
    void valueTreeChildAdded(juce::ValueTree &p, juce::ValueTree &c) override;
    void
    valueTreeChildRemoved(juce::ValueTree &p, juce::ValueTree &c, int) override;
    void valueTreePropertyChanged(juce::ValueTree &,
                                  const juce::Identifier &prop) override;

    // AsyncUpdater
    void handleAsyncUpdate() override;

  private:
    void loadPlugins();
    void displayPlugin(te::Plugin::Ptr plugin);
    void createPlugin(int id);
    void deletePlugin(int index);

    te::AudioTrack::Ptr track;
    te::Plugin::Array plugins;
    std::unique_ptr<PluginDisplay> displayedPlugin;
    juce::ListBox pluginsList;

    // NB: there are other in-built plugins available. See
    // build/_deps/tracktion-src/modules/tracktion_engine/plugins/effects/
    enum SelectablePlugins {
        chorus = 1,
        compressor,
        delay,
        eq,
        lpf,
        phaser,
        pitchShift,
        reverb,
    };
    juce::ComboBox addPluginBtn;
    juce::TextButton deletePluginBtn {"Remove"};

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PluginsModal)
};
