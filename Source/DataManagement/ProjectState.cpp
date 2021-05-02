#include "ProjectState.h"

#include "ErrorTypes.h"
#include "Identifiers.h"

ProjectState::ProjectState()
: state(IDs::PROJECT_STATE), statusVt(IDs::PROJECT_STATE_STATUS)
{
    state.addListener(this);

    statusVt.setProperty(IDs::action, Action::NoAction, nullptr);
    statusVt.setProperty(IDs::has_unsaved_changes, false, nullptr);
    statusVt.setProperty(IDs::has_file, false, nullptr);
    statusVt.addListener(this);
}

ProjectState::ProjectState(juce::ValueTree vt)
: state(vt), statusVt(IDs::PROJECT_STATE_STATUS)
{
    jassert(state.hasType(IDs::PROJECT_STATE));
    state.addListener(this);

    statusVt.setProperty(IDs::action, Action::NoAction, nullptr);
    statusVt.setProperty(IDs::has_unsaved_changes, false, nullptr);
    statusVt.setProperty(IDs::has_file, false, nullptr);
    statusVt.addListener(this);
}

ProjectState::ProjectState(const ProjectState &ps)
: state(ps.state), statusVt(ps.statusVt)
{
    state.addListener(this);
    statusVt.addListener(this);
}

ProjectState::~ProjectState()
{
    state.removeListener(this);
    statusVt.removeListener(this);
}

ParticleList ProjectState::getParticleList() const
{
    return ParticleList(state);
}

SourceList ProjectState::getSourceList() const
{
    return SourceList(state);
}

FigureList ProjectState::getFigureList() const
{
    return FigureList(state);
}

EventList ProjectState::getEventList() const
{
    return EventList(state);
}

EventList ProjectState::getEventList(const Figure &f) const
{
    return EventList(state, f);
}

void ProjectState::save()
{
    if(file == nullptr) {
        statusVt.setProperty(IDs::has_file, false, nullptr);
        throw ProjectFileNotFound();
    }

    saveStateToFile(*file);

    statusVt.setProperty(IDs::action, Action::SaveToExistingFile, nullptr);
    statusVt.setProperty(IDs::has_file, true, nullptr);
    statusVt.setProperty(IDs::has_unsaved_changes, false, nullptr);
}

void ProjectState::save(const juce::File &f)
{
    saveStateToFile(f);

    file = std::make_shared<juce::File>(f);

    statusVt.setProperty(IDs::action, Action::SaveToNewFile, nullptr);
    statusVt.setProperty(IDs::has_file, true, nullptr);
    statusVt.setProperty(IDs::has_unsaved_changes, false, nullptr);
}

void ProjectState::load(const juce::File &f)
{
    loadStateFromFile(f);

    file = std::make_shared<juce::File>(f);

    statusVt.setProperty(IDs::action, Action::LoadNewFile, nullptr);
    statusVt.setProperty(IDs::has_file, true, nullptr);
    statusVt.setProperty(IDs::has_unsaved_changes, false, nullptr);
}

ProjectState::Status ProjectState::getStatus() const
{
    return Status {bool(statusVt[IDs::has_unsaved_changes]),
                   bool(statusVt[IDs::has_file])};
}

const std::shared_ptr<const juce::File> ProjectState::getFile() const
{
    return file;
}

// VT Listeners
void ProjectState::valueTreeChildAdded(juce::ValueTree &parent,
                                       juce::ValueTree &child)
{
    if(parent.hasType(IDs::PROJECT_STATE)) {
        statusVt.setProperty(IDs::action, Action::StateChange, nullptr);
        statusVt.setProperty(IDs::has_unsaved_changes, true, nullptr);
    }
}

void ProjectState::valueTreeChildRemoved(juce::ValueTree &parent,
                                         juce::ValueTree &child,
                                         int)
{
    if(parent.hasType(IDs::PROJECT_STATE)) {
        statusVt.setProperty(IDs::action, Action::StateChange, nullptr);
        statusVt.setProperty(IDs::has_unsaved_changes, true, nullptr);
    }
}

void ProjectState::valueTreePropertyChanged(juce::ValueTree &vt,
                                            const juce::Identifier &property)
{
    if(vt.getParent().hasType(IDs::PROJECT_STATE)) {
        statusVt.setProperty(IDs::action, Action::StateChange, nullptr);
        statusVt.setProperty(IDs::has_unsaved_changes, true, nullptr);
    }

    if(vt.hasType(IDs::PROJECT_STATE_STATUS) && property != IDs::action) {
        if(onStatusChanged) {
            onStatusChanged(Status {bool(statusVt[IDs::has_unsaved_changes]),
                                    bool(statusVt[IDs::has_file])},
                            static_cast<Action>(int(statusVt[IDs::action])));
        }
    }
}

// Private methods
void ProjectState::saveStateToFile(const juce::File &file) const
{
    if(!file.existsAsFile()) {
        throw InvalidFilePath();
    }

    if(!file.hasFileExtension(".xml")) {
        throw InvalidProjectFile();
    }

    const juce::TemporaryFile temp(file);

    juce::FileOutputStream os(temp.getFile());
    jassert(os.openedOk()); // assert because above checks should ensure stream
                            // is ok
    auto xml = state.createXml();
    jassert(xml != nullptr); // assert as there is no forseen way for the
                             // state vt to be invalid
    xml->writeTo(os);

    // NB: Untested: don't know how to test it
    if(os.getStatus().failed()) {
        throw ProjectSaveFailed();
    }

    auto overwriteSuccess = temp.overwriteTargetFileWithTemporary();

    // NB: Untested: don't know how to test it
    if(!overwriteSuccess) {
        throw ProjectSaveFailed();
    }
}

void ProjectState::loadStateFromFile(const juce::File &file)
{
    if(!file.existsAsFile()) {
        throw InvalidFilePath();
    }

    if(!file.hasFileExtension(".xml")) {
        throw InvalidProjectFile();
    }

    auto xml = juce::parseXML(file);

    if(xml == nullptr) {
        throw InvalidProjectFile();
    }

    auto fileAsVt = juce::ValueTree::fromXml(*xml);

    if(!loadedStateIsValid(fileAsVt)) {
        throw InvalidProjectFile();
    }

    // Replaces all children and properties of this object with copies of those
    // from the source object
    state.copyPropertiesAndChildrenFrom(fileAsVt, nullptr);
}

bool ProjectState::loadedStateIsValid(const juce::ValueTree &vt)
{
    if(!vt.hasType(IDs::PROJECT_STATE)) {
        return false;
    }

    if(vt.getNumProperties() > 0) {
        return false;
    }

    std::vector<juce::Identifier> validTypes {IDs::EVENT,
                                              IDs::FIGURE,
                                              IDs::PARTICLE,
                                              IDs::SOURCE};

    for(const auto &child : vt) {
        auto typeIsInvalid = std::find(std::begin(validTypes),
                                       std::end(validTypes),
                                       child.getType()) == std::end(validTypes);

        if(typeIsInvalid) {
            return false;
        }
    }

    // check data for objects (via lists) is valid
    try {
        EventList(vt).getObjects();
        FigureList(vt).getObjects();
        ParticleList(vt).getObjects();
        SourceList(vt).getObjects();
    } catch(const std::exception &e) {
        return false;
    }

    return true;
}
