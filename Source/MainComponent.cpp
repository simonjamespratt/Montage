#include "MainComponent.h"

#include "Identifiers.h"

// constructor
MainComponent::MainComponent()
: appState(IDs::APP_STATE), contentContainer(appState)
{
    juce::ValueTree sources(IDs::SOURCES);
    juce::ValueTree particles(IDs::PARTICLES);
    juce::ValueTree figures(IDs::FIGURES);
    appState.addChild(sources, -1, nullptr);
    appState.addChild(particles, -1, nullptr);
    appState.addChild(figures, -1, nullptr);

    // TODO: DATA-MANAGEMENT: temporary measure until data saving and loading is
    // possible
    if(PRE_POPULATE_DUMMY_DATA) {
        // SOURCES
        juce::String pathBase(AUDIO_ASSETS_DIR);

        auto sourceOnePath = pathBase + "/brick.wav";
        juce::ValueTree sourceOne(IDs::SOURCE);
        sourceOne.setProperty(IDs::id, 1, nullptr);
        sourceOne.setProperty(IDs::file_path, sourceOnePath, nullptr);
        sourceOne.setProperty(IDs::file_name, juce::String("brick"), nullptr);
        sources.addChild(sourceOne, -1, nullptr);

        auto sourceTwoPath = pathBase + "/click.wav";
        juce::ValueTree sourceTwo(IDs::SOURCE);
        sourceTwo.setProperty(IDs::id, 2, nullptr);
        sourceTwo.setProperty(IDs::file_path, sourceTwoPath, nullptr);
        sourceTwo.setProperty(IDs::file_name, juce::String("click"), nullptr);
        sources.addChild(sourceTwo, -1, nullptr);

        auto sourceThreePath = pathBase + "/glass.wav";
        juce::ValueTree sourceThree(IDs::SOURCE);
        sourceThree.setProperty(IDs::id, 3, nullptr);
        sourceThree.setProperty(IDs::file_path, sourceThreePath, nullptr);
        sourceThree.setProperty(IDs::file_name, juce::String("glass"), nullptr);
        sources.addChild(sourceThree, -1, nullptr);

        // PARTICLES
        juce::ValueTree particleOne(IDs::PARTICLE);
        particleOne.setProperty(IDs::id, 1, nullptr);
        particleOne.setProperty(IDs::source_id, 1, nullptr);
        particleOne.setProperty(IDs::start, 0.16, nullptr);
        particleOne.setProperty(IDs::end, 0.55, nullptr);
        particles.addChild(particleOne, -1, nullptr);

        juce::ValueTree particleTwo(IDs::PARTICLE);
        particleTwo.setProperty(IDs::id, 2, nullptr);
        particleTwo.setProperty(IDs::source_id, 2, nullptr);
        particleTwo.setProperty(IDs::start, 0.0, nullptr);
        particleTwo.setProperty(IDs::end, 0.2, nullptr);
        particles.addChild(particleTwo, -1, nullptr);

        juce::ValueTree particleThree(IDs::PARTICLE);
        particleThree.setProperty(IDs::id, 3, nullptr);
        particleThree.setProperty(IDs::source_id, 3, nullptr);
        particleThree.setProperty(IDs::start, 0.125, nullptr);
        particleThree.setProperty(IDs::end, 0.29, nullptr);
        particles.addChild(particleThree, -1, nullptr);
    }

    auto screenSize =
        juce::Desktop::getInstance().getDisplays().getMainDisplay().userArea;
    addAndMakeVisible(&contentContainer);

    // Set the size of the component after you add any child components
    setSize(screenSize.getWidth(), screenSize.getHeight());
}

MainComponent::~MainComponent()
{}

void MainComponent::resized()
{
    contentContainer.setBounds(getLocalBounds());
}
