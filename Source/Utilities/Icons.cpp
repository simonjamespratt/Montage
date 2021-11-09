#include "Icons.h"

std::unique_ptr<juce::Drawable> Icons::getIcon(IconType type)
{
    auto filepath = juce::String(ASSETS_DIR) + "/images";

    switch(type) {
    case Cross:
        filepath += "/plus.svg";
        break;
    case Dash:
        filepath += "/minus.svg";
        break;
    case Loop:
        filepath += "/replay.svg";
        break;
    case Pause:
        filepath += "/pause.svg";
        break;
    case Play:
        filepath += "/play-button.svg";
        break;
    case Stop:
        filepath += "/stop.svg";
        break;

    default:
        jassert(false);
        break;
    }

    auto drawable = juce::Drawable::createFromSVGFile(juce::File(filepath));
    drawable->replaceColour(juce::Colours::black, juce::Colours::white);

    return std::move(drawable);
}
