#include <main.h>
WebServer server(80); // WebServer Port 80

void webServerRun()
{
    handleRoot();
    handleMorseCodeTable();
    handleSendMorse();
    handleOTA();
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

void handleOTA()
{
    server.on("/login", HTTP_GET, []()
              {
    server.sendHeader("Connection", "close");
    server.send(200, "text/html", loginIndex.c_str()); });
    server.on("/serverIndex", HTTP_GET, []()
              {
    server.sendHeader("Connection", "close");
    server.send(200, "text/html", serverIndex.c_str()); });
    /*handling uploading firmware file */
    server.on(
        "/update", HTTP_POST, []()
        {
    server.sendHeader("Connection", "close");
    server.send(200, "text/plain", (Update.hasError()) ? "FAIL" : "OK");
    ESP.restart(); },
        []()
        {
            HTTPUpload &upload = server.upload();
            if (upload.status == UPLOAD_FILE_START)
            {
                Serial.printf("Update: %s\n", upload.filename.c_str());
                if (!Update.begin(UPDATE_SIZE_UNKNOWN))
                { // start with max available size
                    Update.printError(Serial);
                }
            }
            else if (upload.status == UPLOAD_FILE_WRITE)
            {
                /* flashing firmware to ESP*/
                if (Update.write(upload.buf, upload.currentSize) != upload.currentSize)
                {
                    Update.printError(Serial);
                }
            }
            else if (upload.status == UPLOAD_FILE_END)
            {
                if (Update.end(true))
                { // true to set the size to the current progress
                    Serial.printf("Update Success: %u\nRebooting...\n", upload.totalSize);
                }
                else
                {
                    Update.printError(Serial);
                }
            }
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
    "input[type=text], input[type=password], input[type=submit] { width: 100%; padding: 12px; margin: 8px 0; display: inline-block; border: 1px solid #ccc; border-radius: 4px; box-sizing: border-box; background-color: #222; color: #8FBC8F; }\n"
    "input[type=submit] { width: auto; background-color: #4CAF50; color: white; cursor: pointer; }\n"
    "input[type=submit]:hover { background-color: #45a049; }\n"
    "label { color: #8FBC8F; }\n"
    "table { border-collapse: collapse; width: 100%; }\n"
    "th, td { border: 1px solid #dddddd; text-align: left; padding: 8px; }\n"
    "th { background-color: #f2f2f2; }\n"
    "</style>\n";
/*
 * Login page
 */
String loginIndex =
    String(commonCSS) + // 插入共用的CSS样式
    "<form name='loginForm'>"
    "<table>"
    "<tr><td colspan=2><center><font size=4><b> Login Page</b></font></center><br></td></tr>"
    "<tr><td>Username:</td><td><input type='text' name='userid'><br></td></tr>"
    "<tr><td>Password:</td><td><input type='password' name='pwd'><br></td></tr>"
    "<tr><td colspan=2><center><input type='submit' onclick='check(this.form)' value='Login'></center></td></tr>"
    "</table>"
    "<p><p><div style='text-align:center; margin-top:10px; color: #777;'>Compiled on: " __DATE__ " at " __TIME__ "</div>"
    "</form>"
    "<script>"
    "function check(form) {"
    "if(form.userid.value=='admin' && form.pwd.value=='admin') {"
    "window.open('/serverIndex');"
    "} else {"
    "alert('Error Password or Username');"
    "}"
    "}"
    "</script>";

/*
 * Server Index Page
 */
String serverIndex =
    String(commonCSS) + // 插入共用的CSS样式
    "<script src='https://ajax.googleapis.com/ajax/libs/jquery/3.2.1/jquery.min.js'></script>"
    "<form method='POST' action='#' enctype='multipart/form-data' id='upload_form'>"
    "<input type='file' name='update'>"
    "<input type='submit' value='Update'>"
    "<p><p><div id='prg'>progress: 0%</div>"
    "<p><p><div style='text-align:center; margin-top:10px; color: #777;'>Compiled on: " __DATE__ " at " __TIME__ "</div>"
    "</form>"
    "<script>"
    "$('form').submit(function(e){"
    "e.preventDefault();"
    "var form = $('#upload_form')[0];"
    "var data = new FormData(form);"
    " $.ajax({"
    "url: '/update',"
    "type: 'POST',"
    "data: data,"
    "contentType: false,"
    "processData:false,"
    "xhr: function() {"
    "var xhr = new window.XMLHttpRequest();"
    "xhr.upload.addEventListener('progress', function(evt) {"
    "if (evt.lengthComputable) {"
    "var per = evt.loaded / evt.total;"
    "$('#prg').html('progress: ' + Math.round(per*100) + '%');"
    "}"
    "}, false);"
    "return xhr;"
    "},"
    "success:function(d, s) {"
    "console.log('success!')"
    "},"
    "error: function (a, b, c) {"
    "}"
    "});"
    "});"
    "</script>";

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
