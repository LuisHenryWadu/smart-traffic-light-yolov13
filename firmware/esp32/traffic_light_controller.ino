#include <WiFi.h>
#include "config.h"

/*
 * Smart Traffic Light - ESP32 controller
 *
 * Portfolio reconstruction based on the original 2025 project report.
 * The report documents ESP32-based actuator control over Wi-Fi, but it does
 * not preserve the exact original GPIO map or network packet format.
 *
 * Reconstructed command format:
 *   PHASE,<DIRECTION>,<GREEN_DURATION_MS>\n
 */

constexpr uint16_t SERVER_PORT = 8080;
constexpr unsigned long ALL_RED_TRANSITION_MS = 1000;
constexpr unsigned long YELLOW_DURATION_MS = 3000;
constexpr unsigned long MIN_GREEN_MS = 1000;
constexpr unsigned long MAX_GREEN_MS = 120000;

struct LightPins {
  uint8_t red;
  uint8_t yellow;
  uint8_t green;
};

// Reference GPIO reconstruction only. Update to match actual wiring.
const LightPins NORTH = {13, 14, 16};
const LightPins EAST  = {17, 18, 19};
const LightPins SOUTH = {21, 22, 23};
const LightPins WEST  = {25, 26, 27};

WiFiServer server(SERVER_PORT);

enum class Direction { NORTH, EAST, SOUTH, WEST, INVALID };

void configurePins(const LightPins &pins) {
  pinMode(pins.red, OUTPUT);
  pinMode(pins.yellow, OUTPUT);
  pinMode(pins.green, OUTPUT);
}

void writeLight(const LightPins &pins, bool red, bool yellow, bool green) {
  digitalWrite(pins.red, red ? HIGH : LOW);
  digitalWrite(pins.yellow, yellow ? HIGH : LOW);
  digitalWrite(pins.green, green ? HIGH : LOW);
}

void setAllRed() {
  writeLight(NORTH, true, false, false);
  writeLight(EAST, true, false, false);
  writeLight(SOUTH, true, false, false);
  writeLight(WEST, true, false, false);
}

const LightPins* pinsFor(Direction direction) {
  switch (direction) {
    case Direction::NORTH: return &NORTH;
    case Direction::EAST: return &EAST;
    case Direction::SOUTH: return &SOUTH;
    case Direction::WEST: return &WEST;
    default: return nullptr;
  }
}

Direction parseDirection(String value) {
  value.trim();
  value.toUpperCase();
  if (value == "NORTH") return Direction::NORTH;
  if (value == "EAST") return Direction::EAST;
  if (value == "SOUTH") return Direction::SOUTH;
  if (value == "WEST") return Direction::WEST;
  return Direction::INVALID;
}

String directionName(Direction direction) {
  switch (direction) {
    case Direction::NORTH: return "NORTH";
    case Direction::EAST: return "EAST";
    case Direction::SOUTH: return "SOUTH";
    case Direction::WEST: return "WEST";
    default: return "INVALID";
  }
}

bool parsePhaseCommand(const String &line, Direction &direction, unsigned long &greenMs) {
  int firstComma = line.indexOf(',');
  int secondComma = line.indexOf(',', firstComma + 1);
  if (firstComma < 0 || secondComma < 0) return false;

  String command = line.substring(0, firstComma);
  String directionText = line.substring(firstComma + 1, secondComma);
  String durationText = line.substring(secondComma + 1);

  command.trim();
  command.toUpperCase();
  durationText.trim();

  if (command != "PHASE") return false;

  direction = parseDirection(directionText);
  if (direction == Direction::INVALID) return false;

  long parsedDuration = durationText.toInt();
  if (parsedDuration < static_cast<long>(MIN_GREEN_MS) ||
      parsedDuration > static_cast<long>(MAX_GREEN_MS)) {
    return false;
  }

  greenMs = static_cast<unsigned long>(parsedDuration);
  return true;
}

void runGreenPhase(Direction direction, unsigned long greenMs) {
  const LightPins *active = pinsFor(direction);
  if (active == nullptr) return;

  setAllRed();
  delay(ALL_RED_TRANSITION_MS);

  writeLight(*active, false, false, true);
  delay(greenMs);

  writeLight(*active, false, true, false);
  delay(YELLOW_DURATION_MS);

  setAllRed();
}

void connectWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print('.');
  }

  Serial.println();
  Serial.print("Connected. ESP32 IP: ");
  Serial.println(WiFi.localIP());
}

void setup() {
  Serial.begin(115200);

  configurePins(NORTH);
  configurePins(EAST);
  configurePins(SOUTH);
  configurePins(WEST);
  setAllRed();

  connectWiFi();
  server.begin();

  Serial.print("Traffic-light command server listening on TCP port ");
  Serial.println(SERVER_PORT);
}

void loop() {
  WiFiClient client = server.available();
  if (!client) {
    delay(10);
    return;
  }

  client.setTimeout(3000);
  String line = client.readStringUntil('\n');
  line.trim();

  Direction direction = Direction::INVALID;
  unsigned long greenMs = 0;

  if (!parsePhaseCommand(line, direction, greenMs)) {
    client.println("ERR,INVALID_COMMAND");
    client.stop();
    return;
  }

  client.print("ACK,");
  client.print(directionName(direction));
  client.print(',');
  client.println(greenMs);
  client.flush();

  Serial.print("Running phase: ");
  Serial.print(directionName(direction));
  Serial.print(" for ");
  Serial.print(greenMs);
  Serial.println(" ms");

  runGreenPhase(direction, greenMs);
  client.stop();
}
