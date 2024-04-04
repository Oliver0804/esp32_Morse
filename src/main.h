// cwMorse.h
#ifndef MAIN_H
#define MAIN_H

#include <Arduino.h>

#include <WiFi.h>
#include <WebServer.h>
#include <html.h>

#include "si5351.h"
#include <cwMorse.h>
#include <button.h>
extern Si5351 si5351;

extern int32_t cal_factor;

extern uint16_t duration;

extern uint16_t hz;
extern uint64_t target_freq;
extern String cw_message;

// LED
#define LED_ENABLE 1
#define LED_PIN 5
#define PIN_TX LED_PIN

// BUZZER
#define BUZZER_ENABLE 1
#define BUZZER_PIN 26
#define PIN_SP BUZZER_PIN

// BUTTON
#define BUTTON_DI 14
#define BUTTON_DAH 12
#define DEBOUNCE_DELAY 50

#endif