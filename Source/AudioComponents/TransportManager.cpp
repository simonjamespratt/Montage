#include "TransportManager.h"

TransportManager::TransportManager(te::Edit &e) : edit(e)
{}

TransportManager::~TransportManager()
{
    transportReporter.stopTimer();
}

void TransportManager::startManager()
{
    transportReporter.setCallback([this] {
        auto &transport = edit.getTransport();

        if(transport.isPlaying()) {
            auto loopRange = transport.getLoopRange();
            auto transportPosition = transport.getCurrentPosition();
            if(loopRange.isEmpty()) {
                auto editLength = edit.getLength();
                if(transportPosition > editLength) {
                    transport.stop(false, false);
                    transport.setCurrentPosition(0);
                }
            } else {
                // NB: checking loopRange.getEnd() is not quite accurate enough
                // with the frequency of timer being used. Have done a review of
                // how tracktion_TransportControl handles watching for the end
                // of a loop range: It has one method
                // TransportControl::playSectionAndReset() which implements it
                // exactly as is done here. TransportControl::performPlay() when
                // looping handles it differently and is more accurate, but it's
                // not clear exactly how it achieves that. If this inaccuracy
                // becomes an issue, suggest re-reviewing these two methods
                // above as a starting point.
                if(transportPosition > loopRange.getEnd() &&
                   !transport.looping) {
                    transport.stop(false, false);
                    transport.setCurrentPosition(loopRange.getStart());
                }
            }

            if(onChange) {
                onChange();
            }
        }
    });
    transportReporter.startTimerHz(25);
}
