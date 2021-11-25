#include "AudioRenderer.h"

void AudioRenderer::renderFigureToFile(te::Edit &edit, juce::String filename)
{
    juce::FileChooser fileChooser(
        "Choose directory to save rendered figure",
        juce::File::getSpecialLocation(juce::File::userDesktopDirectory));

    if(fileChooser.browseForDirectory()) {
        juce::File outputFile {
            fileChooser.getResult().getNonexistentChildFile(filename,
                                                            ".wav",
                                                            true)};
        te::EditTimeRange range {0.0, edit.getLength()};
        juce::BigInteger tracksToDo {0};

        for(auto i = 0; i < te::getAllTracks(edit).size(); i++) {
            tracksToDo.setBit(i);
        }

        // NB: cannot render and perform playback at the same time
        te::TransportControl::stopAllTransports(edit.engine, false, true);

        te::Renderer::renderToFile("Render",
                                   outputFile,
                                   edit,
                                   range,
                                   tracksToDo);
    }
}
