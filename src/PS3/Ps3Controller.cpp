#include "Ps3Controller.h"

#include <esp_bt_main.h>
#include <esp_bt_defs.h>

extern "C" {
#include  "ps3.h"
}

Ps3Controller::Ps3Controller() {}


bool Ps3Controller::begin()
{
    ps3SetEventObjectCallback(this, &Ps3Controller::_event_callback);
    ps3SetConnectionObjectCallback(this, &Ps3Controller::_connection_callback);

    if(!btStarted() && !btStart()){
        log_e("btStart failed");
        return false;
    }

    esp_bluedroid_status_t bt_state = esp_bluedroid_get_status();
    if(bt_state == ESP_BLUEDROID_STATUS_UNINITIALIZED){
        if (esp_bluedroid_init()) {
            log_e("esp_bluedroid_init failed");
            return false;
        }
    }

    if(bt_state != ESP_BLUEDROID_STATUS_ENABLED){
        if (esp_bluedroid_enable()) {
            log_e("esp_bluedroid_enable failed");
            return false;
        }
    }

    if (activated) {
      ps3Init();
      return true;
    }
    else {
      return false;
    }

}


bool Ps3Controller::begin(char *mac)
{
    esp_bd_addr_t addr;

    #define ESP_BD_ADDR_HEX_PTR(addr) (uint8_t*)addr+0, (uint8_t*)addr+1, (uint8_t*)addr+2, (uint8_t*)addr+3, (uint8_t*)addr+4, (uint8_t*)addr+5
    if (sscanf(mac, ESP_BD_ADDR_STR, ESP_BD_ADDR_HEX_PTR(addr)) != ESP_BD_ADDR_LEN){
        log_e("Could not convert %s\n to a MAC address", mac);
        return false;
    }

    ps3SetBluetoothMacAddress( addr );

    return begin();

}


bool Ps3Controller::end()
{
    // TODO

}


bool Ps3Controller::isConnected()
{
    return ps3IsConnected();

}

void Ps3Controller::activate()
{
  activated = true;
}


void Ps3Controller::setLed(int led)
{
    ps3SetLed(led);

}


void Ps3Controller::attach(callback_t callback)
{
    _callback_event = callback;

}


void Ps3Controller::attachOnConnect(callback_t callback)
{
    _callback_connect = callback;

}

void Ps3Controller::attachOnDisconnect(callback_t callback)
{
    _callback_disconnect = callback;

}


void Ps3Controller::_event_callback(void *object, ps3_t data, ps3_event_t event)
{
    Ps3Controller* This = (Ps3Controller*) object;

    memcpy(&This->data, &data, sizeof(ps3_t));
    memcpy(&This->event, &event, sizeof(ps3_event_t));
    This->wasConnected = true;

    if (This->_callback_event){
        This->_callback_event();
    }
}


void Ps3Controller::_connection_callback(void *object, uint8_t is_connected)
{
    Ps3Controller* This = (Ps3Controller*) object;

    if (is_connected)  {
        delay(250);    // ToDo: figure out how to know when the channel is free again so this delay can be removed

        // Set LED1 by default
        This->setLed(1);

        if (This->_callback_connect){
            This->_callback_connect();
        }
    }
    else if (This->wasConnected) {
        if (This->_callback_disconnect){
            This->_callback_disconnect();
        }
    }

}

#if !defined(NO_GLOBAL_INSTANCES)
Ps3Controller Ps3;
#endif
