#include "button.h"
// button
bool keyboardEnabled = true;
unsigned long buttonPressedTime = 0;
const unsigned long longPressTime = 2000;

String morseCode = ""; // 暫存摩斯電碼

void buttonEvent()
{
    if (digitalRead(BUTTON_DI) == LOW && digitalRead(BUTTON_DAH) == LOW)
    {
        if (buttonPressedTime == 0)
        {
            buttonPressedTime = millis();
        }
        else if (millis() - buttonPressedTime > longPressTime)
        {
            keyboardEnabled = !keyboardEnabled;
            buttonPressedTime = 0;
            if (keyboardEnabled)
            {
                Serial.println("Keyboard enabled.");
            }
            else
            {
                Serial.println("Keyboard disabled.");
            }
        }
    }
    else
    {
        buttonPressedTime = 0;
    }
    if (keyboardEnabled)
    {
        // 如果BUTTON_DI被按下，添加"."到morseCode字符串
        if (digitalRead(BUTTON_DI) == LOW && digitalRead(BUTTON_DAH) == HIGH)
        {
            Serial.print(".");
            if (morseCode.length() > MAX_MORSE_CODE_LENGTH)
            {
                Serial.println("\nError: Morse Code too long");
                morseCode = ""; // 清空morseCode字符串
            }
            else
            {
                morseCode += "."; // 累积摩斯电码
            }
            di();
        }
        // 如果BUTTON_DAH被按下，添加"-"到morseCode字符串
        else if (digitalRead(BUTTON_DAH) == LOW && digitalRead(BUTTON_DI) == HIGH)
        {
            Serial.print("-");
            if (morseCode.length() > MAX_MORSE_CODE_LENGTH)
            {
                Serial.println("\nError: Morse Code too long");
                morseCode = ""; // 清空morseCode字符串
            }
            else
            {
                morseCode += "-"; // 累积摩斯电码
            }
            dah();
        }

        else if (!morseCode.isEmpty() && digitalRead(BUTTON_DI) == HIGH && digitalRead(BUTTON_DAH) == HIGH)
        {
            // Serial.print("morseCodeRaw: ");
            // Serial.println(morseCode);
            char decodedChar = cw_morse2char_proc(morseCode); // 解析摩斯电码
            if (decodedChar != '\0')
            {
                Serial.println();
                Serial.print("Decoded: ");
                Serial.println(decodedChar); // 打印解析结果
            }
            else
            { // 如果解析失败
                Serial.println("\nError: Unknown Morse Code");
            }
            morseCode = "";
            delay(duration);
        }
    }
}