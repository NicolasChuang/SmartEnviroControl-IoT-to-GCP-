/*
ESP32 publish telemetry data to Google Cloud (DHT11 sensor)
*/
#include <WiFi.h>
#include <PubSubClient.h>
#include <ESP32Servo.h>
#include "DHT.h"

const char *WIFI_SSID = "wifi"; //your WiFi SSID
const char *WIFI_PASSWORD = "password"; // your password
const char *MQTT_SERVER = "34.42.110.62"; // your VM instance public IP address
const int MQTT_PORT = 1883;
const char *MQTT_TOPIC = "iot"; // MQTT topic

int servoPin = 40;
int relayPin = 39;
int dhtPin = A4;
Servo myServo;
DHT dht(dhtPin, DHT11);

// Define quartile and median thresholds for temperature and humidity
float tempQuartile1 = 26.44;
float tempMedian = 28.83;
float tempQuartile3 = 31.17;
float humQuartile1 = 63;
float humMedian = 77.9;
float humQuartile3 = 89.5;

WiFiClient espClient;
PubSubClient client(espClient);
void setup_wifi()
{
  delay(10);
  Serial.println("Connecting to WiFi...");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}
void setup()
{
  Serial.begin(115200);
  dht.begin();
  setup_wifi();
  myServo.setPeriodHertz(50);
  myServo.attach(servoPin, 500, 2500);
  pinMode(relayPin, OUTPUT);
  Serial.begin(9600);
  client.setServer(MQTT_SERVER, MQTT_PORT);
}
void reconnect()
{
 while (!client.connected())
 {
  Serial.println("Attempting MQTT connection...");
  if (client.connect("ESP32Client"))
  {
  Serial.println("Connected to MQTT server");
  }
  else
  {
  Serial.print("Failed, rc=");
  Serial.print(client.state());
  Serial.println(" Retrying in 5 seconds...");
  delay(5000);
  }
 }
}

// Euclidean distance function
float calculateDistance(float value, float threshold) {
  return abs(value - threshold);
}

// Nearest neighbor algorithm using Euclidean distance
String getTemperatureCategory(float temperature) {
  float distLow = calculateDistance(temperature, tempQuartile1);
  float distMedian = calculateDistance(temperature, tempMedian);
  float distHigh = calculateDistance(temperature, tempQuartile3);

  if (distLow <= distMedian && distLow <= distHigh)
    return "Low";
  else if (distMedian <= distLow && distMedian <= distHigh)
    return "Median";
  else
    return "High";
}

String getHumidityCategory(float humidity) {
  float distLow = calculateDistance(humidity, humQuartile1);
  float distMedian = calculateDistance(humidity, humMedian);
  float distHigh = calculateDistance(humidity, humQuartile3);

  if (distLow <= distMedian && distLow <= distHigh)
    return "Low";
  else if (distMedian <= distLow && distMedian <= distHigh)
    return "Median";
  else
    return "High";
}

void loop()
{

  Serial.println("Reading temperature and humidity from DHT11");
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();
  int pump;
  float spin_duration;

  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println(" °C");

  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.println(" %");

  // Determine temperature and humidity categories using nearest neighbor algorithm
  String tempCategory = getTemperatureCategory(temperature);
  String humCategory = getHumidityCategory(humidity);

  Serial.println("Temperature Category: " + tempCategory);
  Serial.println("Humidity Category: " + humCategory);

  // Humidity Control
  if (humCategory == "Low") {
    Serial.println("Humidity in 1st quartile. Moving servo back and forth twice");
    myServo.write(90);
    delay(2000);  // wait for 2 seconds
    myServo.write(0);
    delay(2000);  // wait for 2 seconds
    myServo.write(90);
    delay(2000);  // wait for 2 seconds
    myServo.write(0);
    delay(2000);  // wait for 2 seconds
    pump=2;
  } else if (humCategory == "Median"){
    Serial.println("Humidity in 1st quartile. Moving servo back and forth once");
    myServo.write(90);
    delay(2000);  // wait for 2 seconds
    myServo.write(0);
    delay(2000);  // wait for 2 seconds
    pump=1;
  } else if (humCategory == "High") {
    Serial.println("Humidity in 3rd quartile. No action taken.");
    pump = 0;
  } else {
    Serial.println("Humidity above 3rd quartile. No action taken.");
    pump = 0;
  }

  // Temperature Control
  if (tempCategory == "Low") {
    Serial.println("Temperature in 1st quartile. Turning on the fan for 1 second");
    digitalWrite(relayPin, HIGH);
    delay(1000);  // wait for 1 second
    digitalWrite(relayPin, LOW);
    spin_duration = 1.0;
  } else if (tempCategory == "Median") {
    Serial.println("Temperature in median. Turning on the fan for 3 seconds");
    digitalWrite(relayPin, HIGH);
    delay(3000);  // wait for 3 seconds
    digitalWrite(relayPin, LOW);
    spin_duration = 3.0;
  } else if (tempCategory == "High") {
    Serial.println("Temperature in 3rd quartile. Turning on the fan for 6 seconds");
    digitalWrite(relayPin, HIGH);
    delay(6000);  // wait for 6 seconds
    digitalWrite(relayPin, LOW);
    spin_duration = 6.0;
  } else {
    Serial.println("Temperature above 3rd quartile. Turning on the fan for 6 seconds");
    digitalWrite(relayPin, HIGH);
    delay(6000);  // wait for 6 seconds
    digitalWrite(relayPin, LOW);
    spin_duration = 6.0;
  }


 if (!client.connected())
  {
  reconnect();
  }
  client.loop();
  delay(5000); // adjust the delay according to your requirements
  char payload[50];
  sprintf(payload, "%.2f|%.2f|%d|%.2f", temperature, humidity, pump, spin_duration);
  client.publish(MQTT_TOPIC, payload);
}


