#include "SourceManager.h"

SourceManager::SourceManager(juce::ValueTree &as, te::Engine &eng)
: appState(as),
  engine(eng),
  sources(),
  particles(),
  table({}, this),
  crossIcon(icons.getIcon(Icons::IconType::Cross)),
  addSourceFileButton(
      "Add source file",
      juce::DrawableButton::ButtonStyle::ImageOnButtonBackground),
  dashIcon(icons.getIcon(Icons::IconType::Dash)),
  deleteSourceFilesButton(
      "Delete source files",
      juce::DrawableButton::ButtonStyle::ImageOnButtonBackground)
{
    heading.setText("Source Manager", juce::dontSendNotification);
    heading.setFont(juce::Font(24.0f, juce::Font::bold));
    addAndMakeVisible(&heading);

    appState.addListener(this);
    sources = (appState.getChildWithName(sourcesIdentifier));
    particles = (appState.getChildWithName(particlesIdentifier));

    addAndMakeVisible(table);
    table.setColour(juce::ListBox::outlineColourId, juce::Colours::grey);
    table.setOutlineThickness(1);
    table.setMultipleSelectionEnabled(false);

    addSourceFileButton.setImages(&crossIcon);
    addAndMakeVisible(&addSourceFileButton);
    addSourceFileButton.onClick = [this] {
        selectNewSourceFile();
    };

    deleteSourceFilesButton.setImages(&dashIcon);
    addAndMakeVisible(&deleteSourceFilesButton);
    deleteSourceFilesButton.onClick = [this] {
        deleteSources();
    };

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
    numRows = sources.getNumChildren();
}

SourceManager::~SourceManager()
{}

void SourceManager::resized()
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
    headerContainer.items.add(juce::FlexItem(addSourceFileButton)
                                  .withHeight(24.0f)
                                  .withWidth(24.0f)
                                  .withMargin(juce::FlexItem::Margin(5.0f)));
    headerContainer.items.add(juce::FlexItem(deleteSourceFilesButton)
                                  .withHeight(24.0f)
                                  .withWidth(24.0f)
                                  .withMargin(juce::FlexItem::Margin(5.0f)));
    headerContainer.performLayout(headerArea);

    table.setBounds(area);
}

int SourceManager::getNumRows()
{
    return numRows;
}

void SourceManager::paintRowBackground(juce::Graphics &g,
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

void SourceManager::paintCell(juce::Graphics &g,
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

    juce::ValueTree rowElement = sources.getChild(rowNumber);
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

void SourceManager::backgroundClicked(const juce::MouseEvent &)
{
    table.deselectAllRows();
}

void SourceManager::valueTreeChildAdded(juce::ValueTree &parentTree,
                                        juce::ValueTree &childWhichHasBeenAdded)
{
    sources = (appState.getChildWithName(sourcesIdentifier));
    particles = (appState.getChildWithName(particlesIdentifier));
    numRows = sources.getNumChildren();
    table.updateContent();
}

void SourceManager::valueTreeChildRemoved(
    juce::ValueTree &parentTree,
    juce::ValueTree &childWhichHasBeenRemoved,
    int indexFromWhichChildWasRemoved)
{
    sources = (appState.getChildWithName(sourcesIdentifier));
    particles = (appState.getChildWithName(particlesIdentifier));
    numRows = sources.getNumChildren();
    table.updateContent();
}

// -====================================================
// Unused listeners
void SourceManager::valueTreePropertyChanged(
    juce::ValueTree &treeWhosePropertyHasChanged,
    const juce::Identifier &property)
{}
void SourceManager::valueTreeChildOrderChanged(
    juce::ValueTree &parentTreeWhoseChildrenHaveMoved,
    int oldInex,
    int newIndex)
{}
void SourceManager::valueTreeParentChanged(
    juce::ValueTree &treeWhoseParentHasChanged)
{}
// =====================================================

void SourceManager::selectNewSourceFile()
{
    FileManager fileManager;
    fileManager.chooseFile();

    if(!fileManager.fileIsValid(engine)) {
        showErrorMessaging(FileInvalid);
        return;
    }

    // if the fileapth already exists, error and return
    const auto filePath = fileManager.getFile().getFullPathName();
    const auto existingEntry =
        sources.getChildWithProperty(sourcePropFilePathIdentifier, filePath);
    if(existingEntry.isValid()) {
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
    if(sourceIsInUse(selectedRow)) {
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
    auto particleUsingSource =
        particles.getChildWithProperty(particlePropSourceIdIdentifier,
                                       sourceId);
    if(particleUsingSource.isValid()) {
        return true;
    }

    return false;
}
