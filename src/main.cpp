#include "iot/IIotApp.h"

/* @Autowired */
IIotAppPtr iotApp;

extern "C" void app_main(void) {
    iotApp->Start();
}