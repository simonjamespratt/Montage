#include "MyTestClass.h"

#include "TestIdentifiers.h"

MyTestClass::MyTestClass()
{}

MyTestClass::~MyTestClass()
{}

aleatoric::Range MyTestClass::someMethod(juce::ValueTree &v)
{
    int start = v[rangeStart];
    int end = v[rangeEnd];

    return aleatoric::Range(start, end);
}
