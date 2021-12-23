#pragma once
#include <functional>
#include <tracktion_engine/tracktion_engine.h>
namespace te = tracktion_engine;

struct SelectionRange {
    double start;
    double end;
};

class TransportInteractor : public juce::Component,
                            private juce::ValueTree::Listener,
                            juce::AsyncUpdater {
  public:
    TransportInteractor(te::Edit &e);
    ~TransportInteractor();

    enum InteractionState { ControlCursor, ControlRangeSelection };

    void mouseDown(const juce::MouseEvent &event) override;
    void mouseDrag(const juce::MouseEvent &event) override;
    void mouseUp(const juce::MouseEvent &event) override;

    SelectionRange getSelectionRange();
    void setSelectionRange(SelectionRange newRange);
    void clearSelectionRange();
    std::function<void(const juce::MouseEvent event)>
        onSelectionChangeInProgress;
    std::function<void()> onSelectionChanged;

  private:
    void valueTreePropertyChanged(juce::ValueTree &,
                                  const juce::Identifier &prop) override;
    void handleAsyncUpdate() override;

    double xToTime(float mousePosition);
    void handleMouseMovement(int mousePosition);

    te::Edit &edit;
    te::TransportControl &transport;
    InteractionState interactionState;
    int mouseDownPosition; // NB: this is a variable for temporarily holding
                           // on to the mouse down position when user is
                           // making a range selection. Note that this was
                           // introduced because the value of
                           // MouseEvent.getMouseDownPosition().x doesnt seem
                           // to be accurate when used in the context of a
                           // scrolling viewport

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TransportInteractor)
};
