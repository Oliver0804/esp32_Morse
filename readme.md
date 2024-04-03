# ESP32 + SI5351 Morse Code Transmitter with PlatformIO

This project uses an ESP32 and an SI5351 clock generator to transmit Morse code messages. It leverages PlatformIO as the primary development environment, providing an easy way to manage project dependencies and configurations. Users can input Morse code transmission settings via a web page served by the ESP32 or by sending AT commands over a serial connection.

[Youtube Demo](https://youtube.com/shorts/I7Kv4bjHuSA?feature=share)

![./docs/demo.png](./docs/demo.png)

## Features

Web interface for inputting message, frequency, and duration for Morse code transmission.
Serial interface for receiving AT commands with similar capabilities.
Flexible and easily configurable with PlatformIO.

## Prerequisites

ESP32 development board
SI5351 clock generator module
PlatformIO IDE

## Getting Started

Clone the repository to your local machine.
Open the project with PlatformIO.
Connect the SI5351 module to your ESP32 following the module’s datasheet.
Build and upload the project to your ESP32 board.

## Web Interface

The ESP32 hosts a web server that serves an HTML form for easy input of Morse code transmission parameters.

## Accessing the Web Page

Connect your computer or smartphone to the same network as the ESP32 and navigate to the ESP32's IP address using a web browser.

## Serial Interface

You can also send AT commands to the ESP32 via a serial connection to control Morse code transmission.

## AT Command Format

```
AT+SEND=message,frequency,duration
```

- message: The message to be transmitted in Morse code.
- frequency: Transmission frequency in Hz.
- duration: Duration of each Morse code dot in milliseconds.

# RF Usage Warning

When using RF modules, it's crucial to comply with local regulations concerning radio frequency emissions. Unauthorized or improper use of RF devices may cause harmful interference to other radio communications, leading to legal consequences. Ensure that your use of this project does not disrupt normal RF activities in your vicinity.

# Contributions

Contributions are welcome! Feel free to fork the project and submit a pull request with your improvements.
