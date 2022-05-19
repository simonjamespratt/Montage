#include "Figures.h"

#include "Icons.h"

Figures::Figures(te::Engine &e, ProjectState &ps)
: engine(e),
  projectState(ps),
  figureList(ps.getFigureList()),
  figuresTable(ps),
  particleList(ps.getParticleList()),
  addFigureButton("Add figure",
                  juce::DrawableButton::ButtonStyle::ImageOnButtonBackground),
  deleteFigureButton("Delete figure",
                     juce::DrawableButton::ButtonStyle::ImageOnButtonBackground)
{
    projectState.onStatusChanged = [this](auto status, auto action) {
        if(action == ProjectState::Action::LoadProject ||
           action == ProjectState::Action::CreateProject) {
            sequencer = nullptr;
            eventsTable.clear();
        }
    };

    figureList.onObjectRemoved = [this](Figure f) {
        try {
            projectState.deleteFileForFigure(f);
        } catch(const std::exception &e) {
            std::cerr << e.what() << '\n';
        }
    };

    figureList.onObjectUpdated = [this](Figure f, juce::Identifier prop) {
        if(prop == IDs::is_generated) {
            figuresTable.setSelectedFigure(f);
        }
    };

    particleList.onObjectAdded = [this](Particle p) {
        setAddButtonEnabled();
    };
    particleList.onObjectRemoved = [this](Particle p) {
        setAddButtonEnabled();
    };

    addFigureButton.setImages(Icons::getIcon(Icons::IconType::Cross).get());
    addAndMakeVisible(addFigureButton);
    setAddButtonEnabled();
    addFigureButton.onClick = [this] {
        showFigGenModal();
    };

    deleteFigureButton.setImages(Icons::getIcon(Icons::IconType::Dash).get());
    addAndMakeVisible(deleteFigureButton);
    deleteFigureButton.setEnabled(false);
    deleteFigureButton.onClick = [this] {
        sequencer = nullptr;
        eventsTable.clear();
        figuresTable.removeFigure();
    };

    addAndMakeVisible(&eventsTable);

    addAndMakeVisible(&figuresTable);
    figuresTable.onFigureDeselected = [this] {
        sequencer = nullptr;
        eventsTable.clear();
        deleteFigureButton.setEnabled(false);
    };
    figuresTable.onFigureSelected = [this](Figure f) {
        try {
            deleteFigureButton.setEnabled(true);
            auto figureFile =
                projectState.getFileForFigure(f); // this might throw error
            sequencer = std::make_unique<Sequencer>(
                Sequencer::createEdit(f, projectState, figureFile, engine),
                f,
                projectState);
            addAndMakeVisible(*sequencer);
            eventsTable.setData(f, projectState);
            resized();
        } catch(const std::exception &e) {
            std::cerr << e.what() << '\n';
        }
    };

    heading.setText("Figures", juce::dontSendNotification);
    heading.setFont(juce::Font(24.0f, juce::Font::bold));
    addAndMakeVisible(&heading);
}

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

    headerContainer.items.add(juce::FlexItem(addFigureButton)
                                  .withHeight(24.0f)
                                  .withWidth(24.0f)
                                  .withMargin(juce::FlexItem::Margin(5.0f)));
    headerContainer.items.add(juce::FlexItem(deleteFigureButton)
                                  .withHeight(24.0f)
                                  .withWidth(24.0f)
                                  .withMargin(juce::FlexItem::Margin(5.0f)));

    headerContainer.performLayout(headingArea);

    auto heightUnit = area.getHeight() / 3;

    auto managerArea = area.removeFromTop(heightUnit);
    auto sequencerArea = area;

    figuresTable.setBounds(
        managerArea.removeFromLeft(managerArea.getWidth() / 2));
    eventsTable.setBounds(managerArea);

    if(sequencer) {
        sequencer->setBounds(sequencerArea.reduced(margin));
    }
}

// Private methods
void Figures::showFigGenModal()
{
    juce::DialogWindow::LaunchOptions o;
    o.content.setOwned(new FigureGenerator(projectState));
    o.content->setSize(700, 600);
    o.dialogBackgroundColour = juce::Colours::darkgrey;
    o.dialogTitle = "Figure Generator";
    o.launchAsync();
}

void Figures::setAddButtonEnabled()
{
    auto numParticles = particleList.getObjects().size();
    if(numParticles > 1) {
        addFigureButton.setEnabled(true);
        addFigureButton.setTooltip("");
    } else {
        addFigureButton.setEnabled(false);
        addFigureButton.setTooltip(
            "At least 2 particles need to exist to create figures");
    }
}
