
#include <Arduino.h>
#include <main.h>

#include <cwmorse.h>
#include "si5351.h"

Si5351 si5351;

int32_t cal_factor = -514000000; // 需要從範例中進行校準

uint16_t duration = 85; // 摩斯電碼的持續時間 - 毫秒
uint16_t morse_duration = 85;

uint16_t hz = 444;                    // 本地Buzz的频率
uint64_t target_freq = 1000000000ULL; // 10Mhz
String cw_message = "Hello World";    // 不必要XD, 只是為了測試

#include <WiFi.h>
#include <WebServer.h>
const char *ssid = "SSID";
const char *password = "PASSWORD";

WebServer server(80); // WebServer Port 80
String inputForm()
{
  return "<!DOCTYPE html><html><head>"
         "<style>"
         "body { font-family: Arial, sans-serif; margin: 20px; padding: 20px; background-color: #f0f0f0; }"
         "form { background-color: #ffffff; padding: 20px; border-radius: 8px; }"
         "input[type=text], input[type=submit] { width: 100%; padding: 12px; margin: 8px 0; display: inline-block; border: 1px solid #ccc; border-radius: 4px; box-sizing: border-box; }"
         "input[type=submit] { width: auto; background-color: #4CAF50; color: white; cursor: pointer; }"
         "input[type=submit]:hover { background-color: #45a049; }"
         "</style>"
         "</head><body>"
         "<h2>Morse Code Sender</h2>"
         "<form action=\"/send\" method=\"post\">"
         "<label for=\"message\">Message:</label>"
         "<input type=\"text\" id=\"message\" name=\"message\"><br>"
         "<label for=\"frequency\">Frequency (Hz):</label>"
         "<input type=\"text\" id=\"frequency\" name=\"frequency\"><br>"
         "<label for=\"duration\">Duration (ms):</label>"
         "<input type=\"text\" id=\"duration\" name=\"duration\"><br>"
         "<input type=\"submit\" value=\"Send\">"
         "</form></body></html>";
}

// 基於設置進行摩斯電碼發送
void sendMorseCode()
{
  duration = morse_duration;
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
    morse_duration = durationMs;

    // 發送摩斯電碼
    sendMorseCode();

    Serial.println("AT Command received. Message: " + cw_message +
                   ", Frequency: " + String(target_freq) + " Hz" + // 转换回MHz单位
                   ", Duration: " + String(morse_duration) + " ms");
  }
}
//----------
void setup()
{
  Serial.begin(115200);

  // TX-CW, TX-LED, 750 Hz sound
  pinMode(PIN_TX, OUTPUT);

  si5351.init(SI5351_CRYSTAL_LOAD_8PF, 0, 0);
  si5351.set_correction(cal_factor, SI5351_PLL_INPUT_XO);
  si5351.set_pll(SI5351_PLL_FIXED, SI5351_PLLA);
  si5351.set_freq(target_freq, SI5351_CLK0);

  cw(false);
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
  server.on("/", HTTP_GET, []()
            {
              server.send(200, "text/html", inputForm()); // 提供HTML表单
            });

  server.on("/send", HTTP_POST, []()
            {            
  String message = server.arg("message");
  String freq = server.arg("frequency");
  String durationStr = server.arg("duration");

  long freqHz = freq.toInt() ; 
  int durationMs = durationStr.toInt();

  if (freqHz <= 0 || durationMs <= 0) {
    server.send(400, "text/plain", "Invalid frequency or duration.");
    return;
  }

  target_freq = freqHz; 
  morse_duration = durationMs; 
  cw_message = message; 
  cw_string_proc(cw_message);


  server.send(200, "text/plain", "Message received: " + message); });
  server.begin();
}
//----------
void loop()
{
  server.handleClient(); // WebServer listening

  // AT Command
  if (Serial.available())
  {
    String cmd = Serial.readStringUntil('\n');
    handleATCommand(cmd);

    cw(false);
  }
}
