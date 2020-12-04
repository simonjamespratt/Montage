/*
  ==============================================================================

    ErrorManager.h
    Created: 24 Apr 2019 5:06:10pm
    Author:  Simon Pratt

  ==============================================================================
*/

#pragma once

#include "ErrorMessage.h"

enum ErrorType {
    FileInvalid,
    FileAlreadyExists,
    SourceInvalid,
    ParticleRangeInvalid,
    DeleteSourceInvalidSourceInUse
};

class ErrorManager {
  public:
    ErrorManager(ErrorType et);
    ~ErrorManager();

  private:
    juce::String message;
    juce::String getMessage(ErrorType errorType);
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ErrorManager)
};
