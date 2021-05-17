#include "Figures.h"

Figures::Figures(te::Engine &e, ProjectState &ps)
: engine(e), projectState(ps), sequencer(engine), figureGenerator(ps)
{
    projectState.onStatusChanged = [this](auto status, auto action) {
        if(action == ProjectState::Action::LoadNewFile) {
            sequencer.clear();
            figureManager.clear();
        }
    };

    addAndMakeVisible(&toggleGeneratOutputButton);
    toggleGeneratOutputButton.setButtonText("Show Output");
    toggleGeneratOutputButton.onClick = [this] {
        toggleGenerateManagerState();
    };

    addAndMakeVisible(&figureGenerator);
    addChildComponent(&figureManager);
    addChildComponent(&sequencer);

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
    heading.setBounds(headingArea.removeFromLeft(100));
    toggleGeneratOutputButton.setBounds(
        headingArea.removeFromLeft(150).reduced(margin));

    auto heightUnit = area.getHeight() / 3;

    auto generatorArea = area;
    auto managerArea = area.removeFromTop(heightUnit);
    auto sequencerArea = area;

    figureGenerator.setBounds(generatorArea);
    figureManager.setBounds(managerArea);
    sequencer.setBounds(sequencerArea);
}

void Figures::arrangeFigure(Figure f)
{
    sequencer.readFigure(f, projectState);
    figureManager.setData(f, projectState);
    toggleGenerateManagerState();
}

void Figures::toggleGenerateManagerState()
{
    showGenerator = !showGenerator;

    if(showGenerator) {
        figureManager.setVisible(false);
        sequencer.setVisible(false);
        figureGenerator.setVisible(true);
        toggleGeneratOutputButton.setButtonText("Show output");

    } else {
        figureGenerator.setVisible(false);
        figureManager.setVisible(true);
        sequencer.setVisible(true);
        toggleGeneratOutputButton.setButtonText("Show generator");
    }
}
