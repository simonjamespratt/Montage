#include "MainComponent.h"

#include "Identifiers.h"

// constructor
MainComponent::MainComponent()
: projectState(juce::ValueTree(IDs::PROJECT_STATE)),
  contentContainer(projectState)
{
    // TODO: DATA-MANAGEMENT: temporary measure until data saving and loading is
    // possible
    if(PRE_POPULATE_DUMMY_DATA) {
        // Sources
        juce::String pathBase(AUDIO_ASSETS_DIR);
        auto sourceOnePath = pathBase + "/brick.wav";
        auto sourceTwoPath = pathBase + "/click.wav";
        auto sourceThreePath = pathBase + "/glass.wav";

        auto sList = projectState.getSourceList();
        jassert(sList.getObjects().size() == 0);

        juce::File s1File(sourceOnePath);
        juce::File s2File(sourceTwoPath);
        juce::File s3File(sourceThreePath);
        Source s1(s1File);
        Source s2(s2File);
        Source s3(s3File);

        sList.addObject(s1);
        sList.addObject(s2);
        sList.addObject(s3);

        jassert(sList.getObjects().size() == 3);

        // Particles
        auto pList = projectState.getParticleList();
        jassert(pList.getObjects().size() == 0);

        Particle p1(s1);
        p1.setStart(0.16);
        p1.setEnd(0.55);

        Particle p2(s2);
        p2.setStart(0.0);
        p2.setEnd(0.2);

        Particle p3(s3);
        p3.setStart(0.125);
        p3.setEnd(0.29);

        pList.addObject(p1);
        pList.addObject(p2);
        pList.addObject(p3);

        jassert(pList.getObjects().size() == 3);
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
