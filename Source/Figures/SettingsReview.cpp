#include "SettingsReview.h"

SettingsReview::SettingsReview(juce::String headingText,
                               juce::Colour textColour)
: heading(headingText), txtColour(textColour)
{
    message = "";
}

void SettingsReview::paint(juce::Graphics &g)
{
    auto area = getLocalBounds();
    g.setColour(txtColour);
    if(heading.isNotEmpty()) {
        g.setFont(18.0f);
        juce::String headText = heading << "\n\n";
        g.drawFittedText(headText,
                         area.removeFromTop(22),
                         juce::Justification::topLeft,
                         1,
                         1.0f);
    }

    g.setFont(juce::Font(14.0f));
    g.drawFittedText(message, area, juce::Justification::topLeft, 100, 1.0f);
}

void SettingsReview::setMessage(const DurationProtocolSettings &settings)
{
    message = "";

    switch(settings.activeType) {
    case DurationProtocolType::geometric:
        message << "ActiveType: Geometric\n";
        message << "Range start: "
                << juce::String(settings.geometric.rangeStart) << "\n";
        message << "Range end: " << juce::String(settings.geometric.rangeEnd)
                << "\n";
        message << "Collection size: "
                << juce::String(settings.geometric.collectionSize);
        break;
    case DurationProtocolType::prescribed: {
        message << "ActiveType: Prescribed\n";
        message << "Durations: ";
        auto durations = settings.prescribed.durations.get();
        message << getCollectionValuesAsText(durations);
        break;
    }
    case DurationProtocolType::multiples:
        message << "ActiveType: Multiples\n";
        message << "Base increment: "
                << juce::String(settings.multiples.baseIncrement) << "\n";
        message << "Deviation factor: "
                << juce::String(settings.multiples.deviationFactor) << "\n";

        if(settings.multiples.strategy == MultiplierStrategy::range) {
            message << "Selection strategy: ranged-based\n";
            message << "Range start: "
                    << juce::String(settings.multiples.rangeStart) << "\n";
            message << "Range end: "
                    << juce::String(settings.multiples.rangeEnd) << "\n";
        }

        if(settings.multiples.strategy == MultiplierStrategy::hand) {
            message << "Selection strategy: by-hand\n";
            message << "Multipliers: ";
            auto multipliers = settings.multiples.multipliers.get();
            message << getCollectionValuesAsText(multipliers);
        }
        break;
    default:
        break;
    }

    repaint();
}

void SettingsReview::setMessage(const NumberProtocolSettings &settings)
{
    message = "";

    switch(settings.activeType) {
    case NumberProtocolType::adjacentSteps:
        message << "Active type: Adjacent steps\n";
        message << "(no params)";
        break;
    case NumberProtocolType::basic:
        message << "Active type: Basic\n";
        message << "(no params)";
        break;
    case NumberProtocolType::cycle: {
        message << "Active type: Cycle\n";
        auto bidirectional = settings.cycle.bidirectional ? "true" : "false";
        auto reverseDir = settings.cycle.reverseDirection ? "true" : "false";
        message << "Bidirectional: " << bidirectional << "\n";
        message << "Reverse direction: " << reverseDir;
        break;
    }
    case NumberProtocolType::granularWalk:
        message << "Active type: Granular Walk\n";
        message << "Deviation factor: "
                << juce::String(settings.granularWalk.deviationFactor);
        break;
    case NumberProtocolType::groupedRepetition: {
        message << "Active type: Grouped Repetition\n";
        message << "Groupings: ";
        auto groupings = settings.groupedRepetition.groupings.get();
        message << getCollectionValuesAsText(groupings);
        break;
    }
    case NumberProtocolType::noRepetition:
        message << "Active type: No Repetition\n";
        message << "(no params)";
        break;
    case NumberProtocolType::periodic:
        message << "Active type: Periodic\n";
        message << "Chance of repetition: "
                << juce::String(settings.periodic.chanceOfRepetition);
        break;
    case NumberProtocolType::precision: {
        message << "Active type: Precision\n";
        auto distribution = settings.precision.distribution.get();
        message << getCollectionValuesAsText(distribution);
        break;
    }
    case NumberProtocolType::ratio: {
        message << "Active type: Ratio\n";
        auto ratios = settings.ratio.ratios.get();
        message << getCollectionValuesAsText(ratios);
        break;
    }
    case NumberProtocolType::serial:
        message << "Active type: Serial\n";
        message << "(no params)";
        break;
    case NumberProtocolType::subset:
        message << "Active type: Subset\n";
        message << "Min: " << juce::String(settings.subset.min) << "\n";
        message << "Max: " << juce::String(settings.subset.max);
        break;
    case NumberProtocolType::walk:
        message << "Active type: Walk\n";
        message << "Max step: " << juce::String(settings.walk.maxStep);
        break;
    default:
        break;
    }

    repaint();
}

void SettingsReview::clear()
{
    message = "";
}

juce::String
SettingsReview::getCollectionValuesAsText(std::vector<int> &collection)
{
    juce::String text;
    for(auto it = collection.begin(); it != collection.end(); it++) {
        text << juce::String(*it);
        if(std::next(it) != collection.end()) {
            text << ", ";
        }
    }
    return text;
}

juce::String
SettingsReview::getCollectionValuesAsText(std::vector<double> &collection)
{
    juce::String text;
    for(auto it = collection.begin(); it != collection.end(); it++) {
        text << juce::String(*it);
        if(std::next(it) != collection.end()) {
            text << ", ";
        }
    }
    return text;
}
