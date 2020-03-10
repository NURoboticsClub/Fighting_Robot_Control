#include "PS3/Ps3Controller.h"
#include "PS4/PS4Controller.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_task_wdt.h"
#include "esp_bt.h"

enum controllerType {
  ps3 = 3,
  ps4 = 4,

  none = -1
};

controllerType controller = none;

typedef void(*callback_t)();

// Watchdog
const int loopTimeCtl = 0;
hw_timer_t *timer = NULL;

callback_t _callback_end = nullptr;

void feedWatchdog() {
  timerWrite(timer, 0);
}

void notBeingSafe() {
  printf("HEY, LISTEN! INCLUDE A DISCONNECT FUNCTION!\n");
  while(1) delay(1000);
}

//this runs if the robot code hangs! cut off the motors
void IRAM_ATTR ESTOP(){
  printf("EMERGANCY STOP!\n");
  if (_callback_end) {
    _callback_end();
  }
  ESP.restart();
}

void attachFail(callback_t callback) {
  _callback_end = callback;
}

void safetySetup(callback_t callback, controllerType p) {
  timer = timerBegin(0, 80, true); //timer 0, div 80
  timerAttachInterrupt(timer, &ESTOP, true);
  timerAlarmWrite(timer, 1000000, false); //set time in us, 1 second
  timerAlarmEnable(timer); //enable interrupt

  if (!callback) {
    notBeingSafe();
  }
  attachFail(callback);

  controller = p;

  if (controller == ps3) {
    Ps3.attachOnDisconnect(ESTOP);
    Ps3.activate();
  }
  else if (controller == ps4) {
    PS4.attachOnDisconnect(ESTOP);
    PS4.activate();
  }
}

void safetyLoop() {
  feedWatchdog();

  if (controller == ps3 && Ps3.data.button.ps) {
    Ps3.setLed(0);
    ESTOP();
  }
  if (controller == ps4 && PS4.data.button.ps) {
    PS4.setLed(0,0,0);
    PS4.sendToController();
    ESTOP();
  }

  if (!_callback_end) notBeingSafe();
}
