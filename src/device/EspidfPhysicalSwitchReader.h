#ifdef ESP_PLATFORM
#ifndef ESPIDF_PHYSICALSWITCHREADER_H
#define ESPIDF_PHYSICALSWITCHREADER_H

#include <StandardDefines.h>
#include "IPhysicalSwitchReader.h"
#include "SwitchState.h"
#include "ILogger.h"
#include "Tag.h"
#include "esp_adc/adc_oneshot.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

// ZMPT101B-style voltage detection: two analog samples ~10 ms apart (half 50 Hz cycle).
// If difference >= threshold, AC voltage present (On); else Off.
static const Int kVoltageThreshold = 50;
static const UInt kHalfCycleMs = 10;

/* @Component */
class EspidfPhysicalSwitchReader : public IPhysicalSwitchReader {
    /* @Autowired */
    Private ILoggerPtr logger;

    Private adc_oneshot_unit_handle_t adcHandle = nullptr;
    Private Bool adcInitialized = false;

    Public EspidfPhysicalSwitchReader() = default;

    Public Virtual ~EspidfPhysicalSwitchReader() {
        if (adcHandle != nullptr) {
            adc_oneshot_del_unit(adcHandle);
            adcHandle = nullptr;
            adcInitialized = false;
        }
    }

    Private Void EnsureAdcInit() {
        if (adcInitialized) {
            return;
        }
        adc_oneshot_unit_init_cfg_t initConfig = {
            .unit_id = ADC_UNIT_1,
            .ulp_mode = ADC_ULP_MODE_DISABLE,
        };
        adc_oneshot_new_unit(&initConfig, &adcHandle);
        adcInitialized = true;
    }

    Private Int ReadAnalogSample(Int pin) {
        EnsureAdcInit();

        adc_unit_t unit = ADC_UNIT_1;
        adc_channel_t channel = ADC_CHANNEL_0;
        if (adc_oneshot_io_to_channel(static_cast<gpio_num_t>(pin), &unit, &channel) != ESP_OK) {
            return 0;
        }

        adc_oneshot_chan_cfg_t chanConfig = {
            .atten = ADC_ATTEN_DB_12,
            .bitwidth = ADC_BITWIDTH_DEFAULT,
        };
        adc_oneshot_config_channel(adcHandle, channel, &chanConfig);

        int raw = 0;
        adc_oneshot_read(adcHandle, channel, &raw);
        return raw;
    }

    Public Virtual SwitchState ReadPhysicalState(Int pin) override {
        Int v1 = ReadAnalogSample(pin);
        vTaskDelay(pdMS_TO_TICKS(kHalfCycleMs));
        Int v2 = ReadAnalogSample(pin);
        Int diff = (v1 > v2) ? (v1 - v2) : (v2 - v1);
        Bool hasVoltage = (diff >= kVoltageThreshold);
        SwitchState state = hasVoltage ? SwitchState::On : SwitchState::Off;

        return state;
    }
};

#endif // ESPIDF_PHYSICALSWITCHREADER_H
#endif // ESP_PLATFORM
