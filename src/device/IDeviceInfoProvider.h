#ifndef IDEVICEINFOPROVIDER_H
#define IDEVICEINFOPROVIDER_H

#include <StandardDefines.h>
#include "DeviceDetail.h"

DefineStandardPointers(IDeviceInfoProvider)
class IDeviceInfoProvider {
    Public Virtual ~IDeviceInfoProvider() = default;

    /**
     * @brief Get all switch details
     * @return Vector of DeviceDetail objects containing id and pin for each switch
     */
    Public Virtual StdVector<DeviceDetail> GetAllSwitchDetails() = 0;
};

#endif // IDEVICEINFOPROVIDER_H

