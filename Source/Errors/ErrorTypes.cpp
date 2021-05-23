#include "ErrorTypes.h"

// Generic data object types ==================================================
ValueTreeInvalidType::ValueTreeInvalidType(std::string identifier)
: std::invalid_argument("Received ValueTree type must be " + identifier)
{}

ValueTreeCompulsoryPropertyMissing::ValueTreeCompulsoryPropertyMissing(
    std::string vtType, std::string vtProp)
: std::invalid_argument("Received ValueTree of type: " + vtType +
                        ", does not have the property: " + vtProp +
                        ". This property is compulsory.")
{}

ValueTreeUnexpectedPropertyReceived::ValueTreeUnexpectedPropertyReceived(
    std::string vtType, std::string vtProp)
: std::invalid_argument("Received ValueTree of type: " + vtType +
                        " has unexpected property: " + vtProp)
{}

ObjectNotFound::ObjectNotFound()
: std::invalid_argument("Object not found in project state")
{}

ObjectInUse::ObjectInUse()
: std::invalid_argument("Another entity in project state relies on this object")
{}

ObjectAlreadyExists::ObjectAlreadyExists()
: std::invalid_argument("Object is already part of project state")
{}

ObjectDependencyNotFound::ObjectDependencyNotFound(
    std::string object, std::string objectItDependsOn)
: std::invalid_argument("The " + objectItDependsOn + " being used by this " +
                        object +
                        " does not exist in the "
                        "project state. Add the " +
                        objectItDependsOn + " to project state first")
{}

ObjectDependencyInvalid::ObjectDependencyInvalid(std::string object,
                                                 std::string objectItDependsOn)
: std::invalid_argument("The " + objectItDependsOn +
                        " provided does not match the one expected by this " +
                        object)
{}

// File management types ======================================================
InvalidFilePath::InvalidFilePath()
: std::invalid_argument(
      "File path does not point to a file that exists or it is a directory")
{}

InvalidAudioFile::InvalidAudioFile()
: std::invalid_argument(
      "File path points to a file that is not an audio file. Must be of "
      "type .wav, .aif or .aiff")
{}

InvalidProjectFile::InvalidProjectFile()
: std::invalid_argument(
      "Project state can only be loaded from and saved to an xml file. In the "
      "case of loading, the file must contain valid xml and a valid "
      "representation of project state data")
{}

ProjectFileNotFound::ProjectFileNotFound()
: std::invalid_argument(
      "Project state does not have a file to save to. Please provide one")
{}

ProjectSaveFailed::ProjectSaveFailed()
: std::invalid_argument("Failed to save project")
{}

// Source specific types ======================================================
FileAlreadyExists::FileAlreadyExists()
: std::invalid_argument(
      "A source already exists within project state with this file and path")
{}

// Particle specific types ====================================================
ParticleStartInvalid::ParticleStartInvalid()
: std::invalid_argument(
      "start must not be less than 0 and must be less than end")
{}

ParticleEndInvalid::ParticleEndInvalid()
: std::invalid_argument("end must be greater than start and must not exceed "
                        "the length of the source file")
{}

// NB: if other entities start having a name, this error type should be made
// more general so it can be reused by them
ParticleNameInvalid::ParticleNameInvalid()
: std::invalid_argument("name must not be empty")
{}

// Event specific types ======================================================
EventOnsetInvalid::EventOnsetInvalid()
: std::invalid_argument("Event onset must be 0 or more")
{}

EventFigureInvalidForEventList::EventFigureInvalidForEventList()
: std::invalid_argument(
      "The Figure being used by this Event does not match the Figure that this "
      "EventList has been configured with")
{}
