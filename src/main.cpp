
#include <Arduino.h>
#include <main.h>
#include <Wire.h>
#include <cwmorse.h>

Si5351 si5351;

int32_t cal_factor = -514000000; // 需要從範例中進行校準

uint16_t duration = 200; // 摩斯電碼的持續時間 - 毫秒

uint16_t hz = 444;                        // 本地Buzz的频率
uint64_t target_freq = 4428000000ULL;     // 預設CW 100Mhz
String cw_message = "Yuan Shen Chi Dong"; // 不必要XD, 只是為了測試

// WI-FI Connection SSID and Password
#include "wifi_credentials.h"
const char *ssid = WIFI_SSID;
const char *password = WIFI_PASSWORD;

// 基於設置進行摩斯電碼發送
void sendMorseCode()
{
  si5351.set_freq(target_freq * 100, SI5351_CLK0);
  cw_string_proc(cw_message);
  Serial.println("Morse code sent: " + cw_message);
}
// 處理AT命令
void handleATCommand(String cmd)
{
  // 將命令轉換為大寫
  cmd.toUpperCase();

  if (cmd.startsWith("AT+SEND="))
  {
    int firstComma = cmd.indexOf(',');
    int secondComma = cmd.indexOf(',', firstComma + 1);
    cw_message = cmd.substring(8, firstComma);
    long freqHz = cmd.substring(firstComma + 1, secondComma).toInt();
    int durationMs = cmd.substring(secondComma + 1).toInt();

    // 確認頻率範圍主要因為Si5351的限制
    if (freqHz < 8000 || freqHz > 160000000)
    {
      Serial.println(freqHz);
      Serial.println("Frequency setting error. Must be between 8KHz and 160MHz.");
      return;
    }

    // 時間間隔是否大於50ms
    if (durationMs <= 50)
    {
      Serial.println("Duration setting error. Must be greater than 50ms.");
      return;
    }

    // 更新設定
    target_freq = freqHz;
    duration = durationMs;

    // 發送摩斯電碼
    sendMorseCode();

    Serial.println("AT Command received. Message: " + cw_message +
                   ", Frequency: " + String(target_freq) + " Hz" + // 转换回MHz单位
                   ", Duration: " + String(duration) + " ms");
  }
}

void setup()
{
  Serial.begin(115200);
  displayInit();
  drawLogo();
  pinMode(BUTTON_DI, INPUT_PULLUP); // 配置为输入，并启用内部上拉电阻
  pinMode(BUTTON_DAH, INPUT_PULLUP);
#ifdef LED_ENABLE
  pinMode(PIN_TX, OUTPUT);
#endif
  si5351_init();
  // WI-FI Connection
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected.");

  Serial.println("WiFi connected.");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP()); // 打印ESP32的IP地址

  Serial.print("Subnet Mask: ");
  Serial.println(WiFi.subnetMask());

  Serial.print("Gateway: ");
  Serial.println(WiFi.gatewayIP());

  Serial.print("DNS Server: ");
  Serial.println(WiFi.dnsIP());
  webServerRun();
}
//----------
void loop()
{
  webServerloop();

  // AT Command
  if (Serial.available())
  {
    String cmd = Serial.readStringUntil('\n');
    handleATCommand(cmd);
    cw(false);
  }
  buttonEvent();
}
