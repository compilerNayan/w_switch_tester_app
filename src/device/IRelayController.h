#ifndef IRELAYCONTROLLER_H
#define IRELAYCONTROLLER_H

#include <StandardDefines.h>
#include "SwitchState.h"

DefineStandardPointers(IRelayController)
class IRelayController {
    Public Virtual ~IRelayController() = default;

    /**
     * @brief Set the state of the relay switch
     * @param pin The GPIO pin number to control
     * @param state The desired state (SwitchState::On or SwitchState::Off)
     */
    Public Virtual Void SetState(Int pin, SwitchState state) = 0;

    /**
     * @brief Get the current state of the relay switch
     * @param pin The GPIO pin number to read
     * @return SwitchState::On if relay is on, SwitchState::Off if off
     */
    Public Virtual SwitchState GetState(Int pin) = 0;
};

#endif // IRELAYCONTROLLER_H

