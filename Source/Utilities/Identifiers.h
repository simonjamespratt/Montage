#pragma once
#include <juce_core/juce_core.h>

namespace IDs {
#define DECLARE_ID(name) const juce::Identifier name(#name);

// Top level
DECLARE_ID(APP_STATE)

// Reused types
DECLARE_ID(id) // source, particle, figure and event all have ids

// SOURCES
DECLARE_ID(SOURCES)

DECLARE_ID(SOURCE)
DECLARE_ID(file_path)
DECLARE_ID(file_name)

// PARTICLES
DECLARE_ID(PARTICLES)

DECLARE_ID(PARTICLE)
DECLARE_ID(source_id)
DECLARE_ID(start)
DECLARE_ID(end)

// FIGURES
DECLARE_ID(FIGURES)

DECLARE_ID(FIGURE)

DECLARE_ID(EVENT)
DECLARE_ID(onset)
DECLARE_ID(particle_id)

#undef DECLARE_ID
} // namespace IDs
