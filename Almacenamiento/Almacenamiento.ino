#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <NTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
// HUMEDAD
#include "DHT.h"        
#define DHTTYPE DHT11
#define dht_dpin 5
DHT dht(dht_dpin, DHTTYPE); 
// ********************************************** PIN D1 HUMEDAD
// HUMEDAD TERMINA
// POST Y JSON 
#include <ArduinoJson.h>
// VERSION DE ARDUINOJSON.H ES LA 501 5-13-2
#include <ArduinoJson.h>


const int sensorPin = 4;  //********************* PIN D2 PROXIMIDAD
// ***********************+++ VARIABLES DE CONEXION Y SERVER
const char *ssid     = "";
const char *password = "";
String server = "http://192.168.1.134:8529/_db/_system/Sensors/sensors";
HTTPClient http;

//*************************************************** VARIABLES SENSORES Y FECHA
String formattedDate;
String date; 
float h;
int value = 0;
String estado = "";

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);

void setup(){
  Serial.begin(9600);
  pinMode(sensorPin , INPUT);  //definir pin como entrada
  WiFi.begin(ssid, password);
  while ( WiFi.status() != WL_CONNECTED ) {
    delay ( 500 );
    Serial.print ( "." );
  }
  Serial.println("");
  Serial.println("WiFi conectado");     
  Serial.println(WiFi.localIP());
  timeClient.begin();
  dht.begin();
}

void loop() {
  // DATE
  fecha();
  humedad2();
  presensia();
  envio();
  delay(5000);
}

void fecha(){
  timeClient.update();
  date = timeClient.getFormattedDate();
  }
void humedad2(){
  h = dht.readHumidity();
  }
void presensia(){  
  value = digitalRead(sensorPin );  //lectura digital de pin
  if(value == HIGH){
    estado = "Libre";
    }
  if(value == LOW){
    estado = "Ocupado";
    }
  }
void envio(){
  char json[256];
  WiFiClient client;
  HTTPClient http; 
  DynamicJsonBuffer jb;
  JsonObject& root = jb.createObject();
  root["date"] = date;
  root["humedadIV"] = h;
  root["presencia"] = estado;
  root.printTo(json, sizeof(json));
  http.addHeader("Content-Type", "application/json");
  Serial.println("\n");
  Serial.println(json);
  http.begin(client, "http://192.168.1.134:8529/_db/_system/Sensors/sensors");  
  delay(5000);
  http.POST(json);
  int httpResponseCode = http.POST(json);
  Serial.print("respuesta: ");
  Serial.print(httpResponseCode);
  }
