#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>

// ================= OLED CONFIG =================
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// ================= SENSOR PINS =================
#define GAS_SENSOR A0
#define DHTPIN 2
#define DHTTYPE DHT11

// ================= LED PINS =================
#define GREEN_LED 5
#define YELLOW_LED 6
#define RED_LED 7

// ================= BUZZER CONTROL =================
#define BUZZER_TRIGGER 8

DHT dht(DHTPIN, DHTTYPE);

// ================= VARIABLES =================
int gasLevel = 0;
const char* quality = "";

float temperature = 0;
float humidity = 0;

// ================= AIR QUALITY FUNCTION =================
void readAirQuality() {

  gasLevel = analogRead(GAS_SENSOR);

  digitalWrite(GREEN_LED, LOW);
  digitalWrite(YELLOW_LED, LOW);
  digitalWrite(RED_LED, LOW);
  digitalWrite(BUZZER_TRIGGER, LOW);

  if (gasLevel < 150) {
    quality = "GOOD";
    digitalWrite(GREEN_LED, HIGH);
  }

  else if (gasLevel < 250) {
    quality = "POOR";
    digitalWrite(YELLOW_LED, HIGH);
  }

  else if (gasLevel < 400) {
    quality = "VERY BAD";
    digitalWrite(RED_LED, HIGH);
  }

  else {
    quality = "TOXIC";
    digitalWrite(RED_LED, HIGH);
    digitalWrite(BUZZER_TRIGGER, HIGH);   // trigger 555 buzzer
  }
}

// ================= SETUP =================
void setup() {

  Serial.begin(9600);

  pinMode(GAS_SENSOR, INPUT);

  pinMode(GREEN_LED, OUTPUT);
  pinMode(YELLOW_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  pinMode(BUZZER_TRIGGER, OUTPUT);

  digitalWrite(BUZZER_TRIGGER, LOW);

  dht.begin();

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED failed");
    while (true);
  }

  display.clearDisplay();
  display.setTextColor(WHITE);

  // Startup screen
  display.setTextSize(2);
  display.setCursor(25,10);
  display.println("Air");

  display.setTextSize(1);
  display.setCursor(15,35);
  display.println("Quality Monitor");

  display.display();
  delay(2000);
}

// ================= LOOP =================
void loop() {

  display.clearDisplay();

  // Read Gas Sensor
  readAirQuality();

  // Read DHT Sensor
  humidity = dht.readHumidity();
  temperature = dht.readTemperature();

  // Print Air Quality
  display.setTextSize(1);
  display.setCursor(0,0);
  display.println("Air Quality:");

  display.setTextSize(2);
  display.setCursor(0,15);
  display.print(gasLevel);

  display.setTextSize(1);
  display.setCursor(80,22);
  display.println(quality);

  // Print Temperature
  display.setCursor(0,40);
  display.print("Temp: ");

  if (!isnan(temperature)) {
    display.print(temperature);
    display.println(" C");
  } else {
    display.println("Error");
  }

  // Print Humidity
  display.setCursor(0,52);
  display.print("RH: ");

  if (!isnan(humidity)) {
    display.print(humidity);
    display.println(" %");
  } else {
    display.println("Error");
  }

  display.display();

  delay(2000);
}