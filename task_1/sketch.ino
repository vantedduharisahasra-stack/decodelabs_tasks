#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>

#define DHTPIN 4
#define DHTTYPE DHT22 // Wokwi uses DHT22 by default

LiquidCrystal_I2C lcd(0x27, 16, 2);
DHT dht(DHTPIN, DHTTYPE);

unsigned long previousMillis = 0;
const long sampleInterval = 2000;

void setup() {
  Serial.begin(115200);
  Serial.println(F("--- Wokwi Environmental Monitor Starting ---"));

  dht.begin();
  
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Wokwi Sim Active");
  delay(1000);
  lcd.clear();
}

void loop() {
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= sampleInterval) {
    previousMillis = currentMillis;

    float humidity = dht.readHumidity();
    float tempC = dht.readTemperature();

    if (isnan(humidity) || isnan(tempC)) {
      Serial.println(F("[ERROR] Failed to read from DHT sensor!"));
      return;
    }

    // Output to Serial Monitor
    Serial.print(F("Time: "));
    Serial.print(currentMillis / 1000);
    Serial.print(F("s | Temp: "));
    Serial.print(tempC, 1);
    Serial.print(F(" °C | Humidity: "));
    Serial.print(humidity, 1);
    Serial.println(F(" %"));

    // Output to Virtual LCD Screen
    lcd.setCursor(0, 0);
    lcd.print("Temp: ");
    lcd.print(tempC, 1);
    lcd.print((char)223);
    lcd.print("C   ");

    lcd.setCursor(0, 1);
    lcd.print("Humidity: ");
    lcd.print(humidity, 1);
    lcd.print("%   ");
  }
}
