#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include <SPI.h>
#include "BluetoothSerial.h"

BluetoothSerial SerialBT;

#define HARDWARE_TYPE MD_MAX72XX::ICSTATION_HW
#define MAX_DEVICES 1
#define DATA_PIN 23
#define CLK_PIN 18
#define CS_PIN 5

MD_Parola matriz = MD_Parola(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);

void setup() {
  Serial.begin(115200);
  SerialBT.begin("ESP32_Matriz");
  matriz.begin();
  matriz.setIntensity(10);  // BRILLO ALTO
  matriz.displayClear();
  matriz.displayText("Listo!", PA_CENTER, 100, 0, PA_PRINT, PA_NO_EFFECT);
  delay(2000);
}

void loop() {
  if (SerialBT.available()) {
    String recibido = SerialBT.readStringUntil('\n');
    recibido.trim();
    Serial.println("Mensaje recibido: " + recibido);

    if (recibido.startsWith("@")) {
      int sepIndex = recibido.indexOf('|');
      if (sepIndex > 0 && sepIndex < recibido.length() - 1) {
        String velStr = recibido.substring(1, sepIndex);
        String texto = recibido.substring(sepIndex + 1);
        int velocidad = velStr.toInt();

        Serial.println("Velocidad: " + String(velocidad));
        Serial.println("Texto: " + texto);

        matriz.displayClear();
        matriz.setSpeed(velocidad);
        matriz.displayText(texto.c_str(), PA_LEFT, velocidad, 0, PA_SCROLL_LEFT, PA_SCROLL_LEFT);
        matriz.displayReset();

        while (!matriz.displayAnimate()) {
          delay(50);
        }
      } else {
        Serial.println("❌ Formato incorrecto: falta separador o texto.");
      }
    } else {
      matriz.displayClear();
      matriz.setSpeed(100);
      matriz.displayText(recibido.c_str(), PA_LEFT, 100, 0, PA_SCROLL_LEFT, PA_SCROLL_LEFT);
      matriz.displayReset();

      while (!matriz.displayAnimate()) {
        delay(50);
      }
    }
  }
}
