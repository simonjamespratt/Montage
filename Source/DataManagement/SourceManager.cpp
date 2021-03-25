#include "SourceManager.h"

#include "ErrorMessageModal.h"

SourceManager::SourceManager(SourceList sl)
: sourceList(sl),
  table({}, this),
  crossIcon(icons.getIcon(Icons::IconType::Cross)),
  addSourceButton("Add source file",
                  juce::DrawableButton::ButtonStyle::ImageOnButtonBackground),
  dashIcon(icons.getIcon(Icons::IconType::Dash)),
  deleteSourceButton("Delete source files",
                     juce::DrawableButton::ButtonStyle::ImageOnButtonBackground)
{
    heading.setText("Source Manager", juce::dontSendNotification);
    heading.setFont(juce::Font(24.0f, juce::Font::bold));
    addAndMakeVisible(&heading);

    sourceList.onObjectAdded = [this](Source s) {
        refreshView();
    };
    sourceList.onObjectRemoved = [this](Source s) {
        refreshView();
    };

    refreshView();

    // table settings
    addAndMakeVisible(table);
    table.setColour(juce::ListBox::outlineColourId, juce::Colours::grey);
    table.setOutlineThickness(1);
    table.setMultipleSelectionEnabled(false);

    table.getHeader().addColumn("ID", Columns::id, 100, 50, 400);
    table.getHeader().addColumn("File name", Columns::name, 100, 50, 400);
    table.getHeader().addColumn("File path", Columns::path, 300, 50, 400);
    table.getHeader().addColumn("File length", Columns::length, 100, 50, 400);
    numRows = sourceList.getObjects().size(); // sets no. of rows for table

    addSourceButton.setImages(&crossIcon);
    addAndMakeVisible(&addSourceButton);
    addSourceButton.onClick = [this] {
        selectNewSourceFile();
    };

    deleteSourceButton.setImages(&dashIcon);
    addAndMakeVisible(&deleteSourceButton);
    deleteSourceButton.onClick = [this] {
        deleteSource();
    };
}

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
    headerContainer.items.add(juce::FlexItem(addSourceButton)
                                  .withHeight(24.0f)
                                  .withWidth(24.0f)
                                  .withMargin(juce::FlexItem::Margin(5.0f)));
    headerContainer.items.add(juce::FlexItem(deleteSourceButton)
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

    auto source = sourceList.getObjects()[rowNumber];
    juce::String cellText;
    switch(columnId) {
    case Columns::id:
        cellText = source.getId().toString();
        break;
    case Columns::name:
        cellText = source.getFileName();
        break;
    case Columns::path:
        cellText = source.getFile().getFullPathName();
        break;
    case Columns::length:
        cellText = juce::String(source.getFileLengthInSeconds());
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

void SourceManager::backgroundClicked(const juce::MouseEvent &)
{
    table.deselectAllRows();
}

// Private methods
void SourceManager::selectNewSourceFile()
{
    juce::FileChooser fileChooser(
        "Load an audio file to use as a source",
        juce::File::getSpecialLocation(juce::File::userHomeDirectory),
        "*.wav,*.aif,*.aiff");

    if(fileChooser.browseForFileToOpen()) {
        auto selectedFile = fileChooser.getResult();
        addSource(selectedFile);
    }
}

void SourceManager::addSource(juce::File &file)
{
    try {
        Source newSource(file);
        sourceList.addObject(newSource);
    } catch(const std::exception &e) {
        /* catches:
        - InvalidFilePath
        - InvalidAudioFile
        - ObjectAlreadyExists
        - FileAlreadyExists */
        std::make_shared<ErrorMessageModal>(juce::String(e.what()));
        return;
    }
}

void SourceManager::deleteSource()
{
    auto index = table.getLastRowSelected();
    auto sourceToRemove = sourceList.getObjects()[index];

    try {
        sourceList.removeObject(sourceToRemove);
    } catch(const std::exception &e) {
        // catches: ObjectNotFound and ObjectInUse
        std::make_shared<ErrorMessageModal>(juce::String(e.what()));
        return;
    }
}

void SourceManager::refreshView()
{
    numRows = sourceList.getObjects().size();
    table.updateContent();
}
