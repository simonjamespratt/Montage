#include "Figures.h"

#include "Icons.h"

Figures::Figures(te::Engine &e, ProjectState &ps)
: engine(e),
  projectState(ps),
  sequencer(engine),
  figureGenerator(ps),
  figuresTable(ps),
  addFigureButton("Add figure",
                  juce::DrawableButton::ButtonStyle::ImageOnButtonBackground),
  deleteFigureButton("Delete figure",
                     juce::DrawableButton::ButtonStyle::ImageOnButtonBackground)
{
    projectState.onStatusChanged = [this](auto status, auto action) {
        if(action == ProjectState::Action::LoadNewFile) {
            sequencer.clear();
            eventsTable.clear();
        }
    };

    addFigureButton.setImages(Icons::getIcon(Icons::IconType::Cross).get());
    addChildComponent(addFigureButton);
    addFigureButton.onClick = [this] {
        toggleGenerateManagerState();
    };

    deleteFigureButton.setImages(Icons::getIcon(Icons::IconType::Dash).get());
    addChildComponent(deleteFigureButton);
    deleteFigureButton.onClick = [this] {
        sequencer.clear();
        eventsTable.clear();
        figuresTable.removeFigure();
    };

    addAndMakeVisible(&closeGeneratorButton);
    closeGeneratorButton.setButtonText("Close generator");
    closeGeneratorButton.onClick = [this] {
        toggleGenerateManagerState();
    };

    addChildComponent(&figureGenerator);
    addChildComponent(&eventsTable);
    addChildComponent(&sequencer);

    addChildComponent(&figuresTable);
    figuresTable.onFigureDeselected = [this] {
        sequencer.clear();
        eventsTable.clear();
    };
    figuresTable.onFigureSelected = [this](Figure f) {
        sequencer.readFigure(f, projectState);
        eventsTable.setData(f, projectState);
    };

    heading.setText("Figures", juce::dontSendNotification);
    heading.setFont(juce::Font(24.0f, juce::Font::bold));
    addAndMakeVisible(&heading);

    // better to have this callback here than to listen for changes to state, as
    // all the events will not have been created yet when the listener is
    // triggered for having added a new figure to the state. By listening to
    // this, you can be sure all the events will have been created.
    figureGenerator.onFigureGenerated = [this](Figure f) {
        arrangeFigure(f);
    };

    refreshView();
}

Figures::~Figures()
{}

void Figures::paint(juce::Graphics &g)
{}

void Figures::resized()
{
    auto margin = 10;
    auto area = getLocalBounds();

    auto headingArea = area.removeFromTop(50);

    juce::FlexBox headerContainer;
    headerContainer.justifyContent = juce::FlexBox::JustifyContent::flexStart;
    headerContainer.alignContent = juce::FlexBox::AlignContent::center;
    headerContainer.items.add(
        juce::FlexItem(heading).withHeight(24.0f).withWidth(100.0f).withMargin(
            juce::FlexItem::Margin(5.0f)));

    if(showGenerator) {
        headerContainer.items.add(
            juce::FlexItem(closeGeneratorButton)
                .withHeight(24.0f)
                .withWidth(150.0f)
                .withMargin(juce::FlexItem::Margin(5.0f)));

    } else {
        headerContainer.items.add(
            juce::FlexItem(addFigureButton)
                .withHeight(24.0f)
                .withWidth(24.0f)
                .withMargin(juce::FlexItem::Margin(5.0f)));
        headerContainer.items.add(
            juce::FlexItem(deleteFigureButton)
                .withHeight(24.0f)
                .withWidth(24.0f)
                .withMargin(juce::FlexItem::Margin(5.0f)));
    }

    headerContainer.performLayout(headingArea);

    auto heightUnit = area.getHeight() / 3;

    auto generatorArea = area;
    auto managerArea = area.removeFromTop(heightUnit);
    auto sequencerArea = area;

    figureGenerator.setBounds(generatorArea);
    figuresTable.setBounds(
        managerArea.removeFromLeft(managerArea.getWidth() / 2));
    eventsTable.setBounds(managerArea);
    sequencer.setBounds(sequencerArea.reduced(margin));
}

void Figures::arrangeFigure(Figure f)
{
    toggleGenerateManagerState();
    figuresTable.setSelectedFigure(f);
}

// Private methods
void Figures::toggleGenerateManagerState()
{
    showGenerator = !showGenerator;
    refreshView();
}

void Figures::refreshView()
{
    addFigureButton.setVisible(!showGenerator);
    deleteFigureButton.setVisible(!showGenerator);
    figuresTable.setVisible(!showGenerator);
    eventsTable.setVisible(!showGenerator);
    sequencer.setVisible(!showGenerator);

    figureGenerator.setVisible(showGenerator);
    closeGeneratorButton.setVisible(showGenerator);

    resized();
}
