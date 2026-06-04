#ifndef ARDUINO
#ifndef STUBPHYSICALSWITCHREADER_H
#define STUBPHYSICALSWITCHREADER_H

#include <StandardDefines.h>
#include "IPhysicalSwitchReader.h"
#include "SwitchState.h"
#include "ILogger.h"
#include "Tag.h"
#include <map>

/* @Component */
class StubPhysicalSwitchReader : public IPhysicalSwitchReader {
    Private StdMap<Int, SwitchState> pinStates;

    /* @Autowired */
    Private ILoggerPtr logger;

    Public StubPhysicalSwitchReader() {
        // Initialize pins 100 to 110 to OFF
        for (Int pin = 100; pin <= 110; pin++) {
            pinStates[pin] = SwitchState::Off;
        }
    }

    Public Virtual ~StubPhysicalSwitchReader() = default;

    Public Virtual SwitchState ReadPhysicalState(Int pin) override {
        // Return stored state, default to Off if not set
        SwitchState state = (pinStates.find(pin) != pinStates.end()) ? pinStates[pin] : SwitchState::Off;
        
        if (logger != nullptr) {
            StdString message = "Read physical state from pin " + std::to_string(pin) + ": " + 
                               (state == SwitchState::On ? "ON" : "OFF");
            logger->Info(Tag::Untagged, message);
        }
        
        return state;
    }
};

#endif // STUBPHYSICALSWITCHREADER_H
#endif // ARDUINO

