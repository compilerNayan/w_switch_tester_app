#include "iot/IIotApp.h"

/*--@Autowired--*/
IIotAppPtr iotApp = Implementation<IIotApp>::type::GetInstance();

extern "C" void app_main(void) {
    iotApp->Start();
}