// cwMorse.h
#ifndef CWMORSE_H
#define CWMORSE_H

#include <Arduino.h>

void di();
void dah();
void char_space();
void word_space();
void cw(bool state);
void cw_string_proc(String str);
void cw_char_proc(char m);

#endif