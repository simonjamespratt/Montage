/*
  ==============================================================================

    NumberGenerators.h
    Created: 30 May 2019 7:39:29pm
    Author:  Simon Pratt

  ==============================================================================
*/

#include <random>

#pragma once

struct RandomSelectionRange
{
    int start;
    int end;
};

class RandomNumber
{
public:
    RandomNumber(int rangeStart, int rangeEnd);
    ~RandomNumber();

    int getNumber();

private:
    std::uniform_int_distribution<int> dist;
};

// Abstract class for derived classes working with ranges, offsets and offering the ability to get a random number
class RandomNumberSelector
{
public:
    RandomNumberSelector(int rangeStart, int rangeEnd);
    virtual ~RandomNumberSelector();

    // NB: removed as pure virtual method because cannot return different types when method has no params
    // virtual int getNumber() = 0;
    virtual void reset() = 0;

protected:
    RandomSelectionRange range;
    int offset;
    int rangeSize;
    virtual void initialize() = 0;

private:
};

// Abstract Base class for derived classes using a discrete distribution
class DiscreteDistributionRandomNumber : public RandomNumberSelector
{
public:
    DiscreteDistributionRandomNumber(int rangeStart, int rangeEnd);
    virtual ~DiscreteDistributionRandomNumber();

protected:
    std::vector<double> discreteDist;
    void setUniformDistribution(double value);
    void setEqualProbability();

private:
};

// Between from RTC
class RandomNumberWithinRange : RandomNumberSelector
{
public:
    RandomNumberWithinRange(int rangeStart, int rangeEnd);
    ~RandomNumberWithinRange();
    int getNumber();
    void reset() override;

protected:
    void initialize() override;

private:
};

// xRandom from RTC
class SeriesRandomNumber : public DiscreteDistributionRandomNumber
{
public:
    SeriesRandomNumber(int rangeStart, int rangeEnd);
    ~SeriesRandomNumber();

    int getNumber();
    void reset() override;

protected:
    void initialize() override;

private:
    bool seriesIsComplete();
};

// random-norep from RTC
class RandomNumberWithoutDirectRepetition : public DiscreteDistributionRandomNumber
{
public:
    RandomNumberWithoutDirectRepetition(int rangeStart, int rangeEnd);
    ~RandomNumberWithoutDirectRepetition();

    int getNumber();
    void reset() override;

protected:
    void initialize() override;

private:
};

// sneak-random from RTC
class RandomNumberAdjacentSteps : public DiscreteDistributionRandomNumber
{
public:
    RandomNumberAdjacentSteps(int rangeStart, int rangeEnd);
    RandomNumberAdjacentSteps(int rangeStart, int rangeEnd, int initialSelection);
    ~RandomNumberAdjacentSteps();

    int getNumber();
    void reset() override;

protected:
    void initialize() override;

private:
    int initialNumberSelection;
    bool hasInitialNumberSelection;
    bool hasMadeFirstSelection;
    void setZeroProbability();
    void setStepDistribution(int lastSelectedNumber);
    bool isRangeStart(int selectedNumber);
    bool isRangeEnd(int selectedNumber);
};

// Periodic from RTC
/*
- a fine-grained random walk with random step size within a max step range
- produces a fraction within the range - hence fine-grained, by scaling up the selectable range and then scaling back down - as a fraction
 */
class PeriodicRandomNumber : public DiscreteDistributionRandomNumber
{
public:
    PeriodicRandomNumber(int rangeStart, int rangeEnd, double chanceOfRepetition);
    PeriodicRandomNumber(int rangeStart, int rangeEnd, double chanceOfRepetition, int initialSelection);
    ~PeriodicRandomNumber();

    int getNumber();
    void reset() override;

protected:
    void initialize() override;

private:
    double periodicity;
    int initialNumberSelection;
    bool hasInitialNumberSelection;
    bool hasMadeFirstSelection;
    void setSingleBiasedDistribution(int currentSelection);
};

// Brownian from RTC
/*
How Brownian RTC works:

function Brownian (min, max, brownFactor) {
    brownFactor (fraction between 0-1)
    const range = max - min
    const offset = min
    const drunkRange = 65000
    drunkStepSize = brownFactor * drunkRange
    drunk(drunkRange, drunkStepSize)
    drunkResult = drunk(drunkRange, drunkStepSize)
    scaledDrunkResult = drunkResult / drunkRange
    resultScaledToRange = scaledDrunkResult * range
    return resultScaledToRange + offset
}

EXAMPLE:
result = Brownian(1, 10, 0.2) ->
currentlySelectedNumber = 32500
range = 10 - 1 = 9;
offset = 1;
drunkRange = 65000;
drunkStepSize = 0.2 * 65000 = 13000;
drunk called with drunk(65000, 13000) // this will choose a number within 13000 of the current number, either side
drunkResult = 39000;
scaledDrunkResult = 39000 / 65000 = 0.6;
resultScaledToRange = 0.6 * 9 = 5.4;
return 5.4 + 1 = 6.4;
 */
class RandomNumberWalkGranular : public RandomNumberSelector
{
public:
    RandomNumberWalkGranular(int rangeStart, int rangeEnd, double deviation);
    RandomNumberWalkGranular(int rangeStart, int rangeEnd, double deviation, int initialSelection);
    ~RandomNumberWalkGranular();

    double getNumber();
    void reset() override;

protected:
    void initialize() override;

private:
    RandomSelectionRange fixedScaleRange;
    int scaleFactor;
    double deviationFactor;
    int maximumStep;
    bool hasMadeFirstSelection;
    bool hasInitialNumberSelection;
    int initialNumberSelection;
    int lastNumberSelected;
    double scaleResultDown();
    int scaleResultUp(int numberToScale);
    double selectStepWithDirection();
};

// Alternative implementation to RTC Brownian:
// A less fine-grained approach, which is often all that is required if you don't need fractions and only need ints within a specified range
class RandomNumberWalk : public RandomNumberSelector
{
public:
    RandomNumberWalk(int rangeStart, int rangeEnd, int maxStep);
    RandomNumberWalk(int rangeStart, int rangeEnd, int maxStep, int initialSelection);
    ~RandomNumberWalk();

    int getNumber();
    void reset() override;

protected:
    void initialize() override;

private:
    int maximumStep;
    bool hasMadeFirstSelection;
    bool hasInitialNumberSelection;
    int initialNumberSelection;
    int lastNumberSelected;
    int selectStepWithDirection();
};
