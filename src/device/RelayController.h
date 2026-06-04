#ifdef ARDUINO
#ifndef RELAYCONTROLLER_H
#define RELAYCONTROLLER_H

#include <StandardDefines.h>
#include "IRelayController.h"
#include "SwitchState.h"
#include <Arduino.h>
#include "ILogger.h"
#include "Tag.h"

/**
 * Relay board is ACTIVE-LOW: GPIO LOW = relay ON (LED on, click), GPIO HIGH = relay OFF (LED off).
 * We invert logic so SwitchState::On -> write LOW, SwitchState::Off -> write HIGH.
 */
/* @Component */
class RelayController : public IRelayController {
    Public Virtual ~RelayController() = default;

    /* @Autowired */
    Private ILoggerPtr logger;

    Public Virtual Void SetState(Int pin, SwitchState state) override {
        if (logger != nullptr) {
            logger->Info(Tag::Untagged, "[RelayController] SetState called: pin=" + std::to_string(pin) +
                " requested=" + (state == SwitchState::On ? "ON" : "OFF"));
        }

        pinMode(pin, OUTPUT);

        // Active-low: On -> LOW (relay energizes), Off -> HIGH (relay off)
        int gpioValue = (state == SwitchState::On) ? LOW : HIGH;
        digitalWrite(pin, gpioValue);

        if (logger != nullptr) {
            logger->Info(Tag::Untagged, "[RelayController] SetState done: pin=" + std::to_string(pin) +
                " relay=" + (state == SwitchState::On ? "ON" : "OFF") +
                " GPIO=" + (gpioValue == HIGH ? "HIGH" : "LOW"));
        }
    }

    Public Virtual SwitchState GetState(Int pin) override {
        pinMode(pin, OUTPUT);  // keep as OUTPUT so relay isn't floating; read back last value
        int raw = digitalRead(pin);
        // Active-low: GPIO LOW = relay ON, GPIO HIGH = relay OFF
        SwitchState state = (raw == LOW) ? SwitchState::On : SwitchState::Off;
        if (logger != nullptr) {
            logger->Info(Tag::Untagged, "[RelayController] GetState: pin=" + std::to_string(pin) +
                " GPIO=" + (raw == HIGH ? "HIGH" : "LOW") + " relay=" + (state == SwitchState::On ? "ON" : "OFF"));
        }
        return state;
    }
};

#endif // RELAYCONTROLLER_H
#endif // ARDUINO

