#include "FigureGenerator.h"

#include "FigureProcessor.h"

StageIndicator::StageIndicator()
{}

void StageIndicator::setStage(int stage)
{
    stageIndex = stage - 1;
    repaint();
}

void StageIndicator::paint(juce::Graphics &g)
{
    float circleDiameter = 15.0f;
    float marginX = (circleDiameter / 2) + 1; // +1 for breathing space
    float width = getWidth() - (marginX * 2);
    float centreY = getHeight() / 2;
    float lineThickness = 2;
    float pointSpacing = width / 3;
    juce::Rectangle<float> circleBounds(circleDiameter, circleDiameter);

    g.setColour(juce::Colours::cornflowerblue);
    g.fillRect(marginX, centreY - (lineThickness / 2), width, lineThickness);

    for(int i = 0; i < 4; i++) {
        juce::Point<float> point((pointSpacing * i) + marginX, centreY);
        circleBounds.setCentre(point);

        g.setColour(juce::Colours::darkgrey);
        g.fillEllipse(circleBounds);

        g.setColour(juce::Colours::cornflowerblue);
        if(i == stageIndex) {
            g.fillEllipse(circleBounds);

        } else {
            g.drawEllipse(circleBounds, 2);
        }
    }
}

// =====================================================================

StageDescription::StageDescription(juce::String titleText,
                                   juce::String descriptionText)
: title({}, titleText)
{
    title.setFont(juce::Font(24.0f));
    addAndMakeVisible(title);

    description.setText(descriptionText, juce::dontSendNotification);
    description.setMultiLine(true, true);
    description.setReadOnly(true);
    description.setCaretVisible(false);
    description.setColour(juce::TextEditor::ColourIds::backgroundColourId,
                          juce::Colours::transparentBlack);
    description.setColour(juce::TextEditor::ColourIds::outlineColourId,
                          juce::Colours::transparentWhite);
    addAndMakeVisible(description);
}

void StageDescription::resized()
{
    auto area = getLocalBounds();
    title.setBounds(area.removeFromTop(50));
    description.setBounds(area);
}

// =========================================================================

FigGenStage::FigGenStage(FigureGenerator &fg) : figGen(fg)
{}

FigGenStage::~FigGenStage()
{
    figGen.errorMessage.setVisible(false);
}

// =========================================================================

DurationProtocolStage::DurationProtocolStage(FigureGenerator &fg)
: FigGenStage(fg),
  durationProtocolParams(DurationProtocolController::Type::prescribed),
  durationProtocolSelector(durationProtocolParams)
{
    jassert(figGen.durationsProducer == nullptr);
    addAndMakeVisible(stageDescription);
    addAndMakeVisible(durationProtocolSelector);
    figGen.previousBtn.setVisible(false);
    figGen.stageIndicator.setStage(1);
}

DurationProtocolStage::~DurationProtocolStage()
{
    figGen.previousBtn.setVisible(true);
}

void DurationProtocolStage::resized()
{
    auto area = getLocalBounds();
    stageDescription.setBounds(area.removeFromTop(100));
    durationProtocolSelector.setBounds(area);
}

void DurationProtocolStage::nextStage()
{
    try {
        figGen.durationsProducer =
            std::make_shared<aleatoric::DurationsProducer>(
                DurationProtocolController::createProtocol(
                    durationProtocolParams),
                aleatoric::NumberProtocol::create(
                    aleatoric::NumberProtocol::Type::basic));

        figGen.figGenStage = std::make_unique<DurationSelectionStage>(figGen);
        figGen.addAndMakeVisible(*figGen.figGenStage);
    } catch(const std::exception &e) {
        figGen.errorMessage.setText(e.what(), juce::dontSendNotification);
        figGen.errorMessage.setVisible(true);
        figGen.resized();
    }
}

void DurationProtocolStage::previousStage()
{}

// ===================================================================

DurationSelectionStage::DurationSelectionStage(FigureGenerator &fg)
: FigGenStage(fg), durationNumberProtocolSelector(figGen.durationsProducer)
{
    jassert(figGen.durationsProducer);
    addAndMakeVisible(stageDescription);
    addAndMakeVisible(durationNumberProtocolSelector);
    figGen.stageIndicator.setStage(2);
}

void DurationSelectionStage::resized()
{
    auto area = getLocalBounds();
    stageDescription.setBounds(area.removeFromTop(100));
    durationNumberProtocolSelector.setBounds(area);
}

void DurationSelectionStage::nextStage()
{
    try {
        durationNumberProtocolSelector.updateParams();
        figGen.figGenStage = std::make_unique<ParticleSelectionStage>(figGen);
        figGen.addAndMakeVisible(*figGen.figGenStage);
    } catch(const std::exception &e) {
        figGen.errorMessage.setText(e.what(), juce::dontSendNotification);
        figGen.errorMessage.setVisible(true);
        figGen.resized();
    }
}

void DurationSelectionStage::previousStage()
{
    figGen.durationsProducer = nullptr;
    figGen.figGenStage = std::make_unique<DurationProtocolStage>(figGen);
    figGen.addAndMakeVisible(*figGen.figGenStage);
}

// ===================================================================

ParticleSelectionStage::ParticleSelectionStage(FigureGenerator &fg)
: FigGenStage(fg)
{
    jassert(figGen.particlesProducer == nullptr);
    auto particles = figGen.projectState.getParticleList().getObjects();
    figGen.particlesProducer =
        std::make_shared<aleatoric::CollectionsProducer<Particle>>(
            particles,
            aleatoric::NumberProtocol::create(
                aleatoric::NumberProtocol::Type::basic));

    particleProtocolSelector =
        std::make_unique<NumberProtocolSelector>(figGen.particlesProducer);
    addAndMakeVisible(stageDescription);
    addAndMakeVisible(*particleProtocolSelector);
    figGen.stageIndicator.setStage(3);
}

void ParticleSelectionStage::resized()
{
    auto area = getLocalBounds();
    stageDescription.setBounds(area.removeFromTop(100));
    particleProtocolSelector->setBounds(area);
}

void ParticleSelectionStage::nextStage()
{
    try {
        particleProtocolSelector->updateParams();
        figGen.figGenStage = std::make_unique<NoOfEventsStage>(figGen);
        figGen.addAndMakeVisible(*figGen.figGenStage);

    } catch(const std::exception &e) {
        figGen.errorMessage.setText(e.what(), juce::dontSendNotification);
        figGen.errorMessage.setVisible(true);
        figGen.resized();
    }
}

void ParticleSelectionStage::previousStage()
{
    figGen.particlesProducer = nullptr;
    figGen.figGenStage = std::make_unique<DurationSelectionStage>(figGen);
    figGen.addAndMakeVisible(*figGen.figGenStage);
}

// ===================================================================

NoOfEventsStage::NoOfEventsStage(FigureGenerator &fg)
: FigGenStage(fg), noOfEventsInput(noOfEvents, "Number of events")
{
    jassert(figGen.durationsProducer);
    jassert(figGen.particlesProducer);

    addAndMakeVisible(stageDescription);
    addAndMakeVisible(noOfEventsInput);
    figGen.nextBtn.setButtonText("Generate");
    figGen.stageIndicator.setStage(4);
}

NoOfEventsStage::~NoOfEventsStage()
{
    figGen.nextBtn.setButtonText("Next");
}

void NoOfEventsStage::resized()
{
    auto area = getLocalBounds();
    stageDescription.setBounds(area.removeFromTop(100));
    noOfEventsInput.setBounds(area.removeFromTop(50));
}

void NoOfEventsStage::nextStage()
{
    if(noOfEvents == 0) {
        figGen.errorMessage.setText("The number of events specified for a "
                                    "figure must be greater than 0",
                                    juce::dontSendNotification);
        figGen.errorMessage.setVisible(true);
        figGen.resized();
        return;
    }

    FigureProcessor processor;

    try {
        auto newFigure = processor.composeFigure(noOfEvents,
                                                 *figGen.durationsProducer,
                                                 *figGen.particlesProducer,
                                                 figGen.projectState);

        // close the modal from inside
        if(auto dw = figGen.findParentComponentOfClass<juce::DialogWindow>()) {
            dw->exitModalState(0);
        }
    } catch(const std::exception &e) {
        /*
        Catches:
        - Figure creation - no errors thrown
        - FigureList addition: ObjectAlreadyExists
        - Event creation: EventOnsetInvalid
        - EventList addition:
            - ObjectDependencyNotFound,
            - ObjectAlreadyExists,
            - EventFigureInvalidForEventList
        */
        figGen.errorMessage.setText(e.what(), juce::dontSendNotification);
        figGen.errorMessage.setVisible(true);
        figGen.resized();
        return;
    }
}

void NoOfEventsStage::previousStage()
{
    figGen.particlesProducer = nullptr;
    figGen.figGenStage = std::make_unique<ParticleSelectionStage>(figGen);
    figGen.addAndMakeVisible(*figGen.figGenStage);
}

// ===================================================================

FigureGenerator::FigureGenerator(ProjectState &ps) : projectState(ps)
{
    addAndMakeVisible(stageIndicator);

    nextBtn.onClick = [this] {
        if(figGenStage) {
            figGenStage->nextStage();
            resized();
        }
    };
    addAndMakeVisible(nextBtn);

    previousBtn.onClick = [this] {
        if(figGenStage) {
            figGenStage->previousStage();
            resized();
        }
    };
    addAndMakeVisible(previousBtn);

    // order is important here - this must come after buttons are added to
    // component
    figGenStage = std::make_unique<DurationProtocolStage>(*this);
    addAndMakeVisible(*figGenStage);

    errorMessage.setColour(juce::Label::outlineColourId, juce::Colours::orange);
    errorMessage.setColour(juce::Label::textColourId, juce::Colours::orange);
    addChildComponent(errorMessage);
}

void FigureGenerator::resized()
{
    auto margin = 20;
    auto area = getLocalBounds().reduced(margin);

    // Stage
    stageIndicator.setBounds(area.removeFromTop(20));

    // Buttons
    auto btnsArea = area.removeFromBottom(30);
    auto leftBtnArea = btnsArea.removeFromLeft(100);
    auto rightBtnArea = btnsArea.removeFromRight(100);

    if(nextBtn.isVisible()) {
        nextBtn.setBounds(rightBtnArea);
    }

    if(previousBtn.isVisible()) {
        previousBtn.setBounds(leftBtnArea);
    }

    auto errorArea = area.removeFromBottom(50);

    if(errorMessage.isVisible()) {
        errorMessage.setBounds(errorArea.reduced(0, margin / 2));
    }

    if(figGenStage) {
        figGenStage->setBounds(area);
    }
}
