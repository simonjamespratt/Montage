#include "Cursor.h"

Cursor::Cursor()
{
    cursor.setFill(findColour(juce::Label::textColourId));
    addAndMakeVisible(cursor);
}

void Cursor::updatePosition(const double editLength,
                            const double transportPosition)
{
    auto normalisedTransportPosition =
        editLength == 0.0 ? 0.0 : transportPosition / editLength;
    auto area = getLocalBounds().toFloat();
    auto x = area.getWidth() * normalisedTransportPosition;
    cursor.setRectangle(area.withWidth(2.0f).withX(x));
}
