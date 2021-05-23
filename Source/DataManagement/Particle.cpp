#include "Particle.h"

#include "ErrorTypes.h"
#include "StateService.h"

#include <vector>

Particle::Particle(const juce::ValueTree &v, const Source &s)
: state(v), source(s)
{
    StateService::checkTypeIsValid(state, IDs::PARTICLE);

    std::vector<juce::Identifier> compulsoryProps {IDs::id,
                                                   IDs::source_id,
                                                   IDs::start,
                                                   IDs::end,
                                                   IDs::name};

    StateService::checkPropsAreValid(state, compulsoryProps);

    state.addListener(this);

    juce::Uuid sourceId(state[IDs::source_id]);
    double start {state[IDs::start]};
    double end {state[IDs::end]};
    auto sourceFileLength = source.getFileLengthInSeconds();
    juce::String name = state[IDs::name];

    if(sourceId != source.getId()) {
        throw ObjectDependencyInvalid("Particle", "Source");
    }

    validateStart(start, end);
    validateEnd(start, end, sourceFileLength);

    if(name.isEmpty()) {
        throw ParticleNameInvalid();
    }
}

Particle::Particle(const Source &s) : source(s)
{
    state = juce::ValueTree(IDs::PARTICLE);
    state.setProperty(IDs::id, juce::Uuid().toString(), nullptr);
    state.setProperty(IDs::source_id, source.getId().toString(), nullptr);
    state.setProperty(IDs::start, 0, nullptr);
    state.setProperty(IDs::end, source.getFileLengthInSeconds(), nullptr);
    state.setProperty(IDs::name, juce::String("untitled"), nullptr);

    state.addListener(this);
}

Particle::Particle(const Particle &p) : state(p.state), source(p.source)
{
    state.addListener(this);
}

Particle::~Particle()
{
    state.removeListener(this);
}

juce::Uuid Particle::getId() const
{
    return juce::Uuid(state[IDs::id]);
}

juce::Uuid Particle::getSourceId() const
{
    return juce::Uuid(state[IDs::source_id]);
}

Source Particle::getSource() const
{
    return source;
}

double Particle::getStart() const
{
    return state[IDs::start];
}

void Particle::setStart(double newStart)
{
    validateStart(newStart, getEnd());
    state.setProperty(IDs::start, newStart, nullptr);
}

double Particle::ensureNewStartIsWithinBounds(const double &proposedStart,
                                              const double &existingEnd) const
{
    if(proposedStart < 0) {
        return 0;
    }

    // NB: 10 ms is subtracted here because tracktion requires it as a min
    // when looping. Happy to only do this here because it's a helper
    // method, but if it turns out that it is a prerequisite for the correct
    // functioning of a Particle, then it'll need hard baking into the
    // validation for this class too. Note that there is also a check that
    // when the proposed value is within less than 10ms of the end, that it
    // needs to be modified to be 10 ms less than the end. If this 10 ms
    // rule ends up pervading the class, validation will need to check for
    // this type of situation as well.
    auto endMinus10ms = existingEnd - 0.01;
    if(proposedStart > endMinus10ms) {
        return endMinus10ms;
    }

    return proposedStart;
}

double Particle::getEnd() const
{
    return state[IDs::end];
}

void Particle::setEnd(double newEnd)
{
    validateEnd(getStart(), newEnd, source.getFileLengthInSeconds());
    state.setProperty(IDs::end, newEnd, nullptr);
}

double Particle::ensureNewEndIsWithinBounds(const double &existingStart,
                                            const double &proposedEnd) const
{
    auto fileLength = getSource().getFileLengthInSeconds();
    if(proposedEnd > fileLength) {
        return fileLength;
    }

    // NB: see above re. 10 ms rule
    auto startPlus10ms = existingStart + 0.01;
    if(proposedEnd < startPlus10ms) {
        return startPlus10ms;
    }

    return proposedEnd;
}

void Particle::setStartAndEnd(double newStart, double newEnd)
{
    validateStart(newStart, newEnd);
    validateEnd(newStart, newEnd, getSource().getFileLengthInSeconds());

    // NB: important to set props in the right order to avoid interim breach of
    // invariants. Especially relevant for what prop-change listeners do in
    // their callbacks. See tests for more detail.
    if(newStart >= getEnd()) {
        state.setProperty(IDs::end, newEnd, nullptr);
        state.setProperty(IDs::start, newStart, nullptr);
    } else {
        state.setProperty(IDs::start, newStart, nullptr);
        state.setProperty(IDs::end, newEnd, nullptr);
    }
}

juce::String Particle::getName() const
{
    return state[IDs::name];
}

void Particle::setName(juce::String newName)
{
    if(newName.isEmpty()) {
        throw ParticleNameInvalid();
    }

    state.setProperty(IDs::name, newName, nullptr);
}

juce::ValueTree Particle::getState() const
{
    return state;
}

// VT listeners
void Particle::valueTreePropertyChanged(juce::ValueTree &vt,
                                        const juce::Identifier &property)
{
    if(vt == state && onUpdated) {
        onUpdated(property);
    }
}

// Private methods
void Particle::validateStart(double start, double end)
{
    if(start < 0 || start >= end) {
        throw ParticleStartInvalid();
    }
}

void Particle::validateEnd(double start, double end, double sourceFileLength)
{
    if(end <= start || end > sourceFileLength) {
        throw ParticleEndInvalid();
    }
}
