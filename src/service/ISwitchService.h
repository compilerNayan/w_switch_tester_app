#ifndef ISWITCHSERVICE_H
#define ISWITCHSERVICE_H

#include <StandardDefines.h>
#include "../device/SwitchState.h"
#include "../dto/SwitchResponseDto.h"

DefineStandardPointers(ISwitchService)
class ISwitchService {
    Public Virtual ~ISwitchService() = default;

    /**
     * @brief Turn on a switch by ID
     * @param id The switch ID
     * @return SwitchResponseDto (id, virtualState, physicalSwitchState, relayState), or empty optional if not found
     */
    Public Virtual optional<SwitchResponseDto> TurnOnSwitch(Int id) = 0;

    /**
     * @brief Turn off a switch by ID
     * @param id The switch ID
     * @return SwitchResponseDto, or empty optional if not found
     */
    Public Virtual optional<SwitchResponseDto> TurnOffSwitch(Int id) = 0;

    /**
     * @brief Toggle a switch by ID
     * @param id The switch ID
     * @return SwitchResponseDto, or empty optional if not found
     */
    Public Virtual optional<SwitchResponseDto> ToggleSwitch(Int id) = 0;

    /**
     * @brief Get switch details by ID
     * @param id The switch ID
     * @return SwitchResponseDto, or empty optional if not found
     */
    Public Virtual optional<SwitchResponseDto> GetSwitchStateById(Int id) = 0;

    /**
     * @brief Get all switch details
     * @return Vector of SwitchResponseDto (id, virtualState, physicalSwitchState, relayState) for each switch
     */
    Public Virtual StdVector<SwitchResponseDto> GetAllSwitchState() = 0;

    /**
     * @brief Refresh all switches: update each switch's relay to match its current actual state (ISwitchDevice::Refresh)
     */
    Public Virtual Void RefreshAllSwitches() = 0;
};

#endif // ISWITCHSERVICE_H

