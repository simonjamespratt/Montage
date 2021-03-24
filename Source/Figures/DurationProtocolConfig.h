#pragma once
#include "DurationProtocolController.h"

#include <juce_gui_basics/juce_gui_basics.h>
#include <vector>

struct DurationProtocolConfig {
    DurationProtocolConfig(int id,
                           juce::String name,
                           DurationProtocolController::Type protocolType);
    int getId() const;
    juce::String getName() const;
    DurationProtocolController::Type getProtocolType() const;

    static std::vector<DurationProtocolConfig> getConfigurations();

    static DurationProtocolConfig findById(const int &id);

    static DurationProtocolConfig
    findByType(const DurationProtocolController::Type &type);

  private:
    int m_id;
    juce::String m_name;
    DurationProtocolController::Type m_protocolType;

    static std::vector<DurationProtocolConfig> configs;
};
