#include "AppState.h"

AppState::AppState(const juce::ValueTree &v) : state(v)
{}

bool AppState::saveStateToFile(const juce::File &file)
{
    const juce::TemporaryFile temp(file);
    juce::FileOutputStream os(temp.getFile());
    std::unique_ptr<juce::XmlElement> xml(state.createXml());
    xml->writeTo(os, juce::XmlElement::TextFormat());
    return temp.overwriteTargetFileWithTemporary();
}

juce::ValueTree AppState::loadStateFromFile(const juce::File &file)
{
    std::unique_ptr<juce::XmlElement> xml(juce::XmlDocument::parse(file));
    return juce::ValueTree::fromXml(*xml);
}
