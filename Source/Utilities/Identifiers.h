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
DECLARE_ID(is_generated)

// FIGURE CREATION SETTINGS
DECLARE_ID(active_type)

// Duration
DECLARE_ID(DURATION_SETTINGS)

DECLARE_ID(GEOMETRIC_SETTINGS)
DECLARE_ID(geo_range_start)
DECLARE_ID(geo_range_end)
DECLARE_ID(geo_collection_size)

DECLARE_ID(PRESCRIBED_SETTINGS)
DECLARE_ID(prescribed_durations)

DECLARE_ID(MULTIPLES_SETTINGS)
DECLARE_ID(multi_strategy)
DECLARE_ID(multi_range_start)
DECLARE_ID(multi_range_end)
DECLARE_ID(multi_multipliers)
DECLARE_ID(multi_base_increment)
DECLARE_ID(multi_deviation_factor)

// Number
DECLARE_ID(NUMBER_SETTINGS)
DECLARE_ID(number_settings_context)

DECLARE_ID(CYCLE_SETTINGS)
DECLARE_ID(cycle_bidirectional)
DECLARE_ID(cycle_reverse_direction)

DECLARE_ID(GRANULAR_WALK_SETTINGS)
DECLARE_ID(granular_walk_deviation_factor)

DECLARE_ID(GROUPED_REPETITION_SETTINGS)
DECLARE_ID(grouped_repetition_groupings)

DECLARE_ID(PERIODIC_SETTINGS)
DECLARE_ID(periodic_chance_of_repetition)

DECLARE_ID(PRECISION_SETTINGS)
DECLARE_ID(precision_distribution)

DECLARE_ID(RATIO_SETTINGS)
DECLARE_ID(ratio_ratios)

DECLARE_ID(SUBSET_SETTINGS)
DECLARE_ID(subset_min)
DECLARE_ID(subset_max)

DECLARE_ID(WALK_SETTINGS)
DECLARE_ID(walk_max_step)

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
