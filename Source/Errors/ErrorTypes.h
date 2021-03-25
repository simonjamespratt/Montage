#pragma once

#include <stdexcept>
#include <string>

// Generic data object types ==================================================
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

// Source specific types ======================================================
class InvalidFilePath : public std::invalid_argument {
  public:
    InvalidFilePath();
};

class InvalidAudioFile : public std::invalid_argument {
  public:
    InvalidAudioFile();
};

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
