#include "Particles.h"

Particles::Particles(te::Engine &eng, juce::ValueTree &as)
: engine(eng),
  appState(as),
  crossIcon(icons.getIcon(Icons::IconType::Cross)),
  addParticleButton("Add particle button",
                    juce::DrawableButton::ButtonStyle::ImageOnButtonBackground)
{
    heading.setText("Particles", juce::dontSendNotification);
    heading.setFont(juce::Font(24.0f, juce::Font::bold));
    addAndMakeVisible(&heading);

    addParticleButton.setImages(&crossIcon);
    addAndMakeVisible(&addParticleButton);
    addParticleButton.onClick = [this] {
        addParticle();
    };

    recalculateSize();
}

Particles::~Particles()
{}

void Particles::paint(juce::Graphics &g)
{}

void Particles::resized()
{
    auto area = getLocalBounds();
    auto headerArea = area.removeFromTop(headerHeight);
    juce::FlexBox headerContainer;
    headerContainer.justifyContent = juce::FlexBox::JustifyContent::flexStart;
    headerContainer.alignContent = juce::FlexBox::AlignContent::center;
    headerContainer.items.add(
        juce::FlexItem(heading).withHeight(24.0f).withWidth(100.0f).withMargin(
            juce::FlexItem::Margin(5.0f)));
    headerContainer.items.add(juce::FlexItem(addParticleButton)
                                  .withHeight(24.0f)
                                  .withWidth(24.0f)
                                  .withMargin(juce::FlexItem::Margin(5.0f)));
    headerContainer.performLayout(headerArea);

    for(int i = 0; i < particles.size(); i++) {
        particles[i]->setBounds(area.removeFromTop(300));
    }
}

void Particles::changeListenerCallback(juce::ChangeBroadcaster *source)
{
    // find out which particle did this
    for(auto it = particles.begin(); it != particles.end(); it++) {
        if((*it)->readyToBeDeleted()) {
            // delete the object and remove from array
            particles.erase(it--);
        }
    }

    refreshView();
}

void Particles::addParticle()
{
    particles.emplace_back(
        std::make_unique<ParticleSelector>(engine, appState));
    addListeners();
    refreshView();
}

void Particles::recalculateSize()
{
    headerHeight = 50;
    auto totalHeight = headerHeight;

    for(int i = 0; i < particles.size(); i++) {
        totalHeight += 300;
    }

    setSize(getWidth(), totalHeight);
}

void Particles::refreshView()
{
    for(int i = 0; i < particles.size(); i++) {
        addAndMakeVisible(*particles[i]);
    }

    recalculateSize();
}

void Particles::addListeners()
{
    for(int i = 0; i < particles.size(); i++) {
        particles[i]->addChangeListener(this);
    }
}
