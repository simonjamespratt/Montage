#include "NumberProtocolSelector.h"

#include "NumberProtocolConfig.h"

NumberProtocolSelector::NumberProtocolSelector(
    std::shared_ptr<aleatoric::CollectionsProducer<Particle>> particleProducer)
: m_particleProducer(particleProducer)
{
    initialise();
}

NumberProtocolSelector::NumberProtocolSelector(
    std::shared_ptr<aleatoric::DurationsProducer> durationsProducer)
: m_durationsProducer(durationsProducer)
{
    initialise();
    m_durationsProducerListenerToken =
        m_durationsProducer->addListenerForParamsChange(
            [this]() { resetParams(); });

    durationsSelectablesSize =
        m_durationsProducer->getSelectableDurations().size();
}

NumberProtocolSelector::~NumberProtocolSelector()
{
    if(m_durationsProducer != nullptr) {
        m_durationsProducer->removeListenerForParamsChange(
            m_durationsProducerListenerToken);
    }
}

void NumberProtocolSelector::paint(juce::Graphics &g)
{}

void NumberProtocolSelector::resized()
{
    auto area = getLocalBounds();
    auto margin = 10;

    auto chooseProtocolArea = area.removeFromTop(45);
    auto protocolColWidth = chooseProtocolArea.getWidth() / 2;
    protocolSelectorLabel.setBounds(
        chooseProtocolArea.removeFromLeft(protocolColWidth).reduced(margin));
    protocolSelector.setBounds(chooseProtocolArea.reduced(margin));

    if(paramsChangedWarningMessage.isVisible()) {
        paramsChangedWarningMessage.setBounds(
            area.removeFromTop(80).reduced(margin));
    }

    auto controlsArea = area;

    if(controller != nullptr) {
        controller->setBounds(controlsArea);
    }
}

void NumberProtocolSelector::resetParams()
{
    if(controller != nullptr) {
        if(m_particleProducer != nullptr) {
            controller->setParams(m_particleProducer->getParams());

            paramsChangedWarningMessage.setText(
                "Number Protocol params have been reset to defaults "
                "because the the number of particles has "
                "changed.",
                juce::dontSendNotification);
            paramsChangedWarningMessage.setVisible(true);
            resized();
            startTimer(5000);
        }

        if(m_durationsProducer != nullptr) {
            auto newSelectablesSize =
                m_durationsProducer->getSelectableDurations().size();

            if(newSelectablesSize != durationsSelectablesSize) {
                controller->setParams(m_durationsProducer->getParams());
                paramsChangedWarningMessage.setText(
                    "Number Protocol params have been reset to defaults "
                    "because the Duration Protocol collection size has "
                    "changed.",
                    juce::dontSendNotification);
                paramsChangedWarningMessage.setVisible(true);
                resized();
                durationsSelectablesSize = newSelectablesSize;
                startTimer(5000);
            }
        }
    }
}

void NumberProtocolSelector::timerCallback()
{
    paramsChangedWarningMessage.setVisible(false);
    resized();
}

// Private methods
void NumberProtocolSelector::initialise()
{
    protocolSelectorLabel.setText("Selection strategy: ",
                                  juce::dontSendNotification);
    addAndMakeVisible(&protocolSelectorLabel);

    addAndMakeVisible(&protocolSelector);
    configureProtocolSelector();
    protocolSelector.onChange = [this] {
        protocolChanged();
    };

    paramsChangedWarningMessage.setColour(juce::Label::outlineColourId,
                                          juce::Colours::orangered);
    paramsChangedWarningMessage.setColour(juce::Label::textColourId,
                                          juce::Colours::orangered);
    addChildComponent(&paramsChangedWarningMessage);

    setInitialActiveProtocol();
}

void NumberProtocolSelector::configureProtocolSelector()
{
    for(auto &&config : NumberProtocolConfig::getConfigurations()) {
        protocolSelector.addItem(config.getName(), config.getId());
    }
}

void NumberProtocolSelector::protocolChanged()
{
    auto id = protocolSelector.getSelectedId();
    auto selectedConfig = NumberProtocolConfig::findById(id);

    if(m_particleProducer != nullptr) {
        m_particleProducer->setProtocol(aleatoric::NumberProtocol::create(
            selectedConfig.getProtocolType()));

        controller =
            NumberProtocolController::create(selectedConfig.getProtocolType(),
                                             m_particleProducer->getParams());
    }

    if(m_durationsProducer != nullptr) {
        m_durationsProducer->setNumberProtocol(
            aleatoric::NumberProtocol::create(
                selectedConfig.getProtocolType()));

        controller =
            NumberProtocolController::create(selectedConfig.getProtocolType(),
                                             m_durationsProducer->getParams());
    }

    controller->attach([this](aleatoric::NumberProtocolParams newParams) {
        updateParams(newParams);
    });

    addAndMakeVisible(*controller);
    resized();
}

void NumberProtocolSelector::updateParams(
    aleatoric::NumberProtocolParams newParams)
{
    if(m_particleProducer != nullptr) {
        m_particleProducer->setParams(newParams);
    }

    if(m_durationsProducer != nullptr) {
        m_durationsProducer->setParams(newParams);
    }
}

void NumberProtocolSelector::setInitialActiveProtocol()
{
    aleatoric::NumberProtocol::Type activeProtocol;

    if(m_particleProducer != nullptr) {
        activeProtocol = m_particleProducer->getParams().getActiveProtocol();
    }

    if(m_durationsProducer != nullptr) {
        activeProtocol = m_durationsProducer->getParams().getActiveProtocol();
    }

    auto config = NumberProtocolConfig::findByProtocolType(activeProtocol);
    protocolSelector.setSelectedId(config.getId(), juce::dontSendNotification);
    protocolChanged();
}
