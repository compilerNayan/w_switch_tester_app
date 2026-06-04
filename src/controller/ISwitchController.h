#ifndef ISWITCHCONTROLLER_H
#define ISWITCHCONTROLLER_H

#include <StandardDefines.h>

// Forward declarations
template<typename T>
class ResponseEntity;

class SwitchResponseDto;

DefineStandardPointers(ISwitchController)
class ISwitchController {
    Public Virtual ~ISwitchController() = default;

    /**
     * @brief Turn on a switch by ID
     * @param id The switch ID
     * @return ResponseEntity<SwitchResponseDto> with switch details, or 404 if not found
     */
    Public Virtual ResponseEntity<SwitchResponseDto> TurnOnSwitch(Int id) = 0;

    /**
     * @brief Turn off a switch by ID
     * @param id The switch ID
     * @return ResponseEntity<SwitchResponseDto>, or 404 if not found
     */
    Public Virtual ResponseEntity<SwitchResponseDto> TurnOffSwitch(Int id) = 0;

    /**
     * @brief Toggle a switch by ID
     * @param id The switch ID
     * @return ResponseEntity<SwitchResponseDto>, or 404 if not found
     */
    Public Virtual ResponseEntity<SwitchResponseDto> ToggleSwitch(Int id) = 0;

    /**
     * @brief Get switch details by ID
     * @param id The switch ID
     * @return ResponseEntity<SwitchResponseDto>, or 404 if not found
     */
    Public Virtual ResponseEntity<SwitchResponseDto> GetSwitchStateById(Int id) = 0;

    /**
     * @brief Get all switch details
     * @return ResponseEntity<StdVector<SwitchResponseDto>> with all switch details
     */
    Public Virtual ResponseEntity<StdVector<SwitchResponseDto>> GetAllSwitchState() = 0;
};

#include "SwitchController.h"
#endif // ISWITCHCONTROLLER_H

