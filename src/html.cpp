#include <main.h>

WebServer server(80); // WebServer Port 80

extern uint64_t target_freq;
extern String cw_message;

void webServerRun()
{
    handleRoot();
    handleMorseCodeTable();
    handleSendMorse();
    server.begin();
}

void webServerloop()
{
    server.handleClient(); // WebServer listening
}
void handleMorseCodeTable()
{
    server.on("/morse", HTTP_GET, []()
              {
                  server.send(200, "text/html", morseCodeTableHTML.c_str()); // 提供HTML表格
              });
}
void handleRoot()
{
    server.on("/", HTTP_GET, []()
              {
                  server.send(200, "text/html", morseCodeSenderFormHTML.c_str()); // 提供HTML表单
              });
}
void handleSendMorse()
{
    server.on("/send", HTTP_POST, []()
              {
        String message = server.arg("message");
        String freq = server.arg("frequency");
        String durationStr = server.arg("duration");

        long freqHz = freq.toInt();
        int durationMs = durationStr.toInt();

        if (freqHz <= 0 || durationMs <= 0)
        {
            server.send(400, "text/plain", "Invalid frequency or duration.");
            return;
        }

        target_freq = freqHz;
        duration = durationMs;
        cw_message = message;
        cw_string_proc(cw_message);

         server.send(200, "text/plain", "Message Send : " + message); });
}

const char *commonCSS =
    "<style>\n"
    "body { font-family: 'Courier New', monospace; margin: 20px; padding: 20px; background-color: #333; color: #8FBC8F; }\n"
    "form { background-color: #2F4F4F; padding: 20px; border-radius: 8px; }\n"
    "input[type=text], input[type=submit] { width: 100%; padding: 12px; margin: 8px 0; display: inline-block; border: 1px solid #ccc; border-radius: 4px; box-sizing: border-box; }\n"
    "input[type=text] { background-color: #222; color: #8FBC8F; }\n"
    "input[type=submit] { width: auto; background-color: #4CAF50; color: white; cursor: pointer; }\n"
    "input[type=submit]:hover { background-color: #45a049; }\n"
    "label { color: #8FBC8F; }\n"
    "table { border-collapse: collapse; width: 100%; }\n"
    "th, td { border: 1px solid #dddddd; text-align: left; padding: 8px; }\n"
    "th { background-color: #f2f2f2; }"
    "</style>\n";

String morseCodeSenderFormHTML =
    "<!DOCTYPE html><html><head>\n" +
    String(commonCSS) + // 插入共用的CSS样式
    "</head><body>\n"
    "<h2>Morse Code Sender</h2>\n"
    "<form action=\"/send\" method=\"post\">\n"
    "<label for=\"message\">Message:</label>\n"
    "<input type=\"text\" id=\"message\" name=\"message\" value=\"Yuan Shen Chi Dong\"><br>\n"
    "<label for=\"frequency\">Frequency (MHz):</label>\n"
    "<input type=\"text\" id=\"frequency\" name=\"frequency\" value=\"10.100\"><br>\n"
    "<label for=\"duration\">Duration (ms):</label>\n"
    "<input type=\"text\" id=\"duration\" name=\"duration\" value=\"85\"><br>\n"
    "<input type=\"submit\" value=\"Send\">\n"
    "</form></body></html>";

// MorseCodeTable.h
// use wiki svg https://en.wikipedia.org/wiki/Morse_code#/media/File:International_Morse_Code.svg
String morseCodeTableHTML =
    "<!DOCTYPE html>\n"
    "<html>\n"
    "<head>\n" +
    String(commonCSS) + // 插入共用的CSS样式
    "    <title>Morse Code Table</title>\n"
    "</head>\n"
    "<body>\n"
    "\n"
    "<h2>Morse Code Table</h2>\n"
    "Wiki link \n"
    "<img src=\"https://upload.wikimedia.org/wikipedia/commons/b/b5/International_Morse_Code.svg\" alt=\"International Morse Code\">\n"
    "\n"
    "</body>\n"
    "</html>\n";
