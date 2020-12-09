#include "FileManager.h"

FileManager::FileManager()
{}

FileManager::~FileManager()
{}

void FileManager::chooseFile()
{
    // NB: does this need to be a shared pointer?
    auto fileChooser = std::make_shared<juce::FileChooser>(
        "Load an audio file to use as a source",
        juce::File::getSpecialLocation(juce::File::userHomeDirectory),
        "*.wav,*.aif,*.aiff");

    if(fileChooser->browseForFileToOpen()) {
        file = fileChooser->getResult();
    }
}

bool FileManager::fileIsValid(te::Engine &engine)
{
    te::AudioFile audioFile(engine, file);
    if(!file.existsAsFile() || !audioFile.isValid()) {
        return false;
    }

    return true;
}

juce::ValueTree FileManager::addSourceToState(juce::ValueTree &sources)
{
    juce::ValueTree source(IDs::SOURCE);
    source.setProperty(IDs::id, getNewSourceId(sources), nullptr);
    source.setProperty(IDs::file_path, file.getFullPathName(), nullptr);
    source.setProperty(IDs::file_name,
                       file.getFileNameWithoutExtension(),
                       nullptr);
    sources.addChild(source, -1, nullptr);
    return source;
}

void FileManager::loadExistingSourceFile(juce::ValueTree &source)
{
    file = juce::File(source[IDs::file_path]);
}

juce::File FileManager::getFile()
{
    return file;
}

te::AudioFile FileManager::getAudioFile(te::Engine &engine)
{
    return te::AudioFile(engine, file);
}

int FileManager::getNewSourceId(juce::ValueTree &sources)
{
    int highestNumberId = 0;

    for(int i = 0; i < sources.getNumChildren(); i++) {
        int currentId = sources.getChild(i).getProperty(IDs::id);
        if(currentId > highestNumberId) {
            highestNumberId = currentId;
        }
    }

    return highestNumberId + 1;
}
