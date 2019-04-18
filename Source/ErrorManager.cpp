/*
  ==============================================================================

    ErrorManager.cpp
    Created: 24 Apr 2019 5:06:10pm
    Author:  Simon Pratt

  ==============================================================================
*/

#include "ErrorManager.h"

ErrorManager::ErrorManager(ErrorType et)
{
    message = getMessage(et);
    DialogWindow::LaunchOptions options;
    options.content.setOwned(new ErrorMessage(message));
    options.content->setSize(400, 200);
    options.dialogTitle = "Error message";
    options.resizable = false;

    options.launchAsync();
}

ErrorManager::~ErrorManager()
{
}

String ErrorManager::getMessage(ErrorType errorType)
{
    switch (errorType)
    {
    case FileInvalid:
        return "The file selected is invalid.";
    case FileAlreadyExists:
        return "This file already exists as a source.";
    case SourceInvalid:
        return "The source in use is invalid";
    case ParticleRangeInvalid:
        return "The range for this particle is invalid";
    case DeleteSourceInvalidSourceInUse:
        return "Cannot delete source as it is currently being used by a particle";
    default:
        break;
    }
}
