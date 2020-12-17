#include "CollectionHelper.h"

#include "Identifiers.h"

#include <stdexcept>

int CollectionHelper::createIdForNewChild(const juce::ValueTree &state)
{
    auto numChildren = state.getNumChildren();

    if(numChildren < 1) {
        return 1;
    }

    // sort existing ids into order and return highest + 1
    std::vector<int> existingIds;
    for(int i = 0; i < numChildren; i++) {
        auto child = state.getChild(i);
        if(!child.hasProperty(IDs::id)) {
            throw std::invalid_argument(
                "At least one child does not have a property of type IDs::id");
        }

        existingIds.push_back(child[IDs::id]);
    }

    std::sort(existingIds.begin(), existingIds.end());

    return existingIds.back() + 1;
}
