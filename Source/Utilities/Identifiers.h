#pragma once
#include <juce_core/juce_core.h>

namespace IDs {
#define DECLARE_ID(name) const juce::Identifier name(#name);

// Top level
DECLARE_ID(PROJECT_STATE)

// STATUS
DECLARE_ID(PROJECT_STATE_STATUS)
DECLARE_ID(project_directory_filepath)
DECLARE_ID(has_unsaved_changes)
DECLARE_ID(figure_edit_has_unsaved_changes)
DECLARE_ID(has_project_loaded)
DECLARE_ID(action)

// Reused types
DECLARE_ID(id) // source, particle, figure and event all have ids
DECLARE_ID(name)

// SOURCES
DECLARE_ID(SOURCE)
DECLARE_ID(file_path)
DECLARE_ID(file_name)
DECLARE_ID(file_length)

// PARTICLES
DECLARE_ID(PARTICLE)
DECLARE_ID(source_id)
DECLARE_ID(start)
DECLARE_ID(end)

// FIGURES
DECLARE_ID(FIGURE)

// EVENT
DECLARE_ID(EVENT)
DECLARE_ID(onset)
DECLARE_ID(particle_id)
DECLARE_ID(figure_id)

// SEQUENCER
DECLARE_ID(SEQUENCER_VIEW_STATE)
DECLARE_ID(viewStateFigureName)
DECLARE_ID(sequencerLoaded)
DECLARE_ID(trackHeight)
DECLARE_ID(trackHeightMin)
DECLARE_ID(trackHeightMax)
DECLARE_ID(panelsWidth)
DECLARE_ID(panelsWidthMin)
DECLARE_ID(panelsWidthMax)
DECLARE_ID(timeScalingFactorInitial)
DECLARE_ID(timeScalingFactorCurrent)
DECLARE_ID(timeScalingFactorMin)
DECLARE_ID(timeScalingFactorMax)
DECLARE_ID(viewportSyncToMouseRequired)
DECLARE_ID(viewportSyncToTransportRequired)

#undef DECLARE_ID
} // namespace IDs
