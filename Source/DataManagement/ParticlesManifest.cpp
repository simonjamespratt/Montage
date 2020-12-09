#include "ParticlesManifest.h"

ParticlesManifest::ParticlesManifest(juce::ValueTree &as)
: appState(as), particles(), table({}, this)
{
    heading.setText("Particles Manifest", juce::dontSendNotification);
    heading.setFont(juce::Font(24.0f, juce::Font::bold));
    addAndMakeVisible(&heading);

    appState.addListener(this);
    particles = (appState.getChildWithName(IDs::PARTICLES));

    addAndMakeVisible(table);
    table.setColour(juce::ListBox::outlineColourId, juce::Colours::grey);
    table.setOutlineThickness(1);

    // Add table columns to the header
    for(int i = 0; i < dataTypes.size(); i++) {
        int columnId = i + 1;
        table.getHeader().addColumn(dataTypes[i].toString(),
                                    columnId,
                                    100,
                                    50,
                                    400);
    }

    // Set the number of rows for the table
    numRows = particles.getNumChildren();
}

ParticlesManifest::~ParticlesManifest()
{}

void ParticlesManifest::resized()
{
    auto area = getLocalBounds();
    auto headerArea = area.removeFromTop(50);
    area.removeFromRight(5);
    area.removeFromBottom(10);
    area.removeFromLeft(5);

    juce::FlexBox headerContainer;
    headerContainer.justifyContent = juce::FlexBox::JustifyContent::flexStart;
    headerContainer.alignContent = juce::FlexBox::AlignContent::center;
    headerContainer.items.add(
        juce::FlexItem(heading).withHeight(24.0f).withWidth(200.0f).withMargin(
            juce::FlexItem::Margin(5.0f)));
    headerContainer.performLayout(headerArea);

    table.setBounds(area);
}

int ParticlesManifest::getNumRows()
{
    return numRows;
}

void ParticlesManifest::paintRowBackground(juce::Graphics &g,
                                           int rowNumber,
                                           int /*width*/,
                                           int /*height*/,
                                           bool rowIsSelected)
{
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
}

void ParticlesManifest::paintCell(juce::Graphics &g,
                                  int rowNumber,
                                  int columnId,
                                  int width,
                                  int height,
                                  bool rowIsSelected)
{
    g.setColour(rowIsSelected
                    ? juce::Colours::darkblue
                    : getLookAndFeel().findColour(juce::ListBox::textColourId));
    g.setFont(14.0f);

    juce::ValueTree rowElement = particles.getChild(rowNumber);
    if(rowElement.isValid()) {
        // get prop from source (rowElement) by comparing columnId to dataType
        // array position (columnId -1)
        auto propIdentifier = dataTypes[columnId - 1];

        if(rowElement.hasProperty(propIdentifier)) {
            auto cellData = rowElement.getProperty(propIdentifier);
            g.drawText(cellData,
                       2,
                       0,
                       width - 4,
                       height,
                       juce::Justification::centredLeft,
                       true);
        }
    }

    // draw RH-side column separator
    g.setColour(getLookAndFeel().findColour(juce::ListBox::backgroundColourId));
    g.fillRect(width - 1, 0, 1, height);
}

void ParticlesManifest::valueTreeChildAdded(
    juce::ValueTree &parentTree, juce::ValueTree &childWhichHasBeenAdded)
{
    particles = (appState.getChildWithName(IDs::PARTICLES));
    numRows = particles.getNumChildren();
    table.updateContent();
}

void ParticlesManifest::valueTreeChildRemoved(
    juce::ValueTree &parentTree,
    juce::ValueTree &childWhichHasBeenRemoved,
    int indexFromWhichChildWasRemoved)
{
    particles = (appState.getChildWithName(IDs::PARTICLES));
    numRows = particles.getNumChildren();
    table.updateContent();
}

void ParticlesManifest::valueTreePropertyChanged(
    juce::ValueTree &treeWhosePropertyHasChanged,
    const juce::Identifier &property)
{
    particles = (appState.getChildWithName(IDs::PARTICLES));
    numRows = particles.getNumChildren();
    table.repaint();
}
// -====================================================
// Unused listeners
void ParticlesManifest::valueTreeChildOrderChanged(
    juce::ValueTree &parentTreeWhoseChildrenHaveMoved,
    int oldInex,
    int newIndex)
{}
void ParticlesManifest::valueTreeParentChanged(
    juce::ValueTree &treeWhoseParentHasChanged)
{}
// =====================================================
