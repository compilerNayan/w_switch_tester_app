#ifndef SWITCHSERVICE_H
#define SWITCHSERVICE_H

#include <StandardDefines.h>
#include "ISwitchService.h"
#include "../device/IDeviceCollection.h"
#include "../device/ISwitchDevice.h"
#include "../dto/SwitchResponseDto.h"
#include "../device/SwitchState.h"

/* @Service */
class SwitchService : public ISwitchService {
    /* @Autowired */
    Private IDeviceCollectionPtr deviceCollection;

    Public SwitchService() = default;

    Public Virtual ~SwitchService() = default;

    Public Virtual optional<SwitchResponseDto> TurnOnSwitch(Int id) override {
        ISwitchDevicePtr device = deviceCollection->GetSwitchDeviceById(id);
        if (device == nullptr) {
            return optional<SwitchResponseDto>();
        }
        device->TurnOn();
        return optional<SwitchResponseDto>(device->GetSwitchDetails());
    }

    Public Virtual optional<SwitchResponseDto> TurnOffSwitch(Int id) override {
        ISwitchDevicePtr device = deviceCollection->GetSwitchDeviceById(id);
        if (device == nullptr) {
            return optional<SwitchResponseDto>();
        }
        device->TurnOff();
        return optional<SwitchResponseDto>(device->GetSwitchDetails());
    }

    Public Virtual optional<SwitchResponseDto> ToggleSwitch(Int id) override {
        ISwitchDevicePtr device = deviceCollection->GetSwitchDeviceById(id);
        if (device == nullptr) {
            return optional<SwitchResponseDto>();
        }
        device->Toggle();
        return optional<SwitchResponseDto>(device->GetSwitchDetails());
    }

    Public Virtual optional<SwitchResponseDto> GetSwitchStateById(Int id) override {
        ISwitchDevicePtr device = deviceCollection->GetSwitchDeviceById(id);
        if (device == nullptr) {
            return optional<SwitchResponseDto>();
        }
        return optional<SwitchResponseDto>(device->GetSwitchDetails());
    }

    Public Virtual StdVector<SwitchResponseDto> GetAllSwitchState() override {
        StdVector<SwitchResponseDto> result;
        for (Int i = 1; i <= 3; i++) {
            ISwitchDevicePtr device = deviceCollection->GetSwitchDeviceById(i);
            if (device != nullptr) {
                result.push_back(device->GetSwitchDetails());
            }
        }
        return result;
    }

    Public Virtual Void RefreshAllSwitches() override {
        for (Int i = 1; i <= 4; i++) {
            ISwitchDevicePtr device = deviceCollection->GetSwitchDeviceById(i);
            if (device != nullptr) {
                device->Refresh();
            }
        }
    }
};

#endif // SWITCHSERVICE_H

