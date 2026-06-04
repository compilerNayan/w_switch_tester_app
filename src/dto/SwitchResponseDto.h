#ifndef SWITCHRESPONSEDTO_H
#define SWITCHRESPONSEDTO_H

#include <StandardDefines.h>
#include "../device/SwitchState.h"

/* @Serializable */
class SwitchResponseDto {
    Public optional<Int> id;
    Public optional<SwitchState> virtualState;
    Public optional<SwitchState> physicalSwitchState;
    Public optional<SwitchState> relayState;

    /**
     * @brief Default constructor
     */
    Public SwitchResponseDto() : id(), virtualState(), physicalSwitchState(), relayState() {}

    /**
     * @brief Parameterized constructor
     * @param id The switch ID
     * @param virtualState The virtual (desired) state
     * @param physicalSwitchState The physical switch state from sensor/pin
     * @param relayState The relay state
     */
    Public SwitchResponseDto(CInt id, SwitchState virtualState, SwitchState physicalSwitchState, SwitchState relayState)
        : id(id), virtualState(virtualState), physicalSwitchState(physicalSwitchState), relayState(relayState) {}
};

#endif // SWITCHRESPONSEDTO_H
