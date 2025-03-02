#include <M5Stack.h>
#include "BluetoothSerial.h"

BluetoothSerial SerialBT;
String bluetooth_name = "M5Stack_master";
uint8_t slave_mac_adder[6] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00} ;
uint8_t send_data[2] = {0xC0, 0xE0};
uint32_t send_start_time = 0;

void setup() {

    bool connected = false;     //!< 接続完了フラグ

    M5.begin();

    SerialBT.begin(bluetooth_name, true);

    Serial.printf("start connect : /r/n");
    while (connected == (bool)true)
    {
        connected = SerialBT.connect(slave_mac_adder);
    }
    Serial.printf("success connect : /r/n");
    
    send_start_time = millis();
    SerialBT.write(send_data, sizeof(send_data));

}

void loop() {
    uint8_t receive_data = 0;

    if (SerialBT.available() > 0)
    {
        receive_data = SerialBT.read();
        Serial.printf("receive data : %u/r/n", receive_data);
        
        if (receive_data == 0xE0)
        {
            Serial.printf("comm time : %u/r/n", (millis() - send_start_time));
        }
        
    }
    
}