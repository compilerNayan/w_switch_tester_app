#ifndef DEVICEDETAIL_H
#define DEVICEDETAIL_H

#include <StandardDefines.h>

class DeviceDetail {
    Public Int id;
    Public Int relayPin;
    Public Int switchPin;

    /**
     * @brief Default constructor
     */
    Public DeviceDetail() : id(0), relayPin(0), switchPin(0) {}

    /**
     * @brief Parameterized constructor
     * @param id The device ID
     * @param relayPin The relay pin number
     * @param switchPin The physical switch pin number
     */
    Public DeviceDetail(Int id, Int relayPin, Int switchPin) : id(id), relayPin(relayPin), switchPin(switchPin) {}
};

#endif // DEVICEDETAIL_H

