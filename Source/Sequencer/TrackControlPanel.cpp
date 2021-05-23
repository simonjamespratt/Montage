#include "TrackControlPanel.h"

Panel::Panel(juce::String trackNumber, juce::String trackName)
: number(trackNumber, trackNumber), name(trackName, trackName)
{
    addAndMakeVisible(number);
    addAndMakeVisible(name);
    name.setMinimumHorizontalScale(1.0f);
}

void Panel::resized()
{
    auto labelHeight = name.getFont().getHeight();

    juce::FlexBox fb;
    fb.flexWrap = juce::FlexBox::Wrap::noWrap;
    fb.justifyContent = juce::FlexBox::JustifyContent::flexStart;
    fb.alignContent = juce::FlexBox::AlignContent::center;

    juce::FlexItem numberItem(20, labelHeight, number);
    juce::FlexItem nameItem(getWidth() - 20, labelHeight, name);
    fb.items.addArray({numberItem, nameItem});
    fb.performLayout(getLocalBounds().toFloat());
}

void Panel::paint(juce::Graphics &g)
{
    g.setColour(juce::Colours::cornflowerblue);
    g.fillRect(20.0f, 0.0f, 0.5f, (float)getHeight());
}

TrackControlPanel::TrackControlPanel(float initialTrackHeight)
: trackHeight(initialTrackHeight)
{}

void TrackControlPanel::resized()
{
    if(panels.size() > 0) {
        auto area = getLocalBounds();
        auto totalHeight = area.getHeight();
        auto trackHeight = totalHeight / panels.size();
        for(auto &panel : panels) {
            panel->setBounds(area.removeFromTop(trackHeight));
        }
    }
}

void TrackControlPanel::paint(juce::Graphics &g)
{
    if(!panels.empty()) {
        g.setColour(juce::Colours::darkgrey);
        g.fillRect(0, 0, getWidth(), getHeight());
    }

    g.setColour(juce::Colours::cornflowerblue);
    float currentPosition = 0;
    for(size_t i = 0; i < panels.size(); i++) {
        currentPosition += trackHeight;
        g.fillRect(0.0, (currentPosition - 0.5), float(getWidth()), 0.5f);
    }
}

void TrackControlPanel::createPanels(ParticleList particleList)
{
    panels.clear();
    auto particles = particleList.getObjects();
    for(size_t i = 0; i < particles.size(); i++) {
        auto particle = particles[i];
        panels.push_back(
            std::make_unique<Panel>(juce::String(i + 1), particle.getName()));
    }

    for(auto &panel : panels) {
        addAndMakeVisible(*panel);
    }

    resized();
}

void TrackControlPanel::clear()
{
    panels.clear();
    resized();
}

void TrackControlPanel::setTrackHeight(float newHeight)
{
    trackHeight = newHeight;
}
