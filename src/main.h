// cwMorse.h
#ifndef MAIN_H
#define MAIN_H

#include <Arduino.h>

#include <WiFi.h>
#include <WebServer.h>
#include <html.h>

#include <ESP2SOTA.h>

#include "si5351.h"
#include <cwMorse.h>
#include <button.h>
#include <display.h>

extern Si5351 si5351;

extern int32_t cal_factor;

extern uint16_t duration;

extern uint16_t hz;
extern uint64_t target_freq;
extern String cw_message;

// LED
#define LED_ENABLE 1
#define LED_PIN 2
#define PIN_TX LED_PIN

// BUZZER
#define BUZZER_ENABLE 1
#define BUZZER_PIN 23
#define PIN_SP BUZZER_PIN
#define di_hz 2055
#define dah_hz 2030
#define err_hz 440
// BUTTON
#define BUTTON_DI 26
#define BUTTON_DAH 25
#define DEBOUNCE_DELAY 50

// OLED SPI
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define OLED_MOSI 14
#define OLED_CLK 27
#define OLED_DC 13
#define OLED_CS 0
#define OLED_RESET 12
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels
#define NUMFLAKES 10     // Number of snowflakes in the animation example

#define LOGO_HEIGHT 16
#define LOGO_WIDTH 16
#endif