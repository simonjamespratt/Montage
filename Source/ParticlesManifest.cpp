/*
  ==============================================================================

    ParticlesManifest.cpp
    Created: 27 Apr 2019 3:41:45pm
    Author:  Simon Pratt

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "ParticlesManifest.h"

//==============================================================================
ParticlesManifest::ParticlesManifest(ValueTree &as) : appState(as), particles(), table({}, this)
{
    appState.addListener(this);
    particles = (appState.getChildWithName(particlesIdentifier));

    addAndMakeVisible(table);
    table.setColour(ListBox::outlineColourId, Colours::grey);
    table.setOutlineThickness(1);

    // Add table columns to the header
    for (int i = 0; i < dataTypes.size(); i++)
    {
        int columnId = i + 1;
        table.getHeader().addColumn(dataTypes[i].toString(), columnId, 100, 50, 400);
    }

    // Set the number of rows for the table
    numRows = particles.getNumChildren();
}

ParticlesManifest::~ParticlesManifest()
{
}

void ParticlesManifest::resized()
{
    table.setBounds(0, 0, getWidth(), getHeight());
}

int ParticlesManifest::getNumRows()
{
    return numRows;
}

void ParticlesManifest::paintRowBackground(Graphics &g, int rowNumber, int /*width*/, int /*height*/, bool rowIsSelected)
{
    auto alternateRowColour = getLookAndFeel().findColour(ListBox::backgroundColourId).interpolatedWith(getLookAndFeel().findColour(ListBox::textColourId), 0.03f);

    if (rowIsSelected)
    {
        g.fillAll(Colours::lightblue);
    }
    else if (rowNumber % 2)
    {
        g.fillAll(alternateRowColour);
    }
}

void ParticlesManifest::paintCell(Graphics &g, int rowNumber, int columnId, int width, int height, bool rowIsSelected)
{
    g.setColour(rowIsSelected ? Colours::darkblue : getLookAndFeel().findColour(ListBox::textColourId));
    g.setFont(14.0f);

    ValueTree rowElement = particles.getChild(rowNumber);
    if (rowElement.isValid())
    {
        // get prop from source (rowElement) by comparing columnId to dataType array position (columnId -1)
        auto propIdentifier = dataTypes[columnId - 1];

        if (rowElement.hasProperty(propIdentifier))
        {
            auto cellData = rowElement.getProperty(propIdentifier);
            g.drawText(cellData, 2, 0, width - 4, height, Justification::centredLeft, true);
        }
    }

    // draw RH-side column separator
    g.setColour(getLookAndFeel().findColour(ListBox::backgroundColourId));
    g.fillRect(width - 1, 0, 1, height);
}

void ParticlesManifest::valueTreeChildAdded(ValueTree &parentTree, ValueTree &childWhichHasBeenAdded)
{
    particles = (appState.getChildWithName(particlesIdentifier));
    numRows = particles.getNumChildren();
    table.updateContent();
}

void ParticlesManifest::valueTreeChildRemoved(ValueTree &parentTree, ValueTree &childWhichHasBeenRemoved, int indexFromWhichChildWasRemoved) {
    particles = (appState.getChildWithName(particlesIdentifier));
    numRows = particles.getNumChildren();
    table.updateContent();
}

void ParticlesManifest::valueTreePropertyChanged(ValueTree &treeWhosePropertyHasChanged, const Identifier &property) {
    particles = (appState.getChildWithName(particlesIdentifier));
    numRows = particles.getNumChildren();
    table.repaint();
}
// -====================================================
// Unused listeners
void ParticlesManifest::valueTreeChildOrderChanged(ValueTree &parentTreeWhoseChildrenHaveMoved, int oldInex, int newIndex) {}
void ParticlesManifest::valueTreeParentChanged(ValueTree &treeWhoseParentHasChanged) {}
// =====================================================
