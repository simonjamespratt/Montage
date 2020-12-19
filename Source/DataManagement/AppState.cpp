#include "AppState.h"

AppState::AppState(juce::ValueTree &v) : state(v)
{}

bool AppState::saveStateToFile(const juce::File &file)
{
    const juce::TemporaryFile temp(file);
    juce::FileOutputStream os(temp.getFile());
    std::unique_ptr<juce::XmlElement> xml(state.createXml());
    xml->writeTo(os, juce::XmlElement::TextFormat());
    return temp.overwriteTargetFileWithTemporary();
}

bool AppState::loadStateFromFile(const juce::File &file)
{
    std::unique_ptr<juce::XmlElement> xml(juce::XmlDocument::parse(file));
    state.copyPropertiesAndChildrenFrom(juce::ValueTree::fromXml(*xml),
                                        nullptr);
    return true;
}
