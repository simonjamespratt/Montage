#include "ErrorManager.h"

ErrorManager::ErrorManager(ErrorType et)
{
    auto message = getMessage(et);
    juce::DialogWindow::LaunchOptions options;
    options.content.setOwned(new ErrorMessage(message));
    options.content->setSize(400, 200);
    options.dialogTitle = "Error message";
    options.resizable = false;

    options.launchAsync();
}

ErrorManager::~ErrorManager()
{}

juce::String ErrorManager::getMessage(ErrorType errorType)
{
    switch(errorType) {
    case FileInvalid:
        return "The file selected is invalid.";
    case FileAlreadyExists:
        return "This file already exists as a source.";
    case SourceInvalid:
        return "The source in use is invalid";
    case ParticleRangeInvalid:
        return "The range for this particle is invalid";
    case DeleteSourceInvalidSourceInUse:
        return "Cannot delete source as it is currently being used by a "
               "particle";
    case FigureInvalidNumberOfEvents:
        return "The number of events specified for a figure must be greater "
               "than 0";
    default:
        break;
    }
}
