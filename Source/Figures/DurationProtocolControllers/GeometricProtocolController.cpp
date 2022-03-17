#include "GeometricProtocolController.h"

GeometricProtocolController::GeometricProtocolController(
    DurationProtocolParams &p)
: params(p),
  rangeStart(params.geometric.rangeStart, "Range start"),
  rangeEnd(params.geometric.rangeEnd, "Range end"),
  collectionSize(params.geometric.collectionSize, "Collection size")
{
    addAndMakeVisible(&rangeStart);
    addAndMakeVisible(&rangeEnd);
    addAndMakeVisible(&collectionSize);
}

void GeometricProtocolController::resized()
{
    auto margin = 10;
    auto area = getLocalBounds();

    // Params controllers
    auto paramsArea = area.removeFromLeft(250);
    rangeStart.setBounds(paramsArea.removeFromTop(45));
    rangeEnd.setBounds(paramsArea.removeFromTop(45));
    collectionSize.setBounds(paramsArea.removeFromTop(45));
}
