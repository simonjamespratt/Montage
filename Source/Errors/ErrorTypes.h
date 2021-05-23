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

// File management types ======================================================
class InvalidFilePath : public std::invalid_argument {
  public:
    InvalidFilePath();
};

class InvalidAudioFile : public std::invalid_argument {
  public:
    InvalidAudioFile();
};

class InvalidProjectFile : public std::invalid_argument {
  public:
    InvalidProjectFile();
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

// NB: if other entities start having a name, this error type should be made
// more general so it can be reused by them
class ParticleNameInvalid : public std::invalid_argument {
  public:
    ParticleNameInvalid();
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
