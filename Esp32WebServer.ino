#include <WiFi.h>
#include <WebServer.h>
#include <ArduinoJson.h>
 
#include "config.h"  // Sustituir con datos de vuestra red
#include "Server.hpp"
#include "ESP32_Utils.hpp"

#include <HTTPClient.h>

const char* serverUrl = "http://localhost/endpoint";  // Reemplazar la url

void setup() {
  Serial.begin(9600);

  // Conexión a la red WiFi
  Serial.println("Conectando a WiFi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectando...");
  }
  Serial.println("Conectado a la red WiFi");

  // Realizar la solicitud POST
  HTTPClient http;
  http.begin(serverUrl);
  http.addHeader("Content-Type", "application/json");

  // Construir el JSON para enviar al servidor
  StaticJsonDocument<100> doc; // Tamaño del documento JSON, ajusta según tus necesidades
  doc["ip"] = WiFi.localIP().toString();
  doc["identificador"] = 1;
  doc["nombre"] = "Facultad de la energía";
  doc["latitud"] = -4.030185;
  doc["longitud"] = -79.199459;

  String jsonString;
  serializeJson(doc, jsonString);
  
  int httpResponseCode = http.POST(jsonString);
  if (httpResponseCode > 0) {
    Serial.print("Solicitud POST exitosa, código de respuesta: ");
    Serial.println(httpResponseCode);
    String response = http.getString();
    Serial.println("Respuesta del servidor: " + response);
  } else {
    Serial.print("Error en la solicitud POST, código de error: ");
    Serial.println(httpResponseCode);
  }
  http.end();

  // Llamar a la función InitServer después de realizar la solicitud POST
  InitServer();
}

 
void loop()
{
   server.handleClient();
}