#include <M5Stack.h>
#include "BluetoothSerial.h"
#include "esp_bt.h"
#include "esp_bt_main.h"
#include "esp_bt_device.h"

BluetoothSerial SerialBT;
String bluetooth_name = "M5Stack_master";
uint8_t slave_mac_adder[6] = {0x90, 0x15, 0x06, 0xFD, 0xD2, 0xD6};
uint8_t send_data[50] = {0xC0, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0xE0};
uint32_t start_time = 0;
uint32_t send_time = 0;
uint32_t wait_time = 0;
uint32_t receive_time = 0;
uint8_t receive_buffer[50] = {0};

// CPU周波数を最大に設定
void setCpuFrequencyMax() {
    setCpuFrequencyMhz(240);
}

// Bluetoothの設定を最適化
void optimizeBluetooth() {
    // BLEメモリを解放してClassic BTに割り当て
    esp_bt_controller_mem_release(ESP_BT_MODE_BLE);
    
    // Bluetoothコントローラーの設定
    esp_bt_dev_set_device_name(bluetooth_name.c_str());
}

void setup() {
    M5.begin();
    // setCpuFrequencyMax();
    
    // Bluetooth初期化と最適化
    optimizeBluetooth();
    SerialBT.begin(bluetooth_name, true);
    
    Serial.printf("start connect : \r\n");
    bool connected = false;
    while (!connected) {
        connected = SerialBT.connect(slave_mac_adder);
        delay(10); // 短い待機時間
    }
    Serial.printf("success connect : \r\n");
    
    // 接続安定化のための短い待機
    delay(50);
    
    // 送信バッファをフラッシュ
    while (SerialBT.available()) {
        SerialBT.read();
    }
    
    // 送信開始時間を記録して一度に全データを送信
    start_time = millis();
    SerialBT.write(send_data, sizeof(send_data));
    send_time = millis() - start_time;
    start_time = millis();
}

void loop() {
    // 利用可能なデータがある場合、できるだけ多くのデータを一度に読み込む
    if (SerialBT.available() > 0) {
        int available = SerialBT.available();
        int bytes_read = SerialBT.readBytes(receive_buffer, available);
        
        // 終了バイトを検索
        for (int i = 0; i < bytes_read; i++) {
            if (receive_buffer[i] == 0xC0)
            {
                wait_time = millis() - start_time;
                start_time = millis();
            }
            
            if (receive_buffer[i] == 0xE0) {
                // 通信時間を計測
                receive_time = millis() - start_time;

                Serial.printf("send time : %u\r\n", send_time);
                Serial.printf("wait time : %u\r\n", wait_time);
                Serial.printf("receive time : %u\r\n", receive_time);
                
                // 受信データを表示（デバッグ用）
                Serial.printf("receive data : ");
                for (int j = 0; j <= i; j++) {
                    Serial.printf("%u, ", receive_buffer[j]);
                }
                Serial.printf("\r\n");
                
                // 次の送信準備（必要に応じて）
                // send_start_time = millis();
                // SerialBT.write(send_data, sizeof(send_data));
                
                break;
            }
        }
    }
}