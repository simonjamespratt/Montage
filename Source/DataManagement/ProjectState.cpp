#include "ProjectState.h"

#include "ErrorTypes.h"
#include "Identifiers.h"

ProjectState::ProjectState()
: state(IDs::PROJECT_STATE), statusVt(IDs::PROJECT_STATE_STATUS)
{
    state.addListener(this);

    statusVt.setProperty(IDs::action, Action::NoAction, nullptr);
    statusVt.setProperty(IDs::has_unsaved_changes, false, nullptr);
    statusVt.setProperty(IDs::has_project_loaded, false, nullptr);
    statusVt.setProperty(IDs::figure_edit_has_unsaved_changes, false, nullptr);
    statusVt.addListener(this);
}

ProjectState::ProjectState(juce::ValueTree vt)
: state(vt), statusVt(IDs::PROJECT_STATE_STATUS)
{
    jassert(state.hasType(IDs::PROJECT_STATE));
    state.addListener(this);

    statusVt.setProperty(IDs::action, Action::NoAction, nullptr);
    statusVt.setProperty(IDs::has_unsaved_changes, false, nullptr);
    statusVt.setProperty(IDs::has_project_loaded, false, nullptr);
    statusVt.setProperty(IDs::figure_edit_has_unsaved_changes, false, nullptr);
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

void ProjectState::create(const juce::File &directory)
{
    if(!directory.isDirectory()) {
        throw InvalidDirectoryPath();
    }
    if(directory.getNumberOfChildFiles(
           juce::File::TypesOfFileToFind::findFilesAndDirectories) > 0) {
        throw InvalidDirectoryForProjectCreation();
    }

    statusVt.setProperty(IDs::project_directory_filepath,
                         directory.getFullPathName(),
                         nullptr);

    auto projectFile = directory.getChildFile("project-state.xml");
    projectFile.create();
    directory.getChildFile("figures").createDirectory();
    directory.getChildFile("renders").createDirectory();

    // NB: this block ensures that the created project state file has a valid
    // empty project that can be loaded again
    state.removeAllChildren(nullptr);
    state.removeAllProperties(nullptr);
    saveStateToFile(projectFile);

    statusVt.setProperty(IDs::action, Action::CreateProject, nullptr);
    statusVt.setProperty(IDs::has_project_loaded, true, nullptr);
    statusVt.setProperty(IDs::has_unsaved_changes, false, nullptr);
}

void ProjectState::save()
{
    juce::File projectDirectory(statusVt[IDs::project_directory_filepath]);

    if(!projectDirectory.isDirectory()) {
        statusVt.setProperty(IDs::has_project_loaded, false, nullptr);
        throw ProjectDirectoryNotFound();
    }

    auto projectStateFile = projectDirectory.getChildFile("project-state.xml");
    jassert(projectStateFile.existsAsFile());

    saveStateToFile(projectStateFile);

    statusVt.setProperty(IDs::action, Action::SaveProject, nullptr);
    statusVt.setProperty(IDs::has_unsaved_changes, false, nullptr);
}

void ProjectState::load(const juce::File &directory)
{
    if(!directory.isDirectory()) {
        throw InvalidDirectoryPath();
    }

    if(!directory.getChildFile("project-state.xml").existsAsFile() ||
       !directory.getChildFile("figures").isDirectory() ||
       !directory.getChildFile("renders").isDirectory()) {
        throw InvalidDirectoryForProjectLoad();
    }

    loadStateFromFile(directory.getChildFile("project-state.xml"));

    statusVt.setProperty(IDs::project_directory_filepath,
                         directory.getFullPathName(),
                         nullptr);

    statusVt.setProperty(IDs::action, Action::LoadProject, nullptr);
    statusVt.setProperty(IDs::has_project_loaded, true, nullptr);
    statusVt.setProperty(IDs::has_unsaved_changes, false, nullptr);
}

ProjectState::Status ProjectState::getStatus() const
{
    return Status {bool(statusVt[IDs::has_unsaved_changes]),
                   bool(statusVt[IDs::has_project_loaded]),
                   bool(statusVt[IDs::figure_edit_has_unsaved_changes])};
}

juce::File ProjectState::getProjectDirectory() const
{
    return juce::File(statusVt[IDs::project_directory_filepath]);
}

juce::File ProjectState::getFileForFigure(const Figure &f) const
{
    juce::File projectDirectory(statusVt[IDs::project_directory_filepath]);

    if(!projectDirectory.isDirectory()) {
        throw InvalidDirectoryPath();
    }

    auto file = projectDirectory.getChildFile("figures").getChildFile(
        f.getId().toString() + ".xml");

    if(!file.existsAsFile()) {
        file.create();
    }
    return file;
}

void ProjectState::deleteFileForFigure(const Figure &f)
{
    juce::File projectDirectory(statusVt[IDs::project_directory_filepath]);

    if(!projectDirectory.isDirectory()) {
        throw InvalidDirectoryPath();
    }

    auto file = projectDirectory.getChildFile("figures").getChildFile(
        f.getId().toString() + ".xml");

    if(!file.existsAsFile()) {
        throw EditFileNotFoundForFigure();
    }

    file.deleteFile();
}

void ProjectState::setFigureEditHasUnsavedChanges(bool newValue)
{
    statusVt.setProperty(IDs::action, Action::StateChange, nullptr);
    statusVt.setProperty(IDs::figure_edit_has_unsaved_changes,
                         newValue,
                         nullptr);
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

    if(vt.hasType(IDs::PROJECT_STATE_STATUS)) {
        if(onStatusChanged) {
            onStatusChanged(
                Status {bool(statusVt[IDs::has_unsaved_changes]),
                        bool(statusVt[IDs::has_project_loaded]),
                        bool(statusVt[IDs::figure_edit_has_unsaved_changes])},
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
