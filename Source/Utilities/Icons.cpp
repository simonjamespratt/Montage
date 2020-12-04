#include "./Icons.h"

Icons::Icons()
{
    drawCross();
    drawDash();
    drawStop();
    drawPlay();
    drawPause();
}

Icons::~Icons()
{}

juce::DrawablePath Icons::getIcon(IconType iconType)
{
    switch(iconType) {
    case Cross:
        return cross;
    case Dash:
        return dash;
    case Stop:
        return stop;
    case Play:
        return play;
    case Pause:
        return pause;
    default:
        break;
    }
}

// NB: working on the assumption of a theoretical area of 100 x 100
void Icons::drawCross()
{
    float crossThickness = 100.0f / 5.0f; // 20.0f
    juce::Rectangle<float> vertical(100.0f / 2.0f - crossThickness / 2.0f,
                                    0.0f,
                                    crossThickness,
                                    100.0f);
    juce::Rectangle<float> horizontal(0.0f,
                                      100.0f / 2.0f - crossThickness / 2.0f,
                                      100.0f,
                                      crossThickness);
    crossPath.addRectangle(vertical);
    crossPath.addRectangle(horizontal);
    cross.setPath(crossPath);
    cross.setFill(juce::FillType(juce::Colours::white));
}

void Icons::drawDash()
{
    float lineThickness = 100.0f / 5.0f; // 20.0f
    juce::Rectangle<float> dashLine(0.0f,
                                    100.0f / 2.0f - lineThickness / 2.0f,
                                    100.0f,
                                    lineThickness);
    dashPath.addRectangle(dashLine);
    dash.setPath(dashPath);
    dash.setFill(juce::FillType(juce::Colours::white));
}

void Icons::drawStop()
{
    stopPath.addRectangle(0.0f, 0.0f, 100.0f, 100.0f);
    stop.setPath(stopPath);
    stop.setFill(juce::FillType(juce::Colours::white));
}

void Icons::drawPlay()
{
    juce::Point<float> pointOne(0.0f, 0.0f);
    juce::Point<float> pointTwo(0.0f, 100.0f);
    juce::Point<float> pointThree(100.0f, 50.0f);
    playPath.addTriangle(pointOne, pointTwo, pointThree);
    play.setPath(playPath);
    play.setFill(juce::FillType(juce::Colours::white));
}

void Icons::drawPause()
{
    float columnThickness = 100.0f / 3.0f; // 20.0f
    juce::Rectangle<float> colOne(0.0f, 0.0f, columnThickness, 100.0f);
    juce::Rectangle<float> colTwo((100.0f - columnThickness),
                                  0.0f,
                                  columnThickness,
                                  100.0f);
    pausePath.addRectangle(colOne);
    pausePath.addRectangle(colTwo);
    pause.setPath(pausePath);
    pause.setFill(juce::FillType(juce::Colours::white));
}
