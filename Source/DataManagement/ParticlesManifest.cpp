#include "ParticlesManifest.h"

ParticlesManifest::ParticlesManifest(ParticleList pl)
: particleList(pl), table({}, this)
{
    heading.setText("Particles Manifest", juce::dontSendNotification);
    heading.setFont(juce::Font(24.0f, juce::Font::bold));
    addAndMakeVisible(&heading);

    particleList.onObjectAdded = [this](Particle) {
        refreshView();
    };
    particleList.onObjectRemoved = [this](Particle) {
        refreshView();
    };
    particleList.onObjectUpdated = [this](Particle, juce::Identifier) {
        table.repaint();
    };

    refreshView();

    // table settings
    addAndMakeVisible(table);
    table.setColour(juce::ListBox::outlineColourId, juce::Colours::grey);
    table.setOutlineThickness(1);

    table.getHeader().addColumn("ID", Columns::id, 100, 50, 400);
    table.getHeader().addColumn("Source ID", Columns::sourceId, 100, 50, 400);
    table.getHeader().addColumn("Source file name",
                                Columns::sourceFileName,
                                100,
                                50,
                                400);
    table.getHeader().addColumn("Start", Columns::start, 100, 50, 400);
    table.getHeader().addColumn("End", Columns::end, 100, 50, 400);
    numRows = particleList.getObjects().size(); // sets no. of rows for table
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

    auto particle = particleList.getObjects()[rowNumber];
    juce::String cellText;
    switch(columnId) {
    case Columns::id:
        cellText = particle.getId().toString();
        break;
    case Columns::sourceId:
        cellText = particle.getSourceId().toString();
        break;
    case Columns::sourceFileName:
        cellText = particle.getSource().getFileName();
        break;
    case Columns::start:
        cellText = juce::String(particle.getStart());
        break;
    case Columns::end:
        cellText = juce::String(particle.getEnd());
        break;
    default:
        break;
    }

    g.drawText(cellText,
               2,
               0,
               width - 4,
               height,
               juce::Justification::centredLeft,
               true);

    // draw RH-side column separator
    g.setColour(getLookAndFeel().findColour(juce::ListBox::backgroundColourId));
    g.fillRect(width - 1, 0, 1, height);
}

// Private methods
void ParticlesManifest::refreshView()
{
    numRows = particleList.getObjects().size();
    table.updateContent();
}
