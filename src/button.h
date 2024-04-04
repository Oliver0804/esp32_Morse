#ifndef BUTTON_H
#define BUTTON_H

#include <Arduino.h>
#include <main.h>
void buttonEvent();
extern String morseCode; // 暫存摩斯電碼
extern bool keyboardEnabled;
extern unsigned long buttonPressedTime;
extern const unsigned long longPressTime;
#endif
