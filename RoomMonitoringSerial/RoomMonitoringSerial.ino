#include <DHT.h>
#include <BH1750.h>

#define DHTPIN 2        // DHT11 data pin connected to D2
#define DHTTYPE DHT11   // Specify DHT11 sensor type
DHT dht(DHTPIN, DHTTYPE); // Initialize DHT sensor

BH1750 lightMeter;      // Initialize BH1750 sensor

void setup() {
  Serial.begin(9600);   // Start Serial communication at 9600 baud
  dht.begin();          // Initialize DHT11 sensor
  Wire.begin();         // Initialize I2C for BH1750
  lightMeter.begin();   // Initialize BH1750 sensor

  // Brief delay to allow sensors to stabilize
  delay(2000);
  Serial.println("Sensor Monitoring Started");
  Serial.println("Temperature (°C) | Humidity (%) | Light (lux)");
}

void loop() {
  // Read sensor values
  float temperature = dht.readTemperature(); // Temperature in °C
  float humidity = dht.readHumidity();       // Humidity in %
  float lux = lightMeter.readLightLevel();   // Light intensity in lux

  // Check for valid readings
  
    // Print values to Serial Monitor
    Serial.print(temperature);
    Serial.print(" °C\t\t| ");
    Serial.print(humidity);
    Serial.print(" %\t\t| ");
    Serial.print(lux);
    Serial.println(" lux");
  

  delay(2000); // Update every 2 seconds for readability
}