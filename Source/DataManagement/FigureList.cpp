#include "FigureList.h"

#include "Identifiers.h"

FigureList::FigureList(juce::ValueTree vt) : ObjectList<Figure>(vt)
{}

// Private methods
bool FigureList::isSuitableType(const juce::ValueTree &vt) const
{
    return vt.hasType(IDs::FIGURE);
}

Figure FigureList::makeObject(const juce::ValueTree &vt) const
{
    return Figure(vt);
}

juce::ValueTree FigureList::getObjectState(const Figure &figure) const
{
    return figure.getState();
}

void FigureList::validateObjectRemoval(const Figure &figure)
{
    auto figureState = figure.getState();

    auto childUsingSource =
        state.getChildWithProperty(IDs::figure_id, figureState[IDs::id]);

    if(childUsingSource.isValid()) {
        throw ObjectInUse();
    }
}
