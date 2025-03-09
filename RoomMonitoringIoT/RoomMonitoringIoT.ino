#include <WiFiNINA.h>
#include <DHT.h>
#include <BH1750.h>
#include "secrets.h"
#include "ThingSpeak.h"

#define DHTPIN 2          // DHT11 data pin connected to D2
#define DHTTYPE DHT11     // Specify DHT11 sensor type
DHT dht(DHTPIN, DHTTYPE); // Initialize DHT sensor

BH1750 lightMeter;        // Initialize BH1750 sensor

char ssid[] = SECRET_SSID;   // your network SSID (name) 
char pass[] = SECRET_PASS;   // your network password
int keyIndex = 0;            // your network key Index number (needed only for WEP)
WiFiClient  client;

unsigned long myChannelNumber = SECRET_CH_ID;
const char * myWriteAPIKey = SECRET_WRITE_APIKEY;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT); // Set builtin LED as Alarm pin (ON when fire detected)
  Serial.begin(9600);   // Initialize serial 
  dht.begin();            // Initialize DHT11 sensor
  Wire.begin();           // Initialize I2C for BH1750
  lightMeter.begin();     // Initialize BH1750 sensor

  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo native USB port only
  }
  
  ThingSpeak.begin(client);  // Initialize ThingSpeak 

  // Brief delay to allow sensors to stabilize
  delay(2000);
  Serial.println("Sensor Monitoring Started");
}

void loop() {

  float temperature = dht.readTemperature(); // Temperature in °C
  float humidity = dht.readHumidity();       // Humidity in %
  float lux = lightMeter.readLightLevel();   // Light intensity in lux
  int fireAlarm = (temperature > 28 && humidity < 50 && lux > 50) ? 1 : 0;

  digitalWrite(LED_BUILTIN, fireAlarm);  // turn the LED on fire alarm

  // Connect or reconnect to WiFi
  if(WiFi.status() != WL_CONNECTED){
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(SECRET_SSID);
    while(WiFi.status() != WL_CONNECTED){
      WiFi.begin(ssid, pass);
      Serial.print(".");
      delay(5000);     
    } 
    Serial.println("\nConnected.");
  }

  if (isnan(temperature) || isnan(humidity) || lux < 0) {
    ThingSpeak.setStatus("Sensor reading error!");
  } else {
    ThingSpeak.setStatus("Sensor reading OK!");
  }

  // set the fields with the values
  ThingSpeak.setField(1, temperature);
  ThingSpeak.setField(2, humidity);
  ThingSpeak.setField(3, lux);
  ThingSpeak.setField(4, fireAlarm);
  


  
  // write to the ThingSpeak channel 
  int response = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
  if(response == 200){
    Serial.println("Channel update successful.");
  }
  else{
    Serial.println("Problem updating channel. HTTP error code " + String(response));
  }

  
  delay(60000); // Wait 60 seconds to update the channel again
}
