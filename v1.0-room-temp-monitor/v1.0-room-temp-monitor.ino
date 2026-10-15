
#include <LiquidCrystal.h>
#include <DHT.h>

#define DHTPIN 2
#define DHTTYPE DHT11
#define RED_PIN 6
#define GREEN_PIN 5
#define BLUE_PIN 3
#define FAN_PIN 4

// LCD pins: RS, E, D4, D5, D6, D7
LiquidCrystal lcd(7, 8, 9, 10, 11, 12);
DHT dht(DHTPIN, DHTTYPE);
bool fanOn = false;

void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);
  dht.begin();
  lcd.print("Game Room");
  lcd.setCursor(0, 1);
  lcd.print("Booting...");
  delay(2000);
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);
  pinMode(FAN_PIN, OUTPUT);
}

void setColor(int red, int green, int blue) {
  analogWrite(RED_PIN, red);
  analogWrite(GREEN_PIN, green);
  analogWrite(BLUE_PIN, blue);
}

void loop() {
  float humidity = dht.readHumidity();
  float tempF = dht.readTemperature(true);  // true = Fahrenheit

  lcd.clear();

  if (isnan(humidity) || isnan(tempF)) {
    lcd.print("Sensor Error");
    Serial.println("Sensor Error");
    lcd.setCursor(0, 1);
    lcd.print("Check wiring");
    setColor(0, 255, 255);
    digitalWrite(FAN_PIN, LOW); fanOn = false;
  } else {
    lcd.setCursor(0, 0);
    lcd.print(tempF, 1);
    lcd.print((char)223);
    lcd.print("F");

    lcd.setCursor(8, 0);
    lcd.print("Humd ");
    lcd.print(humidity, 0);
    lcd.print("%");
    lcd.setCursor(0, 1);

    Serial.print("Temp: ");
    Serial.print(tempF, 1);
    Serial.print("F  Humidity: ");
    Serial.print(humidity, 0);
    Serial.println(" %");


    if (tempF > 75) {
      fanOn = true;
      lcd.print("Getting Warm!");
      Serial.println("Getting Warm!");
      setColor(255, 0, 0);
    } else if (tempF < 74) {
      fanOn = false;
      lcd.print("Getting Cold!");
      Serial.println("Getting Cold!");
      setColor(0, 0, 255);
    } else {
      lcd.print("Comfy ol vibe!");
      Serial.println("Comfy ol vibe!");
      setColor(0, 255, 0);
    }
  }
  if (fanOn) {
    digitalWrite(FAN_PIN, HIGH);
  } else {
    digitalWrite(FAN_PIN, LOW);
  }

  delay(2000); 
}
