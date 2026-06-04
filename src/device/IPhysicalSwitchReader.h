#ifndef IPHYSICALSWITCHREADER_H
#define IPHYSICALSWITCHREADER_H

#include <StandardDefines.h>
#include "SwitchState.h"

DefineStandardPointers(IPhysicalSwitchReader)
class IPhysicalSwitchReader {
    Public Virtual ~IPhysicalSwitchReader() = default;

    /**
     * @brief Read the physical state of a switch from a GPIO pin
     * @param pin The GPIO pin number to read from
     * @return SwitchState::On if pin is HIGH, SwitchState::Off if pin is LOW
     */
    Public Virtual SwitchState ReadPhysicalState(Int pin) = 0;
};

#endif // IPHYSICALSWITCHREADER_H

