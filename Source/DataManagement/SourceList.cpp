#include "SourceList.h"

SourceList::SourceList(juce::ValueTree vt) : ObjectList<Source>(vt)
{}

// Private methods
bool SourceList::isSuitableType(const juce::ValueTree &vt) const
{
    return vt.hasType(IDs::SOURCE);
}

Source SourceList::makeObject(const juce::ValueTree &vt) const
{
    return Source(vt);
}

juce::ValueTree SourceList::getObjectState(const Source &source) const
{
    return source.getState();
}

void SourceList::validateObjectAddition(const Source &source)
{
    auto sourceState = source.getState();

    auto fileExists =
        state.getChildWithProperty(IDs::file_path, sourceState[IDs::file_path]);

    if(fileExists.isValid()) {
        throw FileAlreadyExists();
    }
}

void SourceList::validateObjectRemoval(const Source &source)
{
    auto sourceState = source.getState();

    auto childUsingSource =
        state.getChildWithProperty(IDs::source_id, sourceState[IDs::id]);

    if(childUsingSource.isValid()) {
        throw ObjectInUse();
    }
}
