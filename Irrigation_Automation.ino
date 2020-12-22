#include <DHT.h>
#include <ESP8266WiFi.h>

String apiKey = "0C9EHRLXFHBSNRYS";     //  Enter your Write API key from ThingSpeak
const char* ssid =  "PREETAM";     // Enter your WiFi Network's SSID
const char* pass =  "12345678"; // Enter your WiFi Network's Password
const char* server = "api.thingspeak.com";
#define DHTPIN D3          // GPIO Pin where the dht11 is connected
DHT dht(DHTPIN, DHT11);
WiFiClient client;

const int relaypin = 2;               //Relaypin
const int moisturePin = A0;             // moisteure sensor pin
unsigned long interval = 1000;
unsigned long previousMillis = 0;
unsigned long interval1 = 500;
unsigned long previousMillis1 = 0;
float moisturePercentage;              //moisture reading
float h;                  // humidity reading
float t;                  //temperature reading

void setup()
{
  Serial.begin(115200);
  delay(10);
  pinMode(relaypin, OUTPUT);
  digitalWrite(relaypin, LOW); // keep motor off initally
  dht.begin();
  Serial.println("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");              // print ... till not connected
  }
  Serial.println("");
  Serial.println("WiFi connected");
}

void loop()
{
  unsigned long currentMillis = millis(); // grab current time

  h = dht.readHumidity();     // read humiduty
  t = dht.readTemperature();     // read temperature



  moisturePercentage = ( 100.00 - ( (analogRead(moisturePin) / 1023.00) * 100.00 ) ); // convering ADU to percentage

 if ((unsigned long)(currentMillis - previousMillis1) >= interval1) {
    Serial.print("Soil Moisture is  = ");
    Serial.print(moisturePercentage);
    Serial.println("%");
    previousMillis1 = millis();
 }

 if(moisturePercentage >=0 && moisturePercentage <= 45)
  {
    digitalWrite(relaypin, HIGH);
    Serial.println("Motor is ON");
    delay(1000);
  }
  else if (moisturePercentage >45 && moisturePercentage <= 100)
  {
    digitalWrite(relaypin, LOW);
    Serial.println("Motor is OFF");
    delay(1000);
  }

  /* if(h >=0 && h <= 30)
  {
    digitalWrite(relaypin, LOW);
    Serial.println("Sprinkler is OFF");
    delay(1000);
  }
  else if (h>30)
  {
    digitalWrite(relaypin, HIGH);
    Serial.println("Sprinkler is ON");
    delay(1000);
  } */




if ((unsigned long)(currentMillis - previousMillis) >= interval) {

  sendThingspeak();           //send data to thing speak
  previousMillis = millis();
  client.stop();
}


}

void sendThingspeak() {
  if (client.connect(server, 80))
  {
    String postStr = apiKey;              // add api key in the postStr string
    postStr += "&field1=";
    postStr += String(moisturePercentage);  // add mositure readin 
   
    postStr += "&field2=";
    postStr += String(t);  // add tempr readin
    
    postStr += "&field3=";
    postStr += String(h);  // add humidity readin

    postStr += "&field4=";
    postStr += String(relaypin);
     
    postStr += "\r\n\r\n";

    client.print("POST /update HTTP/1.1\n");
    client.print("Host: api.thingspeak.com\n");
    client.print("Connection: close\n");
    client.print("X-THINGSPEAKAPIKEY: " + apiKey + "\n");
    client.print("Content-Type: application/x-www-form-urlencoded\n");
    client.print("Content-Length: ");
    client.print(postStr.length());           //send lenght of the string
    client.print("\n\n");
    client.print(postStr);                      // send complete string
    Serial.print("Moisture Percentage: ");
    Serial.print(moisturePercentage);
    Serial.print("%. Temperature: ");
    Serial.print(t);
    Serial.print(" C, Humidity: ");
    Serial.print(h);
    Serial.println("%. Sent to Thingspeak.");
  }
}
