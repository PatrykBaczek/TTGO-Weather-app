#include <TFT_eSPI.h>
#include <SPI.h>
#include <ArduinoJson.h>
#include <NTPClient.h>

// internet setup

#include <WiFi.h>
#include <WiFiUdp.h>
#include <HTTPClient.h>

TFT_eSPI tft = TFT_eSPI();

#define BLACK 0x0000
#define WHITE 0xFFFF
#define GRAY  0x5AEB
#define YELLOW  0xFFFF00

const char* ssid = "realme7";
const char* password = "12345678";

String town = "Cieszyn";

// weather for Cieszyn, Poland

const String link_to_data = "https://api.openweathermap.org/data/2.5/weather?q=Cieszyn&appid=4d52f61bda0d42e0874f6b5865e23a6d&lang=pl&units=metric";

//const String api_key = "4d52f61bda0d42e0874f6b5865e23a6d";

String json ="";
String temperature = "";
String pressure ="";
String humidity = "";
String wind = "";


StaticJsonDocument<1000> document;

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);

String date;
String day_name;
String time_of_day;


void setup() {
  // put your setup code here, to run once:

  tft.init();
  tft.setRotation(3);
  tft.fillScreen(BLACK);
  tft.setTextColor(YELLOW); 
  tft.setTextSize(2);

  Serial.begin(115200);
  tft.setCursor(25, 0, 2);
  tft.println("CONNECTING TO");
  tft.setCursor(60, 40, 2);
  tft.println(ssid);
  WiFi.begin(ssid, password);

  while(WiFi.status() != WL_CONNECTED) {
    delay(400);
  }

  tft.println("");
  tft.setCursor(25, 80, 2);
  tft.println("CONNECTED");

  delay(2000);
  tft.setTextColor(YELLOW);
  tft.fillScreen(BLACK);

  getWeatherData();

  tft.setCursor(75, 0, 2);
  tft.println(town);
  tft.setCursor(25, 40, 1);
  tft.println("TEMP");
  tft.setCursor(25, 60, 1);
  tft.println(temperature);
  tft.setCursor(25, 90, 1);
  tft.println("HUM");
  tft.setCursor(25, 110, 1);
  tft.print(humidity + "%");

  //second column

  tft.setCursor(125, 40, 1);
  tft.println("WIND");
  tft.setCursor(125, 60, 1);
  tft.println(wind + " m/s");
  tft.setCursor(125, 90, 1);
  tft.println("PRESSURE");
  tft.setCursor(125, 110, 1);
  tft.println(pressure + " hPa");

}

void loop() {
  // put your main code here, to run repeatedly:

}

void getWeatherData()
{
  if((WiFi.status() == WL_CONNECTED)) {
    HTTPClient http;

    http.begin(link_to_data);
    int httpCode = http.GET();

    if(httpCode > 0){
      json = http.getString();
      Serial.println(json);
    }else {
      Serial.println("Error");
    }

    http.end();
  }

  char inp[1000];
  json.toCharArray(inp, 1000);
  deserializeJson(document, inp);

  String temperature2 = document["main"]["temp"];
  String pressure2 = document["main"]["pressure"];
  String humidity2 = document["main"]["humidity"];
  String wind2 = document["wind"]["speed"];

  temperature = temperature2;
  pressure = pressure2;
  humidity = humidity2;
  wind = wind2;
}
