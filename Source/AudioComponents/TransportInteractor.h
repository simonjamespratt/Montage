#pragma once
#include <functional>
#include <tracktion_engine/tracktion_engine.h>
namespace te = tracktion_engine;

struct SelectionRange {
    double rangeStart;
    double rangeEnd;
};

class TransportInteractor : public juce::Component {
  public:
    TransportInteractor(te::TransportControl &tc, te::Edit &e);
    ~TransportInteractor();

    enum InteractionState { ControlCursor, ControlRangeSelection };

    void paint(juce::Graphics &) override;
    void mouseDown(const juce::MouseEvent &event) override;
    void mouseDrag(const juce::MouseEvent &event) override;
    void mouseUp(const juce::MouseEvent &event) override;

    SelectionRange getSelectionRange();
    void setSelectionRange(SelectionRange newRange);
    void clearSelectionRange();
    std::function<void(const juce::MouseEvent event)>
        onSelectionChangeInProgress;
    std::function<void()> onSelectionChange;

  private:
    te::TransportControl &transport;
    te::Edit &edit;
    InteractionState interactionState;
    double rangeStart;
    double rangeEnd;
    int mouseDownPosition; // NB: this is a variable for temporarily holding
                           // on to the mouse down position when user is
                           // making a range selection. Note that this was
                           // introduced because the value of
                           // MouseEvent.getMouseDownPosition().x doesnt seem
                           // to be accurate when used in the context of a
                           // scrolling viewport

    double calculateAudioPosition(float mousePosition);
    float calculateUIPosition(double rangePosition);
    void handleMouseMovement(int mousePosition);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TransportInteractor)
};
