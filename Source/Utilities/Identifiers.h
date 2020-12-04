/*
  ==============================================================================

    IUtilitiesdentifiers.h
    Created: 20 Apr 2019 12:07:58pm
    Author:  Simon Pratt

  ==============================================================================
*/

#pragma once
#include <juce_core/juce_core.h>

static juce::Identifier appStateIdentifier("AppState");

// Sources
static juce::Identifier sourcesIdentifier("Sources");
static juce::Identifier sourceIdentifier("Source");
static juce::Identifier sourcePropIdIdentifier("id");
static juce::Identifier sourcePropFilePathIdentifier("filePath");
static juce::Identifier sourcePropFileNameIdentifier("fileName");

// Particles
static juce::Identifier particlesIdentifier("Particles");
static juce::Identifier particleIdentifier("Particle");
static juce::Identifier particlePropIdIdentifier("id");
static juce::Identifier particlePropNameIdentifier("name");
static juce::Identifier particlePropSourceIdIdentifier("sourceId");
static juce::Identifier particlePropRangeStartIdentifier("rangeStart");
static juce::Identifier particlePropRangeEndIdentifier("rangeEnd");

// Figures
static juce::Identifier figuresIdentifier("Figures");
static juce::Identifier figureIdentifier("Figure");
static juce::Identifier figurePropIdIdentifier("figureId");
static juce::Identifier figureEventIdentifier("FigureEvent");
static juce::Identifier figureEventPropIdIdentifier("figureEventId");
static juce::Identifier figureEventPropOnsetIdentifier("onset");
static juce::Identifier figureEventPropParticleIdIdentifier("particleId");
