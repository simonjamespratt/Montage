#pragma once
#include "DurationProtocolParams.h"

#include <juce_gui_basics/juce_gui_basics.h>
#include <vector>

struct DurationProtocolConfig {
    DurationProtocolConfig(int id,
                           juce::String name,
                           DurationProtocolType protocolType);
    int getId() const;
    juce::String getName() const;
    DurationProtocolType getProtocolType() const;

    static std::vector<DurationProtocolConfig> getConfigurations();

    static DurationProtocolConfig findById(const int &id);

    static DurationProtocolConfig findByType(const DurationProtocolType &type);

  private:
    int m_id;
    juce::String m_name;
    DurationProtocolType m_protocolType;

    static std::vector<DurationProtocolConfig> configs;
};
