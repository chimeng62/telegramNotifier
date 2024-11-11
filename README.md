# TelegramNotifier Library

`TelegramNotifier` is an Arduino library that simplifies the process of sending notifications to a Telegram bot. This library allows you to set up a bot with a chat ID and offers a customizable message formatting option.

## Features
- **Easy configuration**: Initialize with a bot token and chat ID.
- **Custom message formatting**: Use a callback function to create custom message formats.
- **Secure connection**: Utilizes a secure WiFi client with Telegram's root certificate for HTTPS requests.
- **Adjustable response timeout**: Control the waiting period for server responses to avoid duplicate messages.

## Getting Started

### Prerequisites
- Arduino-compatible device with WiFi capabilities (e.g., ESP8266, ESP32).
- Telegram Bot API token.
- Chat ID of the user or group where notifications will be sent.

### Installation
1. Clone or download this repository into your Arduino `libraries` folder.
2. Include the library in your Arduino sketch:
   ```cpp
   #include <TelegramNotifier.h>

### Usage
See example