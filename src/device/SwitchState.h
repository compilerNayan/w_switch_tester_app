#ifndef SWITCHSTATE_H
#define SWITCHSTATE_H

#include <StandardDefines.h>

// Switch state enum
/* Serializable */
DefineStandardTypes(SwitchState)
enum class SwitchState {
    Off,    // Switch is off
    On      // Switch is on
};

#endif // SWITCHSTATE_H

