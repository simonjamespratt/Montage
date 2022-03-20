#include "DurationProtocolConfig.h"

#include <algorithm>

DurationProtocolConfig::DurationProtocolConfig(
    int id, juce::String name, DurationProtocolType protocolType)
: m_id(id), m_name(name), m_protocolType(protocolType)
{}

int DurationProtocolConfig::getId() const
{
    return m_id;
}

juce::String DurationProtocolConfig::getName() const
{
    return m_name;
}

DurationProtocolType DurationProtocolConfig::getProtocolType() const
{
    return m_protocolType;
}

// Static stuff
std::vector<DurationProtocolConfig> DurationProtocolConfig::configs {
    DurationProtocolConfig(1, "Geometric", DurationProtocolType::geometric),
    DurationProtocolConfig(2, "Multiples", DurationProtocolType::multiples),
    DurationProtocolConfig(3, "Prescribed", DurationProtocolType::prescribed)};

std::vector<DurationProtocolConfig> DurationProtocolConfig::getConfigurations()
{
    return configs;
}

DurationProtocolConfig DurationProtocolConfig::findById(const int &id)
{
    auto it = std::find_if(configs.begin(),
                           configs.end(),
                           [&id](const DurationProtocolConfig &config) {
                               return config.getId() == id;
                           });
    jassert(it != configs.end());
    return *it;
}

DurationProtocolConfig
DurationProtocolConfig::findByType(const DurationProtocolType &type)
{
    auto it = std::find_if(configs.begin(),
                           configs.end(),
                           [&type](const DurationProtocolConfig &config) {
                               return config.getProtocolType() == type;
                           });
    jassert(it != configs.end());
    return *it;
}
