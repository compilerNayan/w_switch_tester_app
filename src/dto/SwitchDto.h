#ifndef SWITCHDTO_H
#define SWITCHDTO_H

#include <StandardDefines.h>
#include "../device/SwitchState.h"

/* @Serializable */
class SwitchDto {
    Public optional<Int> id;
    Public optional<SwitchState> switchState;

    /**
     * @brief Default constructor
     */
    Public SwitchDto() : id(), switchState() {}

    /**
     * @brief Parameterized constructor
     * @param id The switch ID
     * @param switchState The switch state
     */
    Public SwitchDto(Int id, SwitchState switchState) : id(id), switchState(switchState) {}
};

#endif // SWITCHDTO_H

