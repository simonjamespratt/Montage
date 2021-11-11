#include "FiguresTable.h"

#include "ErrorMessageModal.h"

FiguresTable::FiguresTable(const ProjectState &ps)
: table({}, this), projectState(ps), figureList(ps.getFigureList())
{
    figureList.onObjectAdded = [this](Figure f) {
        refreshTable();
    };
    figureList.onObjectRemoved = [this](Figure f) {
        refreshTable();
    };

    heading.setText("Generated Figures", juce::dontSendNotification);
    heading.setFont(juce::Font(24.0f, juce::Font::bold));
    addAndMakeVisible(&heading);

    addAndMakeVisible(&table);
    table.setColour(juce::ListBox::outlineColourId, juce::Colours::grey);
    table.setOutlineThickness(1);
    table.setMultipleSelectionEnabled(false);

    table.getHeader().addColumn("Figure Number",
                                Columns::figureNum,
                                100,
                                50,
                                400);
    table.getHeader().addColumn("Name", Columns::name, 100, 50, 400);
    table.getHeader().addColumn("ID", Columns::id, 100, 50, 400);
}

void FiguresTable::resized()
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

int FiguresTable::getNumRows()
{
    return numRows;
}

void FiguresTable::paintRowBackground(juce::Graphics &g,
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

void FiguresTable::paintCell(juce::Graphics &g,
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

    auto figure = figureList.getObjects()[rowNumber];
    juce::String cellText;

    switch(columnId) {
    case Columns::figureNum:
        cellText = juce::String(rowNumber + 1);
        break;
    case Columns::name:
        cellText = figure.getName();
        break;
    case Columns::id:
        cellText = figure.getId().toString();
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

void FiguresTable::backgroundClicked(const juce::MouseEvent &)
{
    table.deselectAllRows();
}

void FiguresTable::selectedRowsChanged(int lastRowSelected)
{
    if(lastRowSelected == -1 && onFigureDeselected) {
        onFigureDeselected();
        return;
    }

    if(onFigureSelected) {
        onFigureSelected(figureList.getObjects()[lastRowSelected]);
    }
}

void FiguresTable::setSelectedFigure(Figure f)
{
    auto index = figureList.getIndex(f);
    jassert(index != -1);
    table.selectRow(index);
}

void FiguresTable::removeFigure()
{
    auto index = table.getLastRowSelected();

    if(index == -1) {
        return;
    }

    try {
        auto figure = figureList.getObjects()[index];
        auto eventList = projectState.getEventList(figure);

        for(auto &e : eventList.getObjects()) {
            eventList.removeObject(e);
        }
        figureList.removeObject(figure);

    } catch(const std::exception &e) {
        std::make_shared<ErrorMessageModal>(juce::String(e.what()));
        return;
    }
}

juce::Component *FiguresTable::refreshComponentForCell(
    int rowNumber,
    int columnId,
    bool /*isRowSelected*/,
    juce::Component *existingComponentToUpdate)
{
    if(columnId == Columns::name) {
        auto *textLabel =
            static_cast<EditableCell *>(existingComponentToUpdate);

        if(textLabel == nullptr) {
            textLabel = new EditableCell(*this);
        }

        textLabel->setRowAndColumn(rowNumber, columnId);
        return textLabel;
    }

    jassert(existingComponentToUpdate == nullptr);
    return nullptr;
}

void FiguresTable::setText(const int columnNumber,
                           const int rowNumber,
                           const juce::String &newText)
{
    // NB: access the figure by index in vector returned from
    // figureList.getObjects(). This is not ideal as potentially the order of
    // figures in table may not match that of the order in the vector (for
    // example if sorting had been done in the table), but I cannot see a way to
    // access the data in a cell by row and column within juce. If it turns out
    // this is possible, then a better way would be to get a figure by id (which
    // would need a new method on the ObjectList class to do so)
    if(columnNumber == Columns::name && !newText.isEmpty()) {
        auto figure = figureList.getObjects()[rowNumber];
        figure.setName(newText);
    }
}

juce::String FiguresTable::getText(const int columnNumber,
                                   const int rowNumber) const
{
    if(columnNumber == Columns::name) {
        // see above in setText() re. why access via vector index is not ideal
        auto figure = figureList.getObjects()[rowNumber];
        return figure.getName();
    }

    return juce::String("");
}

// Private methods
void FiguresTable::refreshTable()
{
    auto figures = figureList.getObjects();
    numRows = figures.size();
    table.updateContent();
}

// ======================================================
EditableCell::EditableCell(FiguresTable &ft) : owner(ft)
{
    setEditable(false, true);
}

void EditableCell::mouseDown(const juce::MouseEvent &event)
{
    owner.table.selectRowsBasedOnModifierKeys(row, event.mods, false);

    Label::mouseDown(event);
}

void EditableCell::textWasEdited()
{
    owner.setText(columnId, row, getText());
}

void EditableCell::setRowAndColumn(const int newRow, const int newColumn)
{
    row = newRow;
    columnId = newColumn;
    setText(owner.getText(columnId, row), juce::dontSendNotification);
}
