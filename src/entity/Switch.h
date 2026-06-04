#ifndef SWITCH_H
#define SWITCH_H

#include <StandardDefines.h>
#include "../device/SwitchState.h"

/* @Entity */
class Switch {

    /* @Id */
    /* @NotNull */
    Public optional<int> id;

    Public optional<SwitchState> virtualState;

    /**
     * @brief Default constructor
     */
    Public Switch() : id(), virtualState() {}

    /**
     * @brief Parameterized constructor
     * @param id The switch ID
     * @param virtualState The virtual state of the switch
     */
    Public Switch(optional<int> id, optional<SwitchState> virtualState) 
        : id(id), virtualState(virtualState) {}

    /**
     * @brief Get the switch ID
     * @return The switch ID
     */
    Public optional<int> GetId() const {
        return id;
    }

    /**
     * @brief Set the switch ID
     * @param id The switch ID to set
     */
    Public Void SetId(optional<int> id) {
        this->id = id;
    }

    /**
     * @brief Get the virtual state
     * @return The virtual state of the switch
     */
    Public optional<SwitchState> GetVirtualState() const {
        return virtualState;
    }

    /**
     * @brief Set the virtual state
     * @param virtualState The virtual state to set
     */
    Public Void SetVirtualState(optional<SwitchState> virtualState) {
        this->virtualState = virtualState;
    }
};

#endif

