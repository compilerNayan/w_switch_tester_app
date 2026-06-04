#ifndef DEVICECOLLECTION_H
#define DEVICECOLLECTION_H

#include <StandardDefines.h>
#include "IDeviceCollection.h"
#include "IDeviceInfoProvider.h"
#include "ISwitchDevice.h"
#include "SwitchDevice.h"
#include "DeviceDetail.h"

/* @Component */
class DeviceCollection : public IDeviceCollection {
    Private StdMap<Int, ISwitchDevicePtr> devices;

    /* @Autowired */
    Private IDeviceInfoProviderPtr deviceInfoProvider;

    Public DeviceCollection() {
        // Initialize devices from DeviceInfoProvider
        StdVector<DeviceDetail> deviceDetails = deviceInfoProvider->GetAllSwitchDetails();
        
        // Create switch devices for each device detail
        for (const DeviceDetail& detail : deviceDetails) {
            ISwitchDevicePtr switchDevice(new SwitchDevice(detail.id, detail.relayPin, detail.switchPin));
            devices[detail.id] = switchDevice;
        }
    }

    Public Virtual ~DeviceCollection() = default;

    Public Virtual Void RefreshAllDevices() override {
        // Call Refresh() on all devices one after the other
        for (auto& pair : devices) {
            if (pair.second != nullptr) {
                pair.second->Refresh();
            }
        }
    }

    Public Virtual ISwitchDevicePtr GetSwitchDeviceById(Int id) override {
        // Find device in map by ID
        auto it = devices.find(id);
        if (it != devices.end()) {
            return it->second;
        }
        return nullptr;
    }
};

#endif // DEVICECOLLECTION_H

