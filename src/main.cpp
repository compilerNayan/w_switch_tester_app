#include "iot/IIotApp.h"

#include "IDeviceManager.h"
#include "device/IDeviceCollection.h"

/* @Autowired */
IDeviceCollectionPtr deviceCollection;

Void DeviceManager::Setup() {
}

Void DeviceManager::Loop() {
    deviceCollection->RefreshAllDevices();
}

/* @Autowired */
IIotAppPtr iotApp;

extern "C" void app_main(void) {
    iotApp->Start();
}