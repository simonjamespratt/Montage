#pragma once
#include <NumberProtocol.hpp>
#include <NumberProtocolParameters.hpp>
#include <juce_gui_basics/juce_gui_basics.h>
#include <vector>

struct NumberProtocolConfig {
    NumberProtocolConfig(int id,
                         juce::String name,
                         aleatoric::NumberProtocol::Type protocolType);
    int getId() const;
    juce::String getName() const;
    aleatoric::NumberProtocol::Type getProtocolType() const;

    static std::vector<NumberProtocolConfig> getConfigurations();

    static NumberProtocolConfig
    findByProtocolType(const aleatoric::NumberProtocol::Type &type);

    static NumberProtocolConfig findById(const int &id);

  private:
    int m_id;
    juce::String m_name;
    aleatoric::NumberProtocol::Type m_protocolType;

    static std::vector<NumberProtocolConfig> configs;
};
