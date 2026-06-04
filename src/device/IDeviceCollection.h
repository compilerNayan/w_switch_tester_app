#ifndef IDEVICECOLLECTION_H
#define IDEVICECOLLECTION_H

#include <StandardDefines.h>
#include "ISwitchDevice.h"

DefineStandardPointers(IDeviceCollection)
class IDeviceCollection {
    Public Virtual ~IDeviceCollection() = default;

    /**
     * @brief Refresh all devices in the collection
     * Calls Refresh() on each device one after the other
     */
    Public Virtual Void RefreshAllDevices() = 0;

    /**
     * @brief Get a switch device by its ID
     * @param id The device ID
     * @return Pointer to the switch device, or nullptr if not found
     */
    Public Virtual ISwitchDevicePtr GetSwitchDeviceById(Int id) = 0;
};

#endif // IDEVICECOLLECTION_H

