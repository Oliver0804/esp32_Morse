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
void cw_char2morse_proc(char m);

char cw_morse2char_proc(const String &morseCode);
const int MAX_MORSE_CODE_LENGTH = 6;
struct MorseCodeMapping
{
    const char *code; // 摩斯电码字符串
    char character;   // 对应的字符
};
const MorseCodeMapping morseCodeMappings[] = {
    {".-", 'A'},     // A
    {"-...", 'B'},   // B
    {"-.-.", 'C'},   // C
    {"-..", 'D'},    // D
    {".", 'E'},      // E
    {"..-.", 'F'},   // F
    {"--.", 'G'},    // G
    {"....", 'H'},   // H
    {"..", 'I'},     // I
    {".---", 'J'},   // J
    {"-.-", 'K'},    // K
    {".-..", 'L'},   // L
    {"--", 'M'},     // M
    {"-.", 'N'},     // N
    {"---", 'O'},    // O
    {".--.", 'P'},   // P
    {"--.-", 'Q'},   // Q
    {".-.", 'R'},    // R
    {"...", 'S'},    // S
    {"-", 'T'},      // T
    {"..-", 'U'},    // U
    {"...-", 'V'},   // V
    {".--", 'W'},    // W
    {"-..-", 'X'},   // X
    {"-.--", 'Y'},   // Y
    {"--..", 'Z'},   // Z
    {".----", '1'},  // 1
    {"..---", '2'},  // 2
    {"...--", '3'},  // 3
    {"....-", '4'},  // 4
    {".....", '5'},  // 5
    {"-....", '6'},  // 6
    {"--...", '7'},  // 7
    {"---..", '8'},  // 8
    {"----.", '9'},  // 9
    {"-----", '0'},  // 0
    {"..--..", '?'}, // ?
    {"-...-", '='},  // =
    {"--..--", ','}, // ,
    {"-..-.", '/'},  // /
    {"", '\0'}       // 结束标志
};

#endif