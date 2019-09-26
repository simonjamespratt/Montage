/*
  ==============================================================================

    Icons.cpp
    Created: 5 May 2019 7:05:01pm
    Author:  Simon Pratt

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "./Icons.h"

//==============================================================================
Icons::Icons()
{
    drawCross();
    drawDash();
    drawStop();
    drawPlay();
    drawPause();
}

Icons::~Icons()
{
}

DrawablePath Icons::getIcon(IconType iconType)
{
    switch (iconType)
    {
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
    Rectangle<float> vertical(100.0f / 2.0f - crossThickness / 2.0f, 0.0f, crossThickness, 100.0f);
    Rectangle<float> horizontal(0.0f, 100.0f / 2.0f - crossThickness / 2.0f, 100.0f, crossThickness);
    crossPath.addRectangle(vertical);
    crossPath.addRectangle(horizontal);
    cross.setPath(crossPath);
    cross.setFill(FillType(Colours::white));
}

void Icons::drawDash()
{
    float lineThickness = 100.0f / 5.0f; // 20.0f
    Rectangle<float> dashLine(0.0f, 100.0f / 2.0f - lineThickness / 2.0f, 100.0f, lineThickness);
    dashPath.addRectangle(dashLine);
    dash.setPath(dashPath);
    dash.setFill(FillType(Colours::white));
}

void Icons::drawStop()
{
    stopPath.addRectangle(0.0f, 0.0f, 100.0f, 100.0f);
    stop.setPath(stopPath);
    stop.setFill(FillType(Colours::white));
}

void Icons::drawPlay()
{
    Point<float> pointOne(0.0f, 0.0f);
    Point<float> pointTwo(0.0f, 100.0f);
    Point<float> pointThree(100.0f, 50.0f);
    playPath.addTriangle(pointOne, pointTwo, pointThree);
    play.setPath(playPath);
    play.setFill(FillType(Colours::white));
}

void Icons::drawPause()
{
    float columnThickness = 100.0f / 3.0f; //20.0f
    Rectangle<float> colOne(0.0f, 0.0f, columnThickness, 100.0f);
    Rectangle<float> colTwo((100.0f - columnThickness), 0.0f, columnThickness, 100.0f);
    pausePath.addRectangle(colOne);
    pausePath.addRectangle(colTwo);
    pause.setPath(pausePath);
    pause.setFill(FillType(Colours::white));
}
