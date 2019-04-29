/*
  ==============================================================================

    FileManager.cpp
    Created: 23 Apr 2019 7:58:42pm
    Author:  Simon Pratt

  ==============================================================================
*/
#include "../JuceLibraryCode/JuceHeader.h"
#include "FileManager.h"

FileManager::FileManager()
{
}

FileManager::~FileManager()
{
}

void FileManager::chooseFile()
{
    // TODO: does this need to be a shared pointer?
    auto fileChooser = std::make_shared<FileChooser>(
        "Load an audio file to use as a source",
        File::getSpecialLocation(File::userHomeDirectory),
        "*.wav,*.aif,*.aiff");

    if (fileChooser->browseForFileToOpen())
    {
        file = fileChooser->getResult();
        audioFile = te::AudioFile(file);
    }
}

bool FileManager::fileIsValid()
{
    if (!file.existsAsFile() || !audioFile.isValid())
    {
        return false;
    }

    return true;
}

ValueTree FileManager::addSourceToState(ValueTree &sources)
{
    ValueTree source(sourceIdentifier);
    source.setProperty(sourcePropIdIdentifier, getNewSourceId(sources), nullptr);
    source.setProperty(sourcePropFilePathIdentifier, file.getFullPathName(), nullptr);
    source.setProperty(sourcePropFileNameIdentifier, file.getFileNameWithoutExtension(), nullptr);
    sources.addChild(source, -1, nullptr);
    return source;
}

void FileManager::loadExistingSourceFile(ValueTree &source)
{
    file = File(source[sourcePropFilePathIdentifier]);
    audioFile = te::AudioFile(file);
}

File FileManager::getFile()
{
    return file;
}

te::AudioFile FileManager::getAudioFile()
{
    return audioFile;
}

int FileManager::getNewSourceId(ValueTree &sources)
{
    int highestNumberId = 0;

    for (int i = 0; i < sources.getNumChildren(); i++)
    {
        int currentId = sources.getChild(i).getProperty(sourcePropIdIdentifier);
        if (currentId > highestNumberId)
        {
            highestNumberId = currentId;
        }
    }

    return highestNumberId + 1;
}
