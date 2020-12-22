#include "NumberProtocolConfig.h"

#include <algorithm>

NumberProtocolConfig::NumberProtocolConfig(
    int id, juce::String name, aleatoric::NumberProtocol::Type protocolType)
: m_id(id), m_name(name), m_protocolType(protocolType)
{}

int NumberProtocolConfig::getId() const
{
    return m_id;
}

juce::String NumberProtocolConfig::getName() const
{
    return m_name;
}

aleatoric::NumberProtocol::Type NumberProtocolConfig::getProtocolType() const
{
    return m_protocolType;
}

// Static stuff
std::vector<NumberProtocolConfig> NumberProtocolConfig::configs {
    NumberProtocolConfig(
        1, "Adjacent Steps", aleatoric::NumberProtocol::Type::adjacentSteps),
    NumberProtocolConfig(2, "Basic", aleatoric::NumberProtocol::Type::basic),
    NumberProtocolConfig(3, "Cycle", aleatoric::NumberProtocol::Type::cycle),
    NumberProtocolConfig(
        4, "Granular Walk", aleatoric::NumberProtocol::Type::granularWalk),
    NumberProtocolConfig(5,
                         "Grouped Repetition",
                         aleatoric::NumberProtocol::Type::groupedRepetition),
    NumberProtocolConfig(
        6, "No Repetition", aleatoric::NumberProtocol::Type::noRepetition),
    NumberProtocolConfig(
        7, "Periodic", aleatoric::NumberProtocol::Type::periodic),
    NumberProtocolConfig(
        8, "Precision", aleatoric::NumberProtocol::Type::precision),
    NumberProtocolConfig(9, "Ratio", aleatoric::NumberProtocol::Type::ratio),
    NumberProtocolConfig(10, "Serial", aleatoric::NumberProtocol::Type::serial),
    NumberProtocolConfig(11, "Subset", aleatoric::NumberProtocol::Type::subset),
    NumberProtocolConfig(12, "Walk", aleatoric::NumberProtocol::Type::walk)};

std::vector<NumberProtocolConfig> NumberProtocolConfig::getConfigurations()
{
    return configs;
}

NumberProtocolConfig NumberProtocolConfig::findByProtocolType(
    const aleatoric::NumberProtocol::Type &type)
{
    auto it = std::find_if(configs.begin(),
                           configs.end(),
                           [&type](const NumberProtocolConfig &config) {
                               return config.getProtocolType() == type;
                           });
    jassert(it != configs.end());
    return *it;
}

NumberProtocolConfig NumberProtocolConfig::findById(const int &id)
{
    auto it = std::find_if(configs.begin(),
                           configs.end(),
                           [&id](const NumberProtocolConfig &config) {
                               return config.getId() == id;
                           });
    jassert(it != configs.end());
    return *it;
}
