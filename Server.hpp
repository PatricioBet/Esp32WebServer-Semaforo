#include "sys/_types.h"
#include "WString.h"
#include "esp32-hal.h"
#include "HardwareSerial.h"
#include <ctype.h>
#include <base64.h>
const int portPin = 35;
WebServer server(80);


// Funcion que se ejecutara en la URI '/'
void handleRoot() {
  server.send(200, "text/plain", "Hola mundo!");
}

// Funcion que se ejecutara en URI desconocida
void handleNotFound() {
  server.send(404, "text/plain", "Not found--");
}


String medicionuv(){
  float lectura = analogRead(portPin);
  float potValor = 0.0;
  potValor = map(lectura, 0, 4095, 0.0, 1500.0)/100.0;
  Serial.println(potValor);
  Serial.println(lectura);
  return (String)potValor;
}

//http://<dirección_ip_del_esp32>/console?comando=show_version

String id = "1";

void handleUV() {
  String aux = "";
  aux = "\"code\": 200,\"msg\": \"OK\",\"info\": {\"identificador\": " + id +", \"uv\": "+medicionuv()+"}}";
 
  aux = "{"+aux;
  server.send(200, "application/json", aux);
}


void InitServer() {
  // Ruteo para '/'
  server.on("/", handleRoot);

  // Ruteo para '/inline' usando función lambda
  server.on("/inline", []() {
    server.send(200, "text/plain", "Esto tambien funciona");
  });


  server.on("/radiacion", handleUV);



  // Ruteo para URI desconocida
  server.onNotFound(handleNotFound);

  // Iniciar servidor
  server.begin();
  Serial.println("HTTP server started");
}