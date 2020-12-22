#include "Figures.h"

#include "ErrorManager.h"

Figures::Figures(te::Engine &e, juce::ValueTree &as)
: engine(e),
  appState(as),
  sequencer(engine, appState),
  figureGenerator(appState),
  figureManager(appState)
{
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

    // TODO: DATA-MANAGEMENT: This is a hack. Ideally this component would
    // listen to a value tree to know a figure has been added and then get a
    // Figure data object and call the sequencer with it.
    figureGenerator.generateButton.onClick = [this] {
        generateAndArrangeFigure();
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

void Figures::generateAndArrangeFigure()
{
    try {
        auto figure = figureGenerator.generateFigure();
        sequencer.readFigure(figure);
        toggleGenerateManagerState();
    } catch(const std::runtime_error &e) {
        // NB: this should really be a subclassed exception specific to number
        // of events specified when generating a figure. This will do for now
        // though
        std::make_shared<ErrorManager>(ErrorType::FigureInvalidNumberOfEvents);
        return;
    }
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
