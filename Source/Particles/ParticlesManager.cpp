#include "ParticlesManager.h"

#include "ErrorMessageModal.h"
#include "Icons.h"

ParticlesManager::ParticlesManager(const ProjectState &ps, te::Engine &eng)
: projectState(ps),
  particleList(ps.getParticleList()),
  addParticleButton("Add particle button",
                    juce::DrawableButton::ButtonStyle::ImageOnButtonBackground),
  sourceSelector(ps.getSourceList()),
  engine(eng)
{
    projectState.onStatusChanged = [this](auto status, auto action) {
        if(action == ProjectState::Action::LoadNewFile) {
            closeEditor();
        }
    };

    heading.setText("Particles Manager", juce::dontSendNotification);
    heading.setFont(juce::Font(24.0f, juce::Font::bold));
    addAndMakeVisible(&heading);

    addParticleButton.setImages(Icons::getIcon(Icons::IconType::Cross).get());
    addParticleButton.onClick = [this] {
        addAndMakeVisible(sourceSelector);
        allowInteractionInViews(false);
        resized();
    };
    addAndMakeVisible(addParticleButton);

    sourceSelector.onSourceChosen = [this](Source s) {
        try {
            Particle newParticle(s);
            particleList.addObject(newParticle);
            sourceSelector.setVisible(false);
            resized();
            openEditor(newParticle);
        } catch(const std::exception &e) {
            /*
            Catches (for Particle::Particle()):
            - ObjectDependencyInvalid
            - ParticleStartInvalid
            - ParticleEndInvalid

            Catches (for list.addObject):
            - ObjectDependencyNotFound,
            - ObjectAlreadyExists
            */
            std::make_shared<ErrorMessageModal>(juce::String(e.what()));
            return;
        }
    };
    sourceSelector.onCancel = [this] {
        sourceSelector.setVisible(false);
        allowInteractionInViews(true);
        resized();
    };

    particlesViewport.setViewedComponent(&particlesContainer, false);
    particlesViewport.setScrollBarsShown(true, false);
    addAndMakeVisible(&particlesViewport);

    refreshView();

    particleList.onObjectAdded = [this](Particle p) {
        refreshView();
    };
    particleList.onObjectRemoved = [this](Particle p) {
        refreshView();
    };
}

void ParticlesManager::resized()
{
    auto area = getLocalBounds();
    auto headerArea = area.removeFromTop(50);
    juce::FlexBox headerContainer;
    headerContainer.justifyContent = juce::FlexBox::JustifyContent::flexStart;
    headerContainer.alignContent = juce::FlexBox::AlignContent::center;
    headerContainer.items.add(
        juce::FlexItem(heading).withHeight(24.0f).withWidth(200.0f).withMargin(
            juce::FlexItem::Margin(5.0f)));
    headerContainer.items.add(juce::FlexItem(addParticleButton)
                                  .withHeight(24.0f)
                                  .withWidth(24.0f)
                                  .withMargin(juce::FlexItem::Margin(5.0f)));
    headerContainer.performLayout(headerArea);

    if(sourceSelector.isVisible()) {
        sourceSelector.setBounds(area.removeFromTop(45));
    }

    if(particleEditor != nullptr) {
        particleEditor->setBounds(area.removeFromTop(300));
    }

    // Particle views
    particlesViewport.setBounds(area);
    particlesContainer.setBounds(area);

    int componentHeight = 70;

    auto totalContainerHeight =
        particlesContainer.getNumChildComponents() * componentHeight;
    particlesContainer.setSize(particlesContainer.getWidth(),
                               totalContainerHeight);
    auto particlesContainerArea = particlesContainer.getLocalBounds();
    for(auto &particleView : particlesContainer.getChildren()) {
        particleView->setBounds(
            particlesContainerArea.removeFromTop(componentHeight));
    }
}

// Private methods
void ParticlesManager::refreshView()
{
    particlesContainer.removeAllChildren();
    particleViews.clear();
    auto particles = particleList.getObjects();
    for(auto &p : particles) {
        auto view = std::make_unique<ParticleView>(p, engine);
        view->onRemove = [this](Particle p) {
            removeParticle(p);
        };
        view->onEdit = [this](Particle p) {
            openEditor(p);
        };
        particlesContainer.addAndMakeVisible(*view);
        particleViews.emplace_back(std::move(view));
    }

    resized();
}

void ParticlesManager::removeParticle(Particle p)
{
    try {
        particleList.removeObject(p);
    } catch(const std::exception &e) {
        // Catches: ObjectNotFound, ObjectInUse
        std::make_shared<ErrorMessageModal>(juce::String(e.what()));
        return;
    }
}

void ParticlesManager::openEditor(Particle p)
{
    jassert(particleEditor == nullptr);

    particleEditor = std::make_unique<ParticleEditor>(p, engine);
    particleEditor->onDoneEditing = [this] {
        closeEditor();
    };
    addAndMakeVisible(*particleEditor);
    particleEditor->setFocus();
    resized();

    allowInteractionInViews(false);
    addParticleButton.setVisible(false);
}

void ParticlesManager::closeEditor()
{
    particleEditor = nullptr;
    resized();

    allowInteractionInViews(true);
    addParticleButton.setVisible(true);
}

void ParticlesManager::allowInteractionInViews(bool isAllowed)
{
    for(auto &view : particleViews) {
        view->allowInteraction(isAllowed);
    }
}
