#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>

#define DHTPIN 13
#define DHTTYPE DHT22

DHT dht(DHTPIN, DHTTYPE);

LiquidCrystal_I2C lcd(0x27, 16, 2);

// RELAY
#define RELAY_CH1 26
#define RELAY_CH2 27

// KOMPONEN
#define BUTTON_PIN 23
#define LED_MERAH 18
#define LED_HIJAU 19
#define BUZZER 25

bool sistemON = false;

void setup() {

  Serial.begin(115200);

  pinMode(RELAY_CH1, OUTPUT);
  pinMode(RELAY_CH2, OUTPUT);

  pinMode(LED_MERAH, OUTPUT);
  pinMode(LED_HIJAU, OUTPUT);

  pinMode(BUZZER, OUTPUT);

  pinMode(BUTTON_PIN, INPUT_PULLUP);

  // Relay OFF
  digitalWrite(RELAY_CH1, HIGH);
  digitalWrite(RELAY_CH2, HIGH);

  Wire.begin(21, 22);

  lcd.init();
  lcd.backlight();

  lcd.setCursor(0,0);
  lcd.print("SMART WARMER");

  lcd.setCursor(0,1);
  lcd.print("STATUS: OFF");

  dht.begin();
}

void loop() {

  // Tombol ditekan
  if(digitalRead(BUTTON_PIN) == LOW) {

    sistemON = !sistemON;

    // Bunyi notif
    tone(BUZZER, 1500);
    delay(150);
    noTone(BUZZER);

    delay(400);
  }

  float suhu = dht.readTemperature();
  float hum = dht.readHumidity();

  // Error sensor
  if(isnan(suhu) || isnan(hum)) {

    lcd.clear();

    lcd.setCursor(0,0);
    lcd.print("DHT ERROR");

    return;
  }

  // SISTEM ON
  if(sistemON) {

    lcd.setCursor(0,0);
    lcd.print("STATUS: ON ");

    lcd.setCursor(0,1);
    lcd.print("T:");
    lcd.print(suhu);
    lcd.print((char)223);
    lcd.print("C ");

    // LED hijau ON
    digitalWrite(LED_HIJAU, HIGH);

    // Kipas ON
    digitalWrite(RELAY_CH2, LOW);

    // Lampu kontrol suhu
    if(suhu < 50) {

      digitalWrite(RELAY_CH1, LOW);

      digitalWrite(LED_MERAH, HIGH);

    }

    else if(suhu >= 70) {

      digitalWrite(RELAY_CH1, HIGH);

      digitalWrite(LED_MERAH, LOW);
    }

    // Overheat
    if(suhu > 75) {

      tone(BUZZER, 1000);

    } else {

      noTone(BUZZER);
    }

  }

  // SISTEM OFF
  else {

    digitalWrite(RELAY_CH1, HIGH);
    digitalWrite(RELAY_CH2, HIGH);

    digitalWrite(LED_MERAH, LOW);
    digitalWrite(LED_HIJAU, LOW);

    noTone(BUZZER);

    lcd.setCursor(0,0);
    lcd.print("STATUS: OFF");

    lcd.setCursor(0,1);
    lcd.print("SMART WARMER ");
  }

  delay(1000);
}