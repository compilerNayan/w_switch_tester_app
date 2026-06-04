#ifndef ISWITCHDEVICE_H
#define ISWITCHDEVICE_H

#include <StandardDefines.h>
#include "SwitchState.h"
#include "../dto/SwitchResponseDto.h"

DefineStandardPointers(ISwitchDevice)
class ISwitchDevice {
    Public Virtual ~ISwitchDevice() = default;

    /**
     * @brief Get the switch ID
     * @return The switch ID
     */
    Public Virtual Int GetId() const = 0;

    /**
     * @brief Turn on the switch
     * @return The final relay state after turning on
     */
    Public Virtual SwitchState TurnOn() = 0;

    /**
     * @brief Turn off the switch
     * @return The final relay state after turning off
     */
    Public Virtual SwitchState TurnOff() = 0;

    /**
     * @brief Toggle the switch state
     * @return The final relay state after toggling
     */
    Public Virtual SwitchState Toggle() = 0;

    /**
     * @brief Get the current actual state of the switch
     * @return SwitchState::On if both virtual and physical states match, SwitchState::Off otherwise
     */
    Public Virtual SwitchState GetState() = 0;

    /**
     * @brief Get the current relay state
     * @return The current state of the relay (On/Off)
     */
    Public Virtual SwitchState GetRelayState() const = 0;

    /**
     * @brief Get the current physical switch state (from sensor/pin)
     * @return The state read from the physical switch input
     */
    Public Virtual SwitchState GetPhysicalSwitchState() = 0;

    /**
     * @brief Get the current virtual (desired) state
     * @return The stored virtual state of the switch
     */
    Public Virtual SwitchState GetVirtualState() const = 0;

    /**
     * @brief Get switch details as a DTO (id, virtualState, physicalSwitchState, relayState)
     * @return SwitchResponseDto built from GetId(), GetVirtualState(), GetPhysicalSwitchState(), GetRelayState()
     */
    Public Virtual SwitchResponseDto GetSwitchDetails() = 0;

    /**
     * @brief Refresh the relay state based on current actual state
     * Updates relay state if it doesn't match the current actual state
     */
    Public Virtual Void Refresh() = 0;
};

#endif // ISWITCHDEVICE_H

