#pragma once
#include "Cursor.h"

#include <functional>
#include <tracktion_engine/tracktion_engine.h>

namespace te = tracktion_engine;

class TransportManager {
  public:
    TransportManager(te::Edit &e, Cursor &c);
    ~TransportManager();
    void startManager();
    std::function<void()> onChange;

  private:
    te::LambdaTimer transportReporter;
    te::Edit &edit;
    Cursor &cursor;
};
