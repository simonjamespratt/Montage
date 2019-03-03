/*
  ==============================================================================

    WaveformDisplay.cpp
    Created: 28 Feb 2019 8:19:32pm
    Author:  Simon Pratt

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "WaveformDisplay.h"

//==============================================================================
WaveformDisplay::WaveformDisplay(
    int sourceSamplesPerThumbnailSample,
    AudioFormatManager &formatManager,
    AudioThumbnailCache &cache)
    : thumbnail(sourceSamplesPerThumbnailSample, formatManager, cache)
{
    thumbnail.addChangeListener(this);
}

void WaveformDisplay::paint(Graphics &g)
{
    if (thumbnail.getNumChannels() == 0)
        paintIfNoFileLoaded(g);
    else
        paintIfFileLoaded(g);
}

void WaveformDisplay::paintIfNoFileLoaded(Graphics &g)
{
    g.fillAll(Colours::white);
    g.setColour(Colours::darkgrey);
    g.drawFittedText("No File Loaded", getLocalBounds(), Justification::centred, 1.0f);
}

void WaveformDisplay::paintIfFileLoaded(Graphics &g)
{
    g.fillAll(Colours::white);
    g.setColour(Colours::red);
    thumbnail.drawChannels(g, getLocalBounds(), 0.0, thumbnail.getTotalLength(), 1.0f);
}

void WaveformDisplay::changeListenerCallback(ChangeBroadcaster *source)
{
    if (source == &thumbnail)
    {
        thumbnailChanged();
    }
}

void WaveformDisplay::thumbnailChanged()
{
    repaint();
}

void WaveformDisplay::setFile(const File &file)
{
    thumbnail.setSource(new FileInputSource(file));
}
