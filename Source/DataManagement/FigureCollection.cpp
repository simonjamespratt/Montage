#include "FigureCollection.h"

#include "CollectionHelper.h"
#include "StateChecker.h"

FigureCollection::FigureCollection(const juce::ValueTree &v) : state(v)
{
    StateChecker::checkTypeIsValid(state, IDs::FIGURES);
    StateChecker::checkChildTypesAreValid(state, IDs::FIGURE);
}

const std::vector<Figure> FigureCollection::getFigures()
{
    std::vector<Figure> figures;

    for(int i = 0; i < state.getNumChildren(); i++) {
        auto figureState = state.getChild(i);
        figures.push_back(Figure(figureState));
    }

    return figures;
}

Figure FigureCollection::createFigure()
{
    int newId = CollectionHelper::createIdForNewChild(state);

    juce::ValueTree newFigureState(IDs::FIGURE);
    newFigureState.setProperty(IDs::id, newId, nullptr);
    state.appendChild(newFigureState, nullptr);

    return Figure(newFigureState);
}
