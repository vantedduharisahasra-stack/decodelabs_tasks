#include <Arduino.h>

#define SOIL_MOISTURE_PIN 34
#define RELAY_PIN         26 

const int DRY_THRESHOLD = 2800;
const int WET_THRESHOLD = 1800;

unsigned long previousMillis = 0;
const long sampleInterval = 1000;

bool isPumpActive = false;

void setup() {
  Serial.begin(115200);
  pinMode(RELAY_PIN, OUTPUT);
  
  digitalWrite(RELAY_PIN, HIGH);
  
  Serial.println(F("--- Automated Irrigation System Initialized ---"));
}

void loop() {
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= sampleInterval) {
    previousMillis = currentMillis;

    int rawMoisture = analogRead(SOIL_MOISTURE_PIN);
    
    float moisturePercentage = map(rawMoisture, 4095, 0, 0, 100);
    moisturePercentage = constrain(moisturePercentage, 0.0, 100.0);

    if (rawMoisture > DRY_THRESHOLD && !isPumpActive) {
      isPumpActive = true;
      digitalWrite(RELAY_PIN, LOW);
      Serial.println(F("[ACTION] Soil is DRY! Water Pump ACTIVATED."));
    } 
    else if (rawMoisture < WET_THRESHOLD && isPumpActive) {
      isPumpActive = false;
      digitalWrite(RELAY_PIN, HIGH);
      Serial.println(F("[ACTION] Soil is WET! Water Pump DEACTIVATED."));
    }

    Serial.print(F("ADC Value: "));
    Serial.print(rawMoisture);
    Serial.print(F(" | Moisture: "));
    Serial.print(moisturePercentage, 1);
    Serial.print(F("% | Relay/Pump State: "));
    Serial.println(isPumpActive ? F("ON [WATERING]") : F("OFF [IDLE]"));
  }
}
