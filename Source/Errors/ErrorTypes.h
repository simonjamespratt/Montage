#pragma once

#include <stdexcept>
#include <string>

// Generic data object types ==================================================
class ValueTreeInvalidType : public std::invalid_argument {
  public:
    ValueTreeInvalidType(std::string identifier);
};

class ValueTreeCompulsoryPropertyMissing : public std::invalid_argument {
  public:
    ValueTreeCompulsoryPropertyMissing(std::string vtType, std::string vtProp);
};

class ValueTreeUnexpectedPropertyReceived : public std::invalid_argument {
  public:
    ValueTreeUnexpectedPropertyReceived(std::string vtType, std::string vtProp);
};

class ObjectNotFound : public std::invalid_argument {
  public:
    ObjectNotFound();
};

class ObjectInUse : public std::invalid_argument {
  public:
    ObjectInUse();
};

class ObjectAlreadyExists : public std::invalid_argument {
  public:
    ObjectAlreadyExists();
};

class ObjectDependencyNotFound : public std::invalid_argument {
  public:
    ObjectDependencyNotFound(std::string object, std::string objectItDependsOn);
};

class ObjectDependencyInvalid : public std::invalid_argument {
  public:
    ObjectDependencyInvalid(std::string object, std::string objectItDependsOn);
};

class ObjectNameInvalid : public std::invalid_argument {
  public:
    ObjectNameInvalid(std::string object);
};

// File management types ======================================================
class InvalidFilePath : public std::invalid_argument {
  public:
    InvalidFilePath();
};

class InvalidDirectoryPath : public std::invalid_argument {
  public:
    InvalidDirectoryPath();
};

class InvalidDirectoryForProjectCreation : public std::invalid_argument {
  public:
    InvalidDirectoryForProjectCreation();
};

class InvalidDirectoryForProjectLoad : public std::invalid_argument {
  public:
    InvalidDirectoryForProjectLoad();
};

class InvalidAudioFile : public std::invalid_argument {
  public:
    InvalidAudioFile();
};

class InvalidProjectFile : public std::invalid_argument {
  public:
    InvalidProjectFile();
};

class ProjectDirectoryNotFound : public std::invalid_argument {
  public:
    ProjectDirectoryNotFound();
};

class ProjectFileNotFound : public std::invalid_argument {
  public:
    ProjectFileNotFound();
};

class ProjectSaveFailed : public std::invalid_argument {
  public:
    ProjectSaveFailed();
};

// Source specific types ======================================================
class FileAlreadyExists : public std::invalid_argument {
  public:
    FileAlreadyExists();
};

// Particle specific types ====================================================
class ParticleStartInvalid : public std::invalid_argument {
  public:
    ParticleStartInvalid();
};

class ParticleEndInvalid : public std::invalid_argument {
  public:
    ParticleEndInvalid();
};

// Event specific types ======================================================
class EventOnsetInvalid : public std::invalid_argument {
  public:
    EventOnsetInvalid();
};

class EventFigureInvalidForEventList : public std::invalid_argument {
  public:
    EventFigureInvalidForEventList();
};
