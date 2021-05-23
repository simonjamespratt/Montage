#include "ParticleView.h"

// ParticleThumbnailOverlay =======================================
ParticleThumbnailOverlay::ParticleThumbnailOverlay(const Particle &p)
: particle(p)
{}

void ParticleThumbnailOverlay::paint(juce::Graphics &g)
{
    auto area = getLocalBounds();
    auto width = area.getWidth();
    auto fileLength = particle.getSource().getFileLengthInSeconds();
    auto rectStart = (particle.getStart() / fileLength) * width;
    auto rectEnd = (particle.getEnd() / fileLength) * width;

    g.setColour(juce::Colour::fromFloatRGBA(0.0f, 1.0f, 0.0f, 0.5f));
    g.fillRect(rectStart, 0.0f, (rectEnd - rectStart), float(getHeight()));
}

// ParticleView ===============================================
ParticleView::ParticleView(const Particle &p, te::Engine &eng)
: particle(p),
  thumbnail(te::Edit(eng,
                     /* TODO: TRACKTION: method signature for
                        te::createEmptyEdit() is a legacy signature. Update to
                        newer version. See tracktion_EditFileOperations.h */
                     te::createEmptyEdit(eng),
                     te::Edit::forEditing,
                     nullptr,
                     0)
                .getTransport()),
  overlay(p)
{
    refreshView();

    particle.onUpdated = [this](juce::Identifier) {
        refreshView();
    };

    addAndMakeVisible(name);
    addAndMakeVisible(sourceFileName);
    addAndMakeVisible(start);
    addAndMakeVisible(end);

    removeButton.setButtonText("Remove");
    removeButton.onClick = [this] {
        if(onRemove) {
            onRemove(particle);
        }
    };
    addAndMakeVisible(removeButton);

    editButton.setButtonText("Edit");
    editButton.onClick = [this] {
        if(onEdit) {
            onEdit(particle);
        }
    };
    addAndMakeVisible(editButton);

    te::AudioFile audioFile(eng, particle.getSource().getFile());
    thumbnail.setFile(audioFile, 0.0, audioFile.getLength());
    addAndMakeVisible(&thumbnail);

    addAndMakeVisible(overlay);
}

void ParticleView::resized()
{
    auto lineHeight = 35;
    auto margin = 10;

    auto area = getLocalBounds();
    auto namesArea = area.removeFromLeft(300);
    auto lengthArea = area.removeFromLeft(100);
    auto controlsArea = area.removeFromRight(100);
    auto thumbnailArea = area.reduced(margin);

    name.setBounds(namesArea.removeFromTop(lineHeight).reduced(margin));
    sourceFileName.setBounds(namesArea.reduced(margin));
    start.setBounds(lengthArea.removeFromTop(lineHeight).reduced(margin));
    end.setBounds(lengthArea.reduced(margin));

    if(editButton.isVisible()) {
        editButton.setBounds(
            controlsArea.removeFromTop(lineHeight).reduced(margin / 2));
    }

    if(removeButton.isVisible()) {
        removeButton.setBounds(
            controlsArea.removeFromTop(lineHeight).reduced(margin / 2));
    }

    thumbnail.setBounds(thumbnailArea);
    overlay.setBounds(thumbnailArea);
}

void ParticleView::paint(juce::Graphics &g)
{
    auto area = getLocalBounds();
    auto height = area.getHeight();
    auto width = area.getWidth();

    // draw a line at the top of the component as a divider
    g.setColour(juce::Colours::aliceblue);
    g.fillRect(0.0f, 0.0f, float(width), 1.0f);
}

void ParticleView::allowInteraction(bool isAllowed)
{
    editButton.setVisible(isAllowed);
    removeButton.setVisible(isAllowed);
    resized();
}

// Private methods
void ParticleView::refreshView()
{
    juce::String nameString = "Name: " + particle.getName();
    juce::String fileNameString =
        "File name: " + particle.getSource().getFileName();
    juce::String startString = "Start: " + juce::String(particle.getStart());
    juce::String endString = "End: " + juce::String(particle.getEnd());

    name.setText(nameString, juce::dontSendNotification);
    sourceFileName.setText(fileNameString, juce::dontSendNotification);
    start.setText(startString, juce::dontSendNotification);
    end.setText(endString, juce::dontSendNotification);
}
