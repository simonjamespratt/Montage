#include "AudioRenderer.h"

void AudioRenderer::renderFigureToFile(te::Edit &edit,
                                       ProjectState &ps,
                                       juce::String filename)
{
    auto rendersDir = ps.getProjectDirectory().getChildFile("renders");
    jassert(rendersDir.isDirectory());

    juce::FileChooser fileChooser("Choose directory to save rendered figure",
                                  rendersDir);

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
