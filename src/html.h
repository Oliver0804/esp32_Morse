#ifndef HTML_H
#define HTML_H

void webServerRun();
void webServerloop();

void handleRoot();
void handleOTA();
void handleSendMorse();
void handleMorseCodeTable();

// 使用extern声明变量，但不在此定义
extern const char *commonCSS;
extern String serverIndex;
extern String loginIndex;
extern String morseCodeSenderFormHTML;
extern String morseCodeTableHTML;

#endif