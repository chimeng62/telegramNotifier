#ifndef TELEGRAM_NOTIFIER_H
#define TELEGRAM_NOTIFIER_H

#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <functional>

class TelegramNotifier {
public:

    //how long to wait for response from telegram server like sending message success
    int waitForResponse = 8000; // //default to 8 seconds
    // Define the callback type for custom message formatting
    typedef std::function<String(const String&, const String&, const String&)> MessageFormatterCallback;

    /**
     * @brief Constructor to initialize the TelegramNotifier object.
     *
     * This constructor only initializes the bot with default values. 
     * The configuration (botToken, chatID, messageFormatter) will be set in the setup().
     */
    TelegramNotifier() : telegramBot("", secured_client), messageFormatter(nullptr) {
        secured_client.setCACert(TELEGRAM_CERTIFICATE_ROOT); // Add root certificate for Telegram
        telegramBot.waitForResponse = waitForResponse;
    }

    /**
     * @brief Set the bot configuration.
     *
     * This method sets the bot token, chat ID, and an optional custom message formatter.
     *
     * @param token The Telegram bot token.
     * @param chat_id The chat ID to send messages to.
     * @param formatter A custom message formatter (optional).
     */
    void setConfig(const String& token, const String& chat_id, MessageFormatterCallback formatter = nullptr) {
        botToken = token;
        chatID = chat_id;
        telegramBot = UniversalTelegramBot(botToken, secured_client);
        messageFormatter = formatter; // Set custom message formatter
    }

    //this is a fix for duplicating telegram bot message
    void setWaitForResponse(const int& responseTime){
        waitForResponse = responseTime;
    }

    /**
     * @brief Sends a Telegram message using the default message formatting.
     *
     * This method constructs a message based on a label and state. You can optionally include a duration.
     * It uses the `constructDefaultMessage()` method to build the message and sends it to the specified chat.
     *
     * @param label The label (device or area name) to include in the message (e.g., "first_floor").
     * @param state The state (e.g., "on", "off") of the device or area.
     * @param duration The optional duration (in seconds) to include in the message.
     */
    void sendTelegramMessage(const String& label, const String& state, const String& duration = "") {
        String message;

        // If a custom formatter is provided, use it to format the message
        if (messageFormatter) {
            message = messageFormatter(label, state, duration);
        } else {
            // Default formatting if no callback is provided
            message = constructDefaultMessage(label, state, duration);
        }

        // Send the message to Telegram
        telegramBot.sendMessage(chatID, message, "");
        Serial.println("Sent Telegram message: " + message);
    }

    /**
     * @brief Sets a custom callback function for formatting the message.
     *
     * This function allows you to define a custom way to format the message 
     * before sending it to Telegram. You can use a callback function to tailor 
     * the message to your needs.
     *
     * @param formatter A lambda function or callback that takes a label, state, 
     *                  and optionally other parameters, and returns a formatted message.
     */
    void setMessageFormatter(MessageFormatterCallback formatter) {
        this->messageFormatter = formatter;
    }

private:
    String botToken;  ///< The bot token to authenticate the bot.
    String chatID;    ///< The chat ID to send messages to.
    WiFiClientSecure secured_client;  ///< The secure WiFi client for HTTPS requests.
    UniversalTelegramBot telegramBot;  ///< The Telegram bot object for sending messages.
    
    MessageFormatterCallback messageFormatter; ///< The custom message formatter function (optional).

    /**
     * @brief Constructs a default message based on the label, state, and duration.
     *
     * This is the default implementation for formatting the message. It uses 
     * the label and state to construct a basic message and optionally includes the duration.
     *
     * @param label The label (e.g., "first_floor").
     * @param state The state (e.g., "on", "off").
     * @param duration The duration of the state in seconds (optional).
     * @return A formatted message as a String.
     */
    String constructDefaultMessage(const String& label, const String& state, const String& duration) {
        String message;
        message += "Label: " + label + "\n";  // Add label to message
        message += "State: " + state + "\n";  // Add state to message

        // Optionally add duration if provided
        if (duration.length() > 0) {
            message += "Duration: " + duration + " seconds\n";
        }

        return message;
    }
};

#endif // TELEGRAM_NOTIFIER_H
