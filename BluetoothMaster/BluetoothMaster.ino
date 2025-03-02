#include <M5Stack.h>
#include "BluetoothSerial.h"

BluetoothSerial SerialBT;
String bluetooth_name = "M5Stack_master";
uint8_t slave_mac_adder[6] = {0x90, 0x15, 0x06, 0xFD, 0xD2, 0xD6};
uint8_t send_data[50] = {0xC0, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0xE0};
uint32_t send_start_time = 0;
uint8_t receive_buffer[50] = {0};
uint8_t receive_data_size = 0;

void setup() {

    bool connected = false;     //!< 接続完了フラグ

    M5.begin();

    SerialBT.begin(bluetooth_name, true);

    Serial.printf("start connect : \r\n");
    while (connected == (bool)false)
    {
        connected = SerialBT.connect(slave_mac_adder);
    }
    Serial.printf("success connect : \r\n");
    
    send_start_time = millis();
    SerialBT.write(send_data, sizeof(send_data));

}

void loop() {
    uint8_t receive_data = 0;

    if (SerialBT.available() > 0)
    {
        receive_data = SerialBT.read();
        receive_buffer[receive_data_size] = receive_data; 
        receive_data_size++;

        if (receive_data == 0xE0)
        {
            Serial.printf("comm time : %u\r\n", (millis() - send_start_time));
            Serial.printf("receive data : ");
            for (size_t i = 0; i < receive_data_size; i++)
            {
                Serial.printf("%u, ", receive_buffer[i]);
            }
            Serial.printf("\r\n");
            receive_data_size = 0;
        }
        
    }
    
}