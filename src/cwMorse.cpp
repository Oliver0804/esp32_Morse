// cwMorse.cpp
#include "cwMorse.h"
#include "Si5351.h"
#include <main.h>

extern Si5351 si5351;

extern uint16_t duration;
extern uint16_t hz;

void cw(bool state)
{
    if (state)
    {
        si5351.output_enable(SI5351_CLK0, 1);
#ifdef LED_ENABLE
        digitalWrite(PIN_TX, HIGH);
#endif
#ifdef BUZZER_ENABLE
        tone(PIN_SP, hz);
#endif
    }
    else
    {
        si5351.output_enable(SI5351_CLK0, 0);
#ifdef LED_ENABLE
        digitalWrite(PIN_TX, LOW);
#endif
#ifdef BUZZER_ENABLE
        noTone(PIN_SP);
#endif
    }
}

void di()
{
    cw(true); // TX di
    delay(duration);
    cw(false); // stop TX di
    delay(duration);
}

void dah()
{
    cw(true); // TX dah
    delay(3 * duration);
    cw(false); // stop TX dah
    delay(duration);
}

void char_space()
{                        // 3x, pause between letters
    delay(2 * duration); // 1x from end of character + 2x from the beginning of new character
}

void word_space()
{                        // 7x, pause between words
    delay(6 * duration); // 1x from end of the word + 6x from the beginning of new word
}

void cw_string_proc(String str)
{ // Processing string to characters
    for (uint8_t j = 0; j < str.length(); j++)
        cw_char_proc(str[j]);
}

void cw_char_proc(char m)
{
    // Morse code representation for characters 'A'-'Z' and '0'-'9', '?', '=', ',', '/'
    static const char *morseCodes[] = {
        ".-#", "-...#", "-.-.#", "-..#", ".#", "..-.#", "--.#", "....#", "..#", ".---#", "-.-#", ".-..#", "--#", "-.#", "---#", ".--.#", "--.-#", ".-.#", "...#", "-#", "..-#", "...-#", ".--#", "-..-#", "-.--#", "--..#",
        ".----#", "..---#", "...--#", "....-#", ".....#", "-....#", "--...#", "---..#", "----.#", "-----#",
        "..--..#", "-...-#", "--..--#", "-..-.#"};

    if (m == ' ')
    {
        word_space();
        return;
    }

    // Convert 'a'-'z' to 'A'-'Z'
    if (m >= 'a' && m <= 'z')
        m -= 32;

    // Map 'A'-'Z' to 0-25, '0'-'9' to 26-35, and '?', '=', ',', '/' to 36-39
    int index = -1;
    if (m >= 'A' && m <= 'Z')
        index = m - 'A';
    else if (m >= '0' && m <= '9')
        index = 26 + (m - '0');
    else
        switch (m)
        {
        case '?':
            index = 36;
            break;
        case '=':
            index = 37;
            break;
        case ',':
            index = 38;
            break;
        case '/':
            index = 39;
            break;
        }

    if (index != -1)
    {
        const char *code = morseCodes[index];
        for (int i = 0; code[i] != '#'; ++i)
        {
            code[i] == '.' ? di() : dah();
        }
        char_space(); // Space after each character except for the last one
    }
}
