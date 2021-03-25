#include "FigureManager.h"

FigureManager::FigureManager() : table({}, this)
{
    heading.setText("Figure Manager", juce::dontSendNotification);
    heading.setFont(juce::Font(24.0f, juce::Font::bold));
    addAndMakeVisible(&heading);

    addAndMakeVisible(table);
    table.setColour(juce::ListBox::outlineColourId, juce::Colours::grey);
    table.setOutlineThickness(1);
    table.setMultipleSelectionEnabled(false);

    table.getHeader().addColumn("Event Number",
                                Columns::eventNum,
                                100,
                                50,
                                400);
    table.getHeader().addColumn("ID", Columns::id, 100, 50, 400);
    table.getHeader().addColumn("Onset", Columns::onset, 100, 50, 400);
    table.getHeader().addColumn("Particle start",
                                Columns::particleStart,
                                100,
                                50,
                                400);
    table.getHeader().addColumn("Particle end",
                                Columns::particleEnd,
                                100,
                                50,
                                400);
    table.getHeader().addColumn("File name", Columns::fileName, 100, 50, 400);
}

FigureManager::~FigureManager()
{}

void FigureManager::resized()
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

int FigureManager::getNumRows()
{
    return numRows;
}

void FigureManager::paintRowBackground(juce::Graphics &g,
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

void FigureManager::paintCell(juce::Graphics &g,
                              int rowNumber,
                              int columnId,
                              int width,
                              int height,
                              bool rowIsSelected)
{
    if(eventList == nullptr) {
        return;
    }

    g.setColour(rowIsSelected
                    ? juce::Colours::darkblue
                    : getLookAndFeel().findColour(juce::ListBox::textColourId));
    g.setFont(14.0f);

    auto event = eventList->getObjects()[rowNumber];
    juce::String cellText;
    switch(columnId) {
    case Columns::eventNum:
        cellText = juce::String(rowNumber + 1);
        break;
    case Columns::id:
        cellText = event.getId().toString();
        break;
    case Columns::onset:
        cellText = juce::String(event.getOnset());
        break;
    case Columns::particleStart:
        cellText = juce::String(event.getParticle().getStart());
        break;
    case Columns::particleEnd:
        cellText = juce::String(event.getParticle().getEnd());
        break;
    case Columns::fileName:
        cellText = juce::String(event.getParticle().getSource().getFileName());
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

void FigureManager::backgroundClicked(const juce::MouseEvent &)
{
    table.deselectAllRows();
}

void FigureManager::setData(const Figure &f, const ProjectState &ps)
{
    eventList = std::make_unique<EventList>(ps.getEventList(f));
    numRows = eventList->getObjects().size();
    table.updateContent();
}
