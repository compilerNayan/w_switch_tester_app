#ifdef ESP_PLATFORM
#ifndef ESPIDF_RELAYCONTROLLER_H
#define ESPIDF_RELAYCONTROLLER_H

#include <StandardDefines.h>
#include "IRelayController.h"
#include "SwitchState.h"
#include "driver/gpio.h"
#include "ILogger.h"
#include "Tag.h"

/**
 * Relay board is ACTIVE-LOW: GPIO LOW = relay ON (LED on, click), GPIO HIGH = relay OFF (LED off).
 * We invert logic so SwitchState::On -> write LOW, SwitchState::Off -> write HIGH.
 */
/* @Component */
class EspidfRelayController : public IRelayController {
    /* @Autowired */
    Private ILoggerPtr logger;

    Public Virtual ~EspidfRelayController() = default;

    Private Static Void ConfigureOutputPin(Int pin) {
        gpio_config_t ioConf = {};
        ioConf.intr_type = GPIO_INTR_DISABLE;
        ioConf.mode = GPIO_MODE_OUTPUT;
        ioConf.pin_bit_mask = (1ULL << pin);
        ioConf.pull_down_en = GPIO_PULLDOWN_DISABLE;
        ioConf.pull_up_en = GPIO_PULLUP_DISABLE;
        gpio_config(&ioConf);
    }

    Public Virtual Void SetState(Int pin, SwitchState state) override {
        if (logger != nullptr) {
            logger->Info(Tag::Untagged, "[EspidfRelayController] SetState called: pin=" + std::to_string(pin) +
                " requested=" + (state == SwitchState::On ? "ON" : "OFF"));
        }

        ConfigureOutputPin(pin);

        // Active-low: On -> LOW (relay energizes), Off -> HIGH (relay off)
        int gpioValue = (state == SwitchState::On) ? 0 : 1;
        gpio_set_level(static_cast<gpio_num_t>(pin), gpioValue);

        if (logger != nullptr) {
            logger->Info(Tag::Untagged, "[EspidfRelayController] SetState done: pin=" + std::to_string(pin) +
                " relay=" + (state == SwitchState::On ? "ON" : "OFF") +
                " GPIO=" + (gpioValue == 1 ? "HIGH" : "LOW"));
        }
    }

    Public Virtual SwitchState GetState(Int pin) override {
        ConfigureOutputPin(pin);
        int raw = gpio_get_level(static_cast<gpio_num_t>(pin));
        // Active-low: GPIO LOW = relay ON, GPIO HIGH = relay OFF
        SwitchState state = (raw == 0) ? SwitchState::On : SwitchState::Off;
        if (logger != nullptr) {
            logger->Info(Tag::Untagged, "[EspidfRelayController] GetState: pin=" + std::to_string(pin) +
                " GPIO=" + (raw == 1 ? "HIGH" : "LOW") + " relay=" + (state == SwitchState::On ? "ON" : "OFF"));
        }
        return state;
    }
};

#endif // ESPIDF_RELAYCONTROLLER_H
#endif // ESP_PLATFORM
