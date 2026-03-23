#include <DHT.h>
#include <arduino-timer.h>

#define SENSOR_PIN 14
#define SENSOR_TYPE DHT11

const uint8_t leds[5] = {6, 7, 10, 11, 15};
const int totalLeds = sizeof(leds) / sizeof(leds[0]);

unsigned long lastStateUpdate = 0;
unsigned long lastLedActivation = 0;
unsigned long blinkStartTime = 0;
unsigned long lastBlinkTime = 0;

bool areLedsActive = false;
bool blinkLed = false;
bool builtInLedState = false; // Current state of the built-in LED

String machineState;

DHT temperatureSensor(SENSOR_PIN, SENSOR_TYPE);

// Function prototypes
void handleLeds(String binaryCommand);
void deactivateLeds();
void sendSensorData();
String receiveCommand();

void setup() {
  for (int i = 0; i < totalLeds; i++) {
    pinMode(leds[i], OUTPUT);
    digitalWrite(leds[i], LOW);
  }

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

  Serial.begin(9600);
  temperatureSensor.begin();
}

void loop() {
  String command = receiveCommand();

  if (command.startsWith("OP,")) {
    handleLeds(command.substring(3));
    machineState = "OP";
    lastStateUpdate = millis();
    blinkLed = true;
    blinkStartTime = millis();
    sendSensorData();
  } else if (command.startsWith("ON,")) {
    handleLeds(command.substring(3));
    machineState = "ON";
    lastStateUpdate = millis();
    blinkLed = false;
    digitalWrite(LED_BUILTIN, HIGH);
    delay(2000);
    digitalWrite(LED_BUILTIN, LOW);
    sendSensorData();
  } else if (command.startsWith("OFF,")) {
    handleLeds(command.substring(3));
    machineState = "OFF";
    lastStateUpdate = millis();
    blinkLed = false;
    digitalWrite(LED_BUILTIN, LOW);
    sendSensorData();
  }

  if (areLedsActive && millis() - lastLedActivation >= 2000) {
    deactivateLeds();
    areLedsActive = false;
  }

  if (blinkLed) {
    if (millis() - blinkStartTime >= 2000) {
      blinkLed = false;
      digitalWrite(LED_BUILTIN, LOW);
    } else if (millis() - lastBlinkTime >= 250) {
      lastBlinkTime = millis();
      builtInLedState = !builtInLedState;
      digitalWrite(LED_BUILTIN, builtInLedState ? HIGH : LOW);
    }
  }
}

void handleLeds(String binaryCommand) {
  int ledIndex = 0;
  int commandLength = binaryCommand.length();

  for (int i = commandLength - 1; i >= 0; i--) {
    char bit = binaryCommand[i];
    if (bit == '1' || bit == '0') {
      if (ledIndex < totalLeds) {
        digitalWrite(leds[ledIndex], (bit == '1') ? HIGH : LOW);
        ledIndex++;
      }
    }
  }

  for (; ledIndex < totalLeds; ledIndex++) {
    digitalWrite(leds[ledIndex], LOW);
  }

  areLedsActive = true;
  lastLedActivation = millis();
}

void deactivateLeds() {
  for (int i = 0; i < totalLeds; i++) {
    digitalWrite(leds[i], LOW);
  }
  digitalWrite(LED_BUILTIN, LOW);
}

void sendSensorData() {
  float temp = temperatureSensor.readTemperature();
  float humidity = temperatureSensor.readHumidity();

  String sensorData;

  if (isnan(temp) || isnan(humidity)) {
    sensorData = "ERROR,TEMP,HUM";
  } else {
    int roundedTemp = (int)round(temp);
    int roundedHumidity = (int)round(humidity);

    sensorData = "TEMP&unit:celsius&value:";
    sensorData += roundedTemp;
    sensorData += "#HUM&unit:percentage&value:";
    sensorData += roundedHumidity;
  }

  Serial.println(sensorData);
}

String receiveCommand() {
  if (Serial.available()) {
    String incomingCommand = Serial.readStringUntil('\n');
    incomingCommand.trim();
    return incomingCommand;
  }
  return "";
}

