#pragma once

// #include "Identifiers.h"

#include <Range.hpp>
#include <juce_data_structures/juce_data_structures.h>

class MyTestClass {
  public:
    MyTestClass();
    ~MyTestClass();

    aleatoric::Range someMethod(juce::ValueTree &v);

  private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MyTestClass)
};
