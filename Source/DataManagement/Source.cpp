#include "Source.h"

#include "ErrorTypes.h"
#include "StateService.h"

#include <stdexcept>

Source::Source(const juce::ValueTree &vt) : state(vt)
{
    StateService::checkTypeIsValid(state, IDs::SOURCE);

    std::vector<juce::Identifier> compulsoryProps {IDs::id, IDs::file_path};
    StateService::checkPropsAreValid(state, compulsoryProps);

    processFile(juce::File(state[IDs::file_path]));
}

Source::Source(const juce::File &f)
{
    processFile(f);

    state = juce::ValueTree(IDs::SOURCE);
    state.setProperty(IDs::id, juce::Uuid().toString(), nullptr);
    state.setProperty(IDs::file_path, file.getFullPathName(), nullptr);
}

juce::Uuid Source::getId() const
{
    return juce::Uuid(state[IDs::id]);
}

juce::File Source::getFile() const
{
    return file;
}

juce::String Source::getFileName() const
{
    return file.getFileNameWithoutExtension();
}

double Source::getFileLengthInSeconds() const
{
    return fileLengthInSeconds;
}

juce::ValueTree Source::getState() const
{
    return state;
}

// Private methods
void Source::processFile(juce::File f)
{
    if(!f.existsAsFile()) {
        throw InvalidFilePath();
    }

    juce::AudioFormatManager formatManager;
    formatManager.registerBasicFormats(); // .wav, .aif, .aiff
    std::unique_ptr<juce::AudioFormatReader> reader(
        formatManager.createReaderFor(f));
    if(reader == nullptr) {
        throw InvalidAudioFile();
    }

    file = f;

    fileLengthInSeconds = reader->lengthInSamples / reader->sampleRate;
}
