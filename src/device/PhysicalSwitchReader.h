#ifdef ARDUINO
#ifndef PHYSICALSWITCHREADER_H
#define PHYSICALSWITCHREADER_H

#include <StandardDefines.h>
#include "IPhysicalSwitchReader.h"
#include "SwitchState.h"
#include "ILogger.h"
#include "Tag.h"
#include <Arduino.h>

// ZMPT101B-style voltage detection: two analog samples ~10 ms apart (half 50 Hz cycle).
// If difference >= threshold, AC voltage present (On); else Off.
static const Int kVoltageThreshold = 50;
static const UInt kHalfCycleMs = 10;

/* @Component */
class PhysicalSwitchReader : public IPhysicalSwitchReader {
    /* @Autowired */
    Private ILoggerPtr logger;

    Public PhysicalSwitchReader() = default;

    Public Virtual ~PhysicalSwitchReader() = default;

    Public Virtual SwitchState ReadPhysicalState(Int pin) override {
        pinMode(pin, INPUT);

        Int v1 = analogRead(pin);
        delay(kHalfCycleMs);
        Int v2 = analogRead(pin);
        Int diff = (v1 > v2) ? (v1 - v2) : (v2 - v1);
        Bool hasVoltage = (diff >= kVoltageThreshold);
        SwitchState state = hasVoltage ? SwitchState::On : SwitchState::Off;

        return state;
    }
};

#endif // PHYSICALSWITCHREADER_H
#endif // ARDUINO

