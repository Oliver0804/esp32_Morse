#include "main.h"

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT,
                         OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);

extern String morseCode;
String lastMorseCode = "";
char lastdecodedtempChar = '\0';
String displayText = "";
void displayInit()
{

    if (!display.begin(SSD1306_SWITCHCAPVCC))
    {
        Serial.println(F("SSD1306 allocation failed"));
    }
}

void drawLogo()
{
    display.clearDisplay();              // 清除显示屏内容
    display.setTextSize(2);              // 设置文本大小。根据你的显示屏大小调整这个值以适合显示"MORSE"
    display.setTextColor(SSD1306_WHITE); // 设置文本颜色为白色
    display.setCursor(5, 0);             // 设置开始绘制文本的位置。根据需要调整这些值以在显示屏上居中显示"MORSE"
    // 显示"MORSE"

    display.println("MORSE CODE");

    display.setCursor(5, 15); // 设置开始绘制文本的位置。根据需要调整这些值以在显示屏上居中显示"MORSE"
    display.println("-");
    display.setCursor(115, 15); // 设置开始绘制文本的位置。根据需要调整这些值以在显示屏上居中显示"MORSE"
    display.println(".");
    display.display(); // 更新显示屏以显示新的内容
}
void drawClear()
{
    // Serial.println("[Clear]");
    display.clearDisplay();
    display.display();
}
void drawMorseCode(char decodedtempChar)
{
    if ((morseCode != lastMorseCode) || (lastdecodedtempChar != decodedtempChar))
    {
        // Serial.print("[Morse Code]");
        // Serial.println(lastMorseCode);
        lastMorseCode = morseCode;
        display.clearDisplay();              // 清除显示屏内容
        display.setTextSize(2);              // 设置文本大小。根据你的显示屏大小调整这个值以适合显示"MORSE"
        display.setTextColor(SSD1306_WHITE); // 设置文本颜色为白色
        display.setCursor(5, 0);             // 设置开始绘制文本的位置。根据需要调整这些值以在显示屏上居中显示"MORSE"
        // 显示"MORSE"
        if (decodedtempChar != '\0')
        {
            displayText += decodedtempChar;
            display.println(displayText);
            // Serial.println(displayText);
            if (displayText.length() > 9)
            {
                displayText = "";
            }
        }
        else
        {
            display.println(displayText);
        }

        display.setCursor(30, 15); // 设置开始绘制文本的位置。根据需要调整这些值以在显示屏上居中显示"MORSE"
        display.println(morseCode);

        display.setCursor(5, 15); // 设置开始绘制文本的位置。根据需要调整这些值以在显示屏上居中显示"MORSE"
        display.println("-");
        display.setCursor(115, 15); // 设置开始绘制文本的位置。根据需要调整这些值以在显示屏上居中显示"MORSE"
        display.println(".");
        display.display(); // 更新显示屏以显示新的内容
    }
    else
    {

        return;
    }
}
void testdrawline()
{
    int16_t i;

    display.clearDisplay(); // Clear display buffer

    for (i = 0; i < display.width(); i += 4)
    {
        display.drawLine(0, 0, i, display.height() - 1, SSD1306_WHITE);
        display.display(); // Update screen with each newly-drawn line
        delay(1);
    }
    for (i = 0; i < display.height(); i += 4)
    {
        display.drawLine(0, 0, display.width() - 1, i, SSD1306_WHITE);
        display.display();
        delay(1);
    }
    delay(250);

    display.clearDisplay();

    for (i = 0; i < display.width(); i += 4)
    {
        display.drawLine(0, display.height() - 1, i, 0, SSD1306_WHITE);
        display.display();
        delay(1);
    }
    for (i = display.height() - 1; i >= 0; i -= 4)
    {
        display.drawLine(0, display.height() - 1, display.width() - 1, i, SSD1306_WHITE);
        display.display();
        delay(1);
    }
    delay(250);

    display.clearDisplay();

    for (i = display.width() - 1; i >= 0; i -= 4)
    {
        display.drawLine(display.width() - 1, display.height() - 1, i, 0, SSD1306_WHITE);
        display.display();
        delay(1);
    }
    for (i = display.height() - 1; i >= 0; i -= 4)
    {
        display.drawLine(display.width() - 1, display.height() - 1, 0, i, SSD1306_WHITE);
        display.display();
        delay(1);
    }
    delay(250);

    display.clearDisplay();

    for (i = 0; i < display.height(); i += 4)
    {
        display.drawLine(display.width() - 1, 0, 0, i, SSD1306_WHITE);
        display.display();
        delay(1);
    }
    for (i = 0; i < display.width(); i += 4)
    {
        display.drawLine(display.width() - 1, 0, i, display.height() - 1, SSD1306_WHITE);
        display.display();
        delay(1);
    }

    delay(2); // Pause for 2 seconds
}