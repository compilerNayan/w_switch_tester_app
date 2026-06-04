#ifndef ARDUINO
#ifndef STUBRELAYCONTROLLER_H
#define STUBRELAYCONTROLLER_H

#include <StandardDefines.h>
#include "IRelayController.h"
#include "SwitchState.h"
#include "ILogger.h"
#include "Tag.h"
#include <map>

/* @Component */
class StubRelayController : public IRelayController {
    Private
        // Store pin states for testing purposes
        std::map<Int, SwitchState> pinStates;

    /* @Autowired */
    Private ILoggerPtr logger;

    Public Virtual ~StubRelayController() = default;

    Public Virtual Void SetState(Int pin, SwitchState state) override {
        pinStates[pin] = state;
        
        StdString message = "Set relay at pin " + std::to_string(pin) + " to " + 
                           (state == SwitchState::On ? "ON" : "OFF");
        StdString functionName = "SetState";
        logger->Info(Tag::Untagged, message, functionName);
    }

    Public Virtual SwitchState GetState(Int pin) override {
        // Return stored state, default to Off if not set
        SwitchState state = (pinStates.find(pin) != pinStates.end()) ? pinStates[pin] : SwitchState::Off;
        if (logger != nullptr) {
            StdString message = "Get state of pin " + std::to_string(pin) + ": " + (state == SwitchState::On ? "ON" : "OFF");
            StdString functionName = "GetState";
            logger->Info(Tag::Untagged, message, functionName);
        }
        return state;
    }
};

#endif // STUBRELAYCONTROLLER_H
#endif // ARDUINO

