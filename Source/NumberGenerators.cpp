/*
  ==============================================================================

    NumberGenerators.cpp
    Created: 30 May 2019 7:39:29pm
    Author:  Simon Pratt

  ==============================================================================
*/

#include "NumberGenerators.h"

namespace random_number
{
std::random_device rd;
std::mt19937 engine(rd());
} // namespace random_number

RandomNumber::RandomNumber(int rangeStart, int rangeEnd) : dist(rangeStart, rangeEnd) {}

RandomNumber::~RandomNumber() {}

int RandomNumber::getNumber()
{
    return dist(random_number::engine);
}

// RANDOM NUMBER SELECTOR ==============================================================================
RandomNumberSelector::RandomNumberSelector(int rangeStart, int rangeEnd)
{
    range.start = rangeStart;
    range.end = rangeEnd;
    offset = rangeStart;
    rangeSize = rangeEnd - rangeStart + 1;
}

RandomNumberSelector::~RandomNumberSelector() {}

// DISCRETE DISTRIBUTION RANDOM NUMBER =================================================================
DiscreteDistributionRandomNumber::DiscreteDistributionRandomNumber(int rangeStart, int rangeEnd) : RandomNumberSelector(rangeStart, rangeEnd) {}

DiscreteDistributionRandomNumber::~DiscreteDistributionRandomNumber() {}

void DiscreteDistributionRandomNumber::setUniformDistribution(double value)
{
    discreteDist.clear();
    for (int i = 0; i < rangeSize; i++)
    {
        discreteDist.push_back(value);
    }
}

void DiscreteDistributionRandomNumber::setEqualProbability()
{
    setUniformDistribution(1.0);
}

// RANDOM WITHIN RANGE =====================================================
// range is inclusive, so range is: end - start + 1
RandomNumberWithinRange::RandomNumberWithinRange(int rangeStart, int rangeEnd) : RandomNumberSelector(rangeStart, rangeEnd)
{
}

RandomNumberWithinRange::~RandomNumberWithinRange() {}

int RandomNumberWithinRange::getNumber()
{
    // select a number in range - uniform dist
    RandomNumber randomNumber(range.start, range.end);
    return randomNumber.getNumber();
}

void RandomNumberWithinRange::reset() {}

void RandomNumberWithinRange::initialize() {}

// SERIES RANDOM =====================================================
// range is inclusive, so range is: end - start + 1
SeriesRandomNumber::SeriesRandomNumber(int rangeStart, int rangeEnd) : DiscreteDistributionRandomNumber(rangeStart, rangeEnd)
{
    initialize();
}

SeriesRandomNumber::~SeriesRandomNumber()
{
}

int SeriesRandomNumber::getNumber()
{
    if (seriesIsComplete())
    {
        setEqualProbability();
    }

    std::discrete_distribution<int> dist(discreteDist.begin(), discreteDist.end());
    int selectedNumber = dist(random_number::engine);
    discreteDist[selectedNumber] = 0.0;
    return selectedNumber + offset;
}

void SeriesRandomNumber::reset()
{
    initialize();
}

void SeriesRandomNumber::initialize()
{
    setEqualProbability();
}

bool SeriesRandomNumber::seriesIsComplete()
{
    for (auto &&item : discreteDist)
    {
        if (item > 0.0)
        {
            return false;
        }
    }
    return true;
}

// RANDOM WITHOUT DIRECT REPETITION =============================================
// range is inclusive, so range is: end - start + 1
RandomNumberWithoutDirectRepetition::RandomNumberWithoutDirectRepetition(int rangeStart, int rangeEnd) : DiscreteDistributionRandomNumber(rangeStart, rangeEnd)
{
    initialize();
}

RandomNumberWithoutDirectRepetition::~RandomNumberWithoutDirectRepetition()
{
}

int RandomNumberWithoutDirectRepetition::getNumber()
{
    std::discrete_distribution<int> dist(discreteDist.begin(), discreteDist.end());
    int selectedNumber = dist(random_number::engine);
    setEqualProbability();
    discreteDist[selectedNumber] = 0.0;
    return selectedNumber + offset;
}

void RandomNumberWithoutDirectRepetition::reset()
{
    initialize();
}

void RandomNumberWithoutDirectRepetition::initialize()
{
    setEqualProbability();
}

// RANDOM ADJACENT STEPS =============================================
// range is inclusive, so range is: end - start + 1
// range does not wrap, so when rangeStart or rangeEnd is hit, can only step up or step down respectively
RandomNumberAdjacentSteps::RandomNumberAdjacentSteps(int rangeStart, int rangeEnd) : DiscreteDistributionRandomNumber(rangeStart, rangeEnd)
{
    initialize();
}

RandomNumberAdjacentSteps::RandomNumberAdjacentSteps(int rangeStart, int rangeEnd, int initialSelection) : RandomNumberAdjacentSteps(rangeStart, rangeEnd)
{
    initialNumberSelection = initialSelection - offset;
    hasInitialNumberSelection = true;
}

RandomNumberAdjacentSteps::~RandomNumberAdjacentSteps()
{
}

int RandomNumberAdjacentSteps::getNumber()
{
    std::discrete_distribution<int> dist(discreteDist.begin(), discreteDist.end());
    int selectedNumber;

    if (hasInitialNumberSelection && !hasMadeFirstSelection)
    {
        selectedNumber = initialNumberSelection;
    }
    else
    {
        selectedNumber = dist(random_number::engine);
    }

    hasMadeFirstSelection = true;
    setStepDistribution(selectedNumber);
    return selectedNumber + offset;
}

void RandomNumberAdjacentSteps::reset()
{
    // NB: this is essentially the same as the constructor where no initialSelection is provided
    // that means on reset, the initialSelection is disregarded, even if the class was originally consstructed with an initialSelection
    initialize();
}

void RandomNumberAdjacentSteps::initialize()
{
    hasMadeFirstSelection = false;
    hasInitialNumberSelection = false;
    setEqualProbability();
}

void RandomNumberAdjacentSteps::setZeroProbability()
{
    setUniformDistribution(0.0);
}

void RandomNumberAdjacentSteps::setStepDistribution(int lastSelectedNumber)
{
    setZeroProbability();
    if (isRangeStart(lastSelectedNumber))
    {
        discreteDist[lastSelectedNumber + 1] = 1.0;
    }
    else if (isRangeEnd(lastSelectedNumber))
    {
        discreteDist[lastSelectedNumber - 1] = 1.0;
    }
    else
    {
        discreteDist[lastSelectedNumber + 1] = 1.0;
        discreteDist[lastSelectedNumber - 1] = 1.0;
    }
}

bool RandomNumberAdjacentSteps::isRangeStart(int selectedNumber)
{
    return selectedNumber + offset == range.start;
}

bool RandomNumberAdjacentSteps::isRangeEnd(int selectedNumber)
{
    return selectedNumber + offset == range.end;
}

// PERIODIC RANDOM ============================================================
PeriodicRandomNumber::PeriodicRandomNumber(int rangeStart, int rangeEnd, double chanceOfRepetition) : DiscreteDistributionRandomNumber(rangeStart, rangeEnd)
{
    // NB: periodicity should probably be limited to 0-1 in future and chanceOfRepetition checked before setting this up
    periodicity = chanceOfRepetition;
    initialize();
}

PeriodicRandomNumber::PeriodicRandomNumber(int rangeStart, int rangeEnd, double chanceOfRepetition, int initialSelection) : PeriodicRandomNumber(rangeStart, rangeEnd, chanceOfRepetition)
{
    initialNumberSelection = initialSelection - offset;
    hasInitialNumberSelection = true;
}

PeriodicRandomNumber::~PeriodicRandomNumber() {}

int PeriodicRandomNumber::getNumber()
{
    std::discrete_distribution<int> dist(discreteDist.begin(), discreteDist.end());

    int selectedNumber;

    if (hasInitialNumberSelection && !hasMadeFirstSelection)
    {
        selectedNumber = initialNumberSelection;
    }
    else
    {
        selectedNumber = dist(random_number::engine);
    }

    hasMadeFirstSelection = true;
    setSingleBiasedDistribution(selectedNumber);
    return selectedNumber + offset;
}
void PeriodicRandomNumber::reset()
{
    initialize();
}

void PeriodicRandomNumber::initialize()
{
    hasMadeFirstSelection = false;
    hasInitialNumberSelection = false;
    setEqualProbability();
}

void PeriodicRandomNumber::setSingleBiasedDistribution(int lastSelectedNumber)
{
    // each unselected entry in dist should be given the same value which is: (1 - periodicity) / (dist.length - 1)
    double total = 1.0;
    double remainder = total - periodicity;
    double distributionSize = static_cast<double>(discreteDist.size());
    double unselectedEntryValue = remainder / (distributionSize - 1.0); // minus 1 to remove lastSelectedNumber from the equation
    setUniformDistribution(unselectedEntryValue);
    // currentSelection dist value should be the value of periodicity
    discreteDist[lastSelectedNumber] = periodicity;
}

// RANDOM WALK GRANULAR ==============================================================================================
// range is inclusive, so range is: end - start + 1
RandomNumberWalkGranular::RandomNumberWalkGranular(int rangeStart, int rangeEnd, double deviation) : RandomNumberSelector(rangeStart, rangeEnd)
{
    fixedScaleRange.start = 1;
    fixedScaleRange.end = 65000;
    scaleFactor = 65000;
    deviationFactor = deviation;
    maximumStep = std::round(deviationFactor * double(scaleFactor));
    initialize();
}

RandomNumberWalkGranular::RandomNumberWalkGranular(int rangeStart, int rangeEnd, double deviation, int initialSelection) : RandomNumberWalkGranular(rangeStart, rangeEnd, deviation)
{
    initialNumberSelection = initialSelection;
    hasInitialNumberSelection = true;
}

RandomNumberWalkGranular::~RandomNumberWalkGranular() {}

void RandomNumberWalkGranular::initialize()
{
    hasMadeFirstSelection = false;
    hasInitialNumberSelection = false;
}

double RandomNumberWalkGranular::getNumber()
{
    if (!hasMadeFirstSelection)
    {
        if (hasInitialNumberSelection)
        {
            lastNumberSelected = scaleResultUp(initialNumberSelection);
            hasMadeFirstSelection = true;
            return double(initialNumberSelection);
        }
        else
        {
            // select an initial number in range - uniform dist
            RandomNumber initialRandomNumber(fixedScaleRange.start, fixedScaleRange.end);
            lastNumberSelected = initialRandomNumber.getNumber();
            hasMadeFirstSelection = true;
            return scaleResultDown();
        }
    }

    return selectStepWithDirection();
}

void RandomNumberWalkGranular::reset()
{
    // NB: this is essentially the same as the constructor where no initialSelection is provided
    // that means on reset, the initialSelection is disregarded, even if the class was originally constructed with an initialSelection
    // additionally, the maxStep stays the same as when the object was created
    initialize();
}

double RandomNumberWalkGranular::scaleResultDown()
{
    double scaledNumber = double(lastNumberSelected) / double(scaleFactor);
    double scaledNumberToRange = scaledNumber * double(rangeSize - 1); // needs to scale to within the range, otherwise it overshoots
    return scaledNumberToRange + double(offset);
}

int RandomNumberWalkGranular::scaleResultUp(int numberToScale)
{
    // y = (x - xa) * ((yb - ya)  / (xb - xa)) + ya
    double scaledUpNumber = double(numberToScale - range.start) * double(double(fixedScaleRange.end - fixedScaleRange.start) / double(range.end - range.start)) + double(fixedScaleRange.start);
    return int(std::round(scaledUpNumber));
}

double RandomNumberWalkGranular::selectStepWithDirection()
{
    // determine direction: up or down
    int down = 0;
    int up = 1;
    RandomNumber upOrDown(down, up);
    int direction = upOrDown.getNumber();
    int stepRangeStart;
    int stepRangeEnd;

    // determine potential step size as uniform dist random number between (inclusive) current selection and max step
    // determine whether the maxStep > either range.start or range.end depending on which direction of travel was selected
    if (direction == down)
    {
        auto potentialStepRangeStart = lastNumberSelected - maximumStep;
        stepRangeStart = potentialStepRangeStart < fixedScaleRange.start ? fixedScaleRange.start : potentialStepRangeStart;
        stepRangeEnd = lastNumberSelected;
    }

    if (direction == up)
    {
        stepRangeStart = lastNumberSelected;
        auto potentialStepRangeEnd = lastNumberSelected + maximumStep;
        stepRangeEnd = potentialStepRangeEnd > fixedScaleRange.end ? fixedScaleRange.end : potentialStepRangeEnd;
    }

    RandomNumber randomNumber(stepRangeStart, stepRangeEnd);
    lastNumberSelected = randomNumber.getNumber();
    return scaleResultDown();
}

// RANDOM WALK ==============================================================================================
// range is inclusive, so range is: end - start + 1
RandomNumberWalk::RandomNumberWalk(int rangeStart, int rangeEnd, int maxStep) : RandomNumberSelector(rangeStart, rangeEnd)
{
    initialize();
    maximumStep = maxStep;
}

RandomNumberWalk::RandomNumberWalk(int rangeStart, int rangeEnd, int maxStep, int initialSelection) : RandomNumberWalk(rangeStart, rangeEnd, maxStep)
{
    initialNumberSelection = initialSelection;
    hasInitialNumberSelection = true;
}

RandomNumberWalk::~RandomNumberWalk() {}

int RandomNumberWalk::getNumber()
{
    if (!hasMadeFirstSelection)
    {
        if (hasInitialNumberSelection)
        {
            lastNumberSelected = initialNumberSelection;
        }
        else
        {
            // select an initial number in range - uniform dist
            RandomNumber initialRandomNumber(range.start, range.end);
            lastNumberSelected = initialRandomNumber.getNumber();
        }

        hasMadeFirstSelection = true;
        return lastNumberSelected;
    }

    return selectStepWithDirection();
}

void RandomNumberWalk::reset()
{
    // NB: this is essentially the same as the constructor where no initialSelection is provided
    // that means on reset, the initialSelection is disregarded, even if the class was originally constructed with an initialSelection
    // additionally, the maxStep stays the same as when the object was created
    initialize();
}

void RandomNumberWalk::initialize()
{
    hasMadeFirstSelection = false;
    hasInitialNumberSelection = false;
}

int RandomNumberWalk::selectStepWithDirection()
{
    // determine direction: up or down
    int down = 0;
    int up = 1;
    RandomNumber upOrDown(down, up);
    int direction = upOrDown.getNumber();
    int stepRangeStart;
    int stepRangeEnd;

    // determine potential step size as uniform dist random number between (inclusive) current selection and max step
    // determine whether the maxStep > either range.start or range.end depending on which direction of travel was selected
    if (direction == down)
    {

        auto potentialStepRangeStart = lastNumberSelected - maximumStep;
        stepRangeStart = potentialStepRangeStart < range.start ? range.start : potentialStepRangeStart;
        stepRangeEnd = lastNumberSelected;
    }

    if (direction == up)
    {
        stepRangeStart = lastNumberSelected;
        auto potentialStepRangeEnd = lastNumberSelected + maximumStep;
        stepRangeEnd = potentialStepRangeEnd > range.end ? range.end : potentialStepRangeEnd;
    }

    RandomNumber randomNumber(stepRangeStart, stepRangeEnd);
    lastNumberSelected = randomNumber.getNumber();
    return lastNumberSelected;
}
