/*

Copyright 2025 Marc Ketel
SPDX-License-Identifier: Apache-2.0

*/

#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <Ticker.h>

#include <AsyncMqttClient.h>
#include <ESPAsyncWebServer.h>
#include <ESPAsyncWiFiManager.h>
#include <MaxLedControl.h>

#define ESP8266_DRD_USE_RTC       true
#define DOUBLERESETDETECTOR_DEBUG true
#define DRD_TIMEOUT               2
#define DRD_ADDRESS               0
#include <ESP_DoubleResetDetector.h>
DoubleResetDetector *drd;

LedControl lc = LedControl(13, 14, 15);

AsyncWebServer server(80);
DNSServer dns;

String name;

const char *mqtt_server = "revspace.nl";
AsyncMqttClient mqttClient;
Ticker mqttReconnectTimer;

WiFiEventHandler wifiConnectHandler;
WiFiEventHandler wifiDisconnectHandler;

void displayN(int16_t n) {
    if (n < 0) {
        lc.setRow(0, 0, 0);
        lc.setRow(0, 1, 0b00000001);
        lc.setRow(0, 2, 0b00000001);
        lc.setRow(0, 3, 0);
        return;
    }

    if (n > 9999) {
        lc.setRow(0, 0, 0);
        lc.setRow(0, 1, 0b00110111); // H
        lc.setRow(0, 2, 0b00110111); // H
        lc.setRow(0, 3, 0);
        return;
    }

    if (n > 999) {
        lc.setChar(0, 0, n / 1000, false);
    } else {
        lc.setRow(0, 0, 0b00010101); // n
    }

    if (n > 99) {
        lc.setChar(0, 1, (n % 1000) / 100, false);
    } else {
        lc.setRow(0, 1, 0b00001001); // =
    }

    if (n > 9) {
        lc.setChar(0, 2, (n % 100) / 10, false);
    } else {
        lc.setRow(0, 2, 0);
    }

    lc.setChar(0, 3, n % 10, false);
}

void connectToMqtt() {
    Serial.println("Connecting to MQTT server");
    mqttClient.connect();
}

void onWifiConnect(const WiFiEventStationModeGotIP &event) {
    lc.setRow(0, 0, 0b00011100); // v
    lc.setRow(0, 1, 0b00010000); // i
    lc.setRow(0, 2, 0b01000111); // F
    lc.setRow(0, 3, 0b00010000); // i

    Serial.println("WiFi connected.");

    connectToMqtt();
}

void onWifiDisconnect(const WiFiEventStationModeDisconnected &event) {
    Serial.println("WiFi has disconnected.");
    mqttReconnectTimer.detach();
}

void onMqttSubscribe(uint16_t packetId, uint8_t qos) {
    Serial.println("Subscription to topic acknowledged.");
}

void onMqttConnect(bool sessionPresent) {

    lc.setRow(0, 0, 0b01110110); // N
    lc.setRow(0, 1, 0b01110011); // Q
    lc.setRow(0, 2, 0b00001111); // t
    lc.setRow(0, 3, 0b00001111); // t

    Serial.println("Connected to MQTT server.");

    Serial.println("Subscribing topics.");

    mqttClient.subscribe("revspace/doorduino/checked-in", 0);
    mqttClient.subscribe("revspace/state", 0);
}

void onMqttDisconnect(AsyncMqttClientDisconnectReason reason) {
    Serial.println("I seem to be disconnected from the MQTT server.");

    if (WiFi.isConnected()) {
        Serial.println("Lets reconnect to MQTT server.");
        mqttReconnectTimer.once(2, connectToMqtt);
    }
}

int16_t checkedin = 0;
uint8_t state = 0;

void onMqttMessage(char *topic, char *payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index, size_t total) {
    String topicString = topic;

    Serial.print("A MQTT message has arrived on topic: ");
    Serial.println(topicString);

    // Do nothing if unexpected length is being published.
    if (len > 32) {
        Serial.println("Ignoring payload.");
        return;
    }

    Serial.print("MQTT payload: ");

    char payloadChars[len + 1];
    for (uint8_t i = 0; i < len; i++) {
        payloadChars[i] = payload[i];
    }
    payloadChars[len] = '\0';

    String payloadString = payloadChars;

    Serial.println(payloadString);

    if (topicString == "revspace/doorduino/checked-in") {
        checkedin = payloadString.toInt();
    }

    if (topicString == "revspace/state") {
        state = (payloadString == "open");
    }

    if (state == 0) {
        displayN(-1); // Closed
    } else {
        displayN(checkedin);
    }
}

void setup() {
    drd = new DoubleResetDetector(DRD_TIMEOUT, DRD_ADDRESS);

    Serial.begin(115200);

    Serial.println("setup() starting.");

    String mac = WiFi.macAddress();
    mac.replace(":", "");
    mac.toLowerCase();
    name = "nteller-" + mac;

    Serial.print("Hello there! My hostname is ");
    Serial.println(name);

    Serial.println("Configuring MAX7219.");
    lc.shutdown(0, false);
    lc.setIntensity(0, 2);
    for (uint8_t i = 0; i < 4; i++) {
        lc.setRow(0, i, 0xFF);
    }

    Serial.println("Starting wifi.");
    WiFi.hostname(name.c_str());
    WiFi.setSleepMode(WIFI_NONE_SLEEP);

    if (drd->detectDoubleReset()) {
        Serial.println("Double reset detected. Starting wifimanager.");

        lc.setRow(0, 0, 0);
        lc.setRow(0, 1, 0b01110111);
        lc.setRow(0, 2, 0b01100111);
        lc.setRow(0, 3, 0);

        AsyncWiFiManager wifiManager(&server, &dns);
        wifiManager.setTimeout(60);
        if (!wifiManager.startConfigPortal(name.c_str())) {
            Serial.println("Failed to connect and hit timeout.");
            ESP.restart();
        }

        Serial.println("Our new WiFi credentials seem to work!");
    } else {
        WiFi.begin();
    }

    wifiConnectHandler = WiFi.onStationModeGotIP(onWifiConnect);
    wifiDisconnectHandler = WiFi.onStationModeDisconnected(onWifiDisconnect);

    mqttClient.onConnect(onMqttConnect);
    mqttClient.onDisconnect(onMqttDisconnect);
    mqttClient.onSubscribe(onMqttSubscribe);
    mqttClient.onMessage(onMqttMessage);
    mqttClient.setClientId(name.c_str());
    mqttClient.setServer(mqtt_server, 1883);

    if (!MDNS.begin(name.c_str())) {
        Serial.println("Error starting mDNS.");
    } else {
        Serial.println("mDNS started.");
    }

    Serial.println("Configuring async webserver.");
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(200, "text/plain", name);
    });

    Serial.println("setup() done.");
}

void loop() {
    MDNS.update();
    drd->loop();
}