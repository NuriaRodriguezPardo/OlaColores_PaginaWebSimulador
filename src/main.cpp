#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "RedmiNuria";
const char* password = "Patata123";

const int numRows = 10; // Número de filas en la matriz de LEDs de la simulación
const int numCols = 10; // Número de columnas en la matriz de LEDs de la simulación

WebServer server(80);

void handleRoot();

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectando al WiFi...");
  }

  Serial.println("Conectado al WiFi");
  Serial.print("Dirección IP: ");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);
  server.begin();
  Serial.println("Servidor iniciado");
  delay(100);
}

void loop() {
  server.handleClient();
}

void handleRoot() {
  String paginaWeb = "<!DOCTYPE html>\
<html lang=\"en\">\
<head>\
  <meta charset=\"UTF-8\">\
  <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\
  <title>Simulador de LEDs</title>\
  <style>\
    body {\
      font-family: 'Arial', sans-serif;\
      background-color: #f0f0f0;\
      text-align: center;\
    }\
    h1 {\
      color: #333;\
      margin-top: 50px;\
    }\
    .led {\
      width: 30px;\
      height: 30px;\
      border-radius: 50%;\
      display: inline-block;\
      margin: 5px;\
      background-color: black;\
      cursor: pointer;\
      box-shadow: 0 0 10px rgba(0,0,0,0.5);\
    }\
    button {\
      padding: 10px 20px;\
      font-size: 16px;\
      background-color: #4CAF50;\
      color: white;\
      border: none;\
      border-radius: 5px;\
      cursor: pointer;\
      margin-top: 20px;\
    }\
    .led.on {\
      background-color: #007bff;\
    }\
  </style>\
</head>\
<body>\
  <h1>Simulador de LEDs</h1>";

  // Generar LEDs
  for (int row = 0; row < numRows; row++) {
    for (int col = 0; col < numCols; col++) {
      paginaWeb += "<div class=\"led\" id=\"led_" + String(row) + "_" + String(col) + "\" onclick=\"toggleLED('led_" + String(row) + "_" + String(col) + "')\"></div>";
    }
    paginaWeb += "<br>";
  }

  // Añadir botón para activar la ola
  paginaWeb += "<button onclick=\"activarOla()\">Activar Ola</button>";

  // Script JavaScript
  paginaWeb += "<script>\
    var leds = document.querySelectorAll('.led');\
    var intervalo;\
\
    function toggleLED(id) {\
      var led = document.getElementById(id);\
      var estado = led.classList.contains('on');\
      if (estado) {\
        led.classList.remove('on');\
      } else {\
        led.classList.add('on');\
      }\
    }\
\
    function activarOla() {\
      var contador = 0;\
      intervalo = setInterval(function() {\
        leds.forEach(function(led, index) {\
          var columna = index % " + String(numCols) + ";\
          var fila = Math.floor(index / " + String(numCols) + ");\
          if (fila === contador) {\
            setTimeout(function() {\
              toggleLED('led_' + fila + '_' + columna);\
            }, 100 * columna);\
          }\
        });\
        contador++;\
        if (contador === " + String(numRows) + ") {\
          clearInterval(intervalo);\
          setTimeout(function() { activarOlaReversa(); }, 2000);\
        }\
      }, 500);\
    }\
\
    function activarOlaReversa() {\
      var contador = " + String(numRows - 1) + ";\
      intervalo = setInterval(function() {\
        leds.forEach(function(led, index) {\
          var columna = index % " + String(numCols) + ";\
          var fila = Math.floor(index / " + String(numCols) + ");\
          if (fila === contador) {\
            setTimeout(function() {\
              toggleLED('led_' + fila + '_' + columna);\
            }, 100 * columna);\
          }\
        });\
        contador--;\
        if (contador === -1) {\
          clearInterval(intervalo);\
        }\
      }, 500);\
    }\
\
  </script>\
</body>\
</html>";

  server.send(200, "text/html", paginaWeb);
}
