/*
  ==============================================================================

    SourceManager.cpp
    Created: 22 Apr 2019 7:30:41pm
    Author:  Simon Pratt

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "SourceManager.h"

//==============================================================================
SourceManager::SourceManager(ValueTree &as) :   appState(as),
                                                sources(),
                                                particles(),
                                                table({}, this),
                                                crossIcon(icons.getIcon(Icons::IconType::Cross)),
                                                addSourceFileButton("Add source file", DrawableButton::ButtonStyle::ImageOnButtonBackground),
                                                dashIcon(icons.getIcon(Icons::IconType::Dash)),
                                                deleteSourceFilesButton("Delete source files", DrawableButton::ButtonStyle::ImageOnButtonBackground)
{
    heading.setText("Source Manager", dontSendNotification);
    heading.setFont(Font(24.0f, Font::bold));
    addAndMakeVisible(&heading);
    
    appState.addListener(this);
    sources = (appState.getChildWithName(sourcesIdentifier));
    particles = (appState.getChildWithName(particlesIdentifier));

    addAndMakeVisible(table);
    table.setColour(ListBox::outlineColourId, Colours::grey);
    table.setOutlineThickness(1);
    table.setMultipleSelectionEnabled(false);
    
    addSourceFileButton.setImages(&crossIcon);
    addAndMakeVisible(&addSourceFileButton);
    addSourceFileButton.onClick = [this] { selectNewSourceFile(); };
    
    deleteSourceFilesButton.setImages(&dashIcon);
    addAndMakeVisible(&deleteSourceFilesButton);
    deleteSourceFilesButton.onClick = [this] { deleteSources(); };

    // Add table columns to the header
    for (int i = 0; i < dataTypes.size(); i++)
    {
        int columnId = i + 1;
        table.getHeader().addColumn(dataTypes[i].toString(), columnId, 100, 50, 400);
    }

    // Set the number of rows for the table
    numRows = sources.getNumChildren();
}

SourceManager::~SourceManager()
{
}

void SourceManager::resized()
{
    auto area = getLocalBounds();
    auto headerArea = area.removeFromTop(50);
    area.removeFromRight(5);
    area.removeFromBottom(10);
    area.removeFromLeft(5);
    
    FlexBox headerContainer;
    headerContainer.justifyContent = FlexBox::JustifyContent::flexStart;
    headerContainer.alignContent = FlexBox::AlignContent::center;
    headerContainer.items.add(FlexItem(heading).withHeight(24.0f).withWidth(200.0f).withMargin(FlexItem::Margin(5.0f)));
    headerContainer.items.add(FlexItem(addSourceFileButton).withHeight(24.0f).withWidth(24.0f).withMargin(FlexItem::Margin(5.0f)));
    headerContainer.items.add(FlexItem(deleteSourceFilesButton).withHeight(24.0f).withWidth(24.0f).withMargin(FlexItem::Margin(5.0f)));
    headerContainer.performLayout(headerArea);
    
    table.setBounds(area);
}

int SourceManager::getNumRows()
{
    return numRows;
}

void SourceManager::paintRowBackground(Graphics &g, int rowNumber, int /*width*/, int /*height*/, bool rowIsSelected)
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

void SourceManager::paintCell(Graphics &g, int rowNumber, int columnId, int width, int height, bool rowIsSelected)
{
    g.setColour(rowIsSelected ? Colours::darkblue : getLookAndFeel().findColour(ListBox::textColourId));
    g.setFont(14.0f);

    ValueTree rowElement = sources.getChild(rowNumber);
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

void SourceManager::backgroundClicked(const MouseEvent&)
{
    table.deselectAllRows();
}

void SourceManager::valueTreeChildAdded(ValueTree &parentTree, ValueTree &childWhichHasBeenAdded)
{
    sources = (appState.getChildWithName(sourcesIdentifier));
    particles = (appState.getChildWithName(particlesIdentifier));
    numRows = sources.getNumChildren();
    table.updateContent();
}

void SourceManager::valueTreeChildRemoved(ValueTree &parentTree, ValueTree &childWhichHasBeenRemoved, int indexFromWhichChildWasRemoved) {
    sources = (appState.getChildWithName(sourcesIdentifier));
    particles = (appState.getChildWithName(particlesIdentifier));
    numRows = sources.getNumChildren();
    table.updateContent();
}

// -====================================================
// Unused listeners
void SourceManager::valueTreePropertyChanged(ValueTree &treeWhosePropertyHasChanged, const Identifier &property) {}
void SourceManager::valueTreeChildOrderChanged(ValueTree &parentTreeWhoseChildrenHaveMoved, int oldInex, int newIndex) {}
void SourceManager::valueTreeParentChanged(ValueTree &treeWhoseParentHasChanged) {}
// =====================================================

void SourceManager::selectNewSourceFile()
{
    FileManager fileManager;
    fileManager.chooseFile();

    if (!fileManager.fileIsValid())
    {
        showErrorMessaging(FileInvalid);
        return;
    }

    // if the fileapth already exists, error and return
    const auto filePath = fileManager.getFile().getFullPathName();
    const auto existingEntry = sources.getChildWithProperty(sourcePropFilePathIdentifier, filePath);
    if (existingEntry.isValid())
    {
        showErrorMessaging(FileAlreadyExists);
        return;
    }

    fileManager.addSourceToState(sources);
}

void SourceManager::showErrorMessaging(const ErrorType &errorType)
{
    std::make_shared<ErrorManager>(errorType);
}

void SourceManager::deleteSources()
{
    auto selectedRow = table.getLastRowSelected();
    if (sourceIsInUse(selectedRow))
    {
        showErrorMessaging(DeleteSourceInvalidSourceInUse);
        return;
    }
    sources.removeChild(selectedRow, nullptr);

    numRows = sources.getNumChildren();
    table.updateContent();
}

bool SourceManager::sourceIsInUse(int index)
{
    auto source = sources.getChild(index);
    int sourceId = source.getProperty(sourcePropIdIdentifier);
    auto particleUsingSource = particles.getChildWithProperty(particlePropSourceIdIdentifier, sourceId);
    if (particleUsingSource.isValid())
    {
        return true;
    }

    return false;
}
