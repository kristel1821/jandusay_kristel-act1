#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "KSMJ";
const char* password = "07212005";

WebServer server(80);
const int ledpins[] = {15, 16, 19, 21, 22};
bool allOn = false;

void setupWiFi() {
    WiFi.begin(ssid, password);
    Serial.print("Connecting to WiFi");
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.print(".");
    }
    Serial.println("\nConnected to WiFi");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
}

void setupLEDs() {
    for (int pin :ledpins) {
        pinMode(pin, OUTPUT);
        digitalWrite(pin, LOW);
    }
}

void toggleAllLEDs() {
    allOn = !allOn;
    for (int pin : ledpins) {
        digitalWrite(pin, allOn ? HIGH : LOW);
    }
}

void playSequence(const int sequence[], int length) {
    for (int i = 0; i < length; i++) {
        digitalWrite(sequence[i], HIGH);
        delay(300);
        digitalWrite(sequence[i], LOW);
    }
}

void ledPatternOne() {
    digitalWrite(19, HIGH);
    delay(300);
    digitalWrite(19, LOW);
}

void ledPatternTwo() {
    digitalWrite(16, HIGH);
    digitalWrite(21, HIGH);
    delay(300);
    digitalWrite(16, LOW);
    digitalWrite(21, LOW);
}

void ledPatternThree() {
    digitalWrite(15, HIGH);
    digitalWrite(22, HIGH);
    delay(300);
    digitalWrite(15, LOW);
    digitalWrite(22, LOW);
}

void clearLEDs() {
    for (int pin : ledpins) {
        digitalWrite(pin, LOW);
    }
    delay(100);
}

void handleRoot() {
    String html = "<!DOCTYPE html><html><head><title>ESP32 LED Control</title>";
    html += "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\"></head><body>";
    html += "<div style=\"text-align: center; margin-top: 50px;\">";
    html += "<h1>ESP32 LED Control</h1>";
    html += "<p><a href='/LED=ALL'><button>All On</button></a></p>";
    html += "<p><a href='/LED=32415'><button>Individualy</button></a></p>";
    html += "<p><a href='/LED=12345'><button>32415</button></a></p>";
    html += "</div></body></html>";
    server.send(200, "text/html", html);
}

void handleLED() {
    String request = server.uri();

    if (request.indexOf("/LED=ALL") != -1) {
        Serial.println("Toggle All LEDs");
        toggleAllLEDs();
    } else if (request.indexOf("/LED=32415") != -1) {
        Serial.println("Sequence 32415");
        int sequence[] = {15, 16, 19, 21, 22};
        playSequence(sequence, 5);
    } else if (request.indexOf("/LED=12345") != -1) {
        Serial.println("Sequence 12345");
        ledPatternOne();
        ledPatternTwo();
        ledPatternThree();
        ledPatternTwo();
        ledPatternOne();
        clearLEDs();
    }

    server.sendHeader("Location", "/");
    server.send(303);
}

void setup() {
    Serial.begin(115200);
    setupLEDs();
    setupWiFi();

    server.on("/", handleRoot);
    server.on("/LED=ALL", handleLED);
    server.on("/LED=32415", handleLED);
    server.on("/LED=12345", handleLED);

    server.begin();
}

void loop() {
    server.handleClient();
}
