#include <Process.h>
#include <LiquidCrystal.h>
#include <Adafruit_NeoPixel.h>

#define PinNeo 6
#define PinBoton 7
#define CantidadLed 300

LiquidCrystal LCD(12, 11, 5, 4, 3, 2);
Adafruit_NeoPixel TiraLed = Adafruit_NeoPixel(CantidadLed, PinNeo, NEO_GRB + NEO_KHZ800);

String NombreGlobal = "";
String ColorGlobal = "";
String NombreAnimaciones[] = {"secuencia", "blanco", "rojo", "verde", "azul", "amarillo", "cian", "rosado", "fiesta", "arcoiris"};
int CantidadAnimaciones = 10;
int EstadoLed = 1;
uint16_t I_Supremo = 0;

void setup() {
  LCD.begin(16, 2);
  LCD.clear();
  LCD.print("NeoTree ALSW");

  Bridge.begin();        // Iniciar puente entre el Arduino y OpenWRT
  Serial.begin(9600);     // Serial con la computadora

  TiraLed.begin();
  TiraLed.show();

  pinMode(PinBoton, INPUT);
}

void loop() {
  if (digitalRead(PinBoton) == 1) {
    Serial.println("Buscando en FB");
    LCD.setCursor(0, 1);
    LCD.print("Buscando en FB");
    EjecutarFB();
  }
  ActualizarLed();
}


void ActualizarLed() {
  switch (EstadoLed) {
    case 1://Banco
      LineaColor(TiraLed.Color(100, 100, 100), 50);
      break;
    case 2://Rojo
      LineaColor(TiraLed.Color(255, 0, 0), 50);
      break;
    case 3://Verde
      LineaColor(TiraLed.Color(0, 255, 0), 50);
      break;
    case 4://Azul
      LineaColor(TiraLed.Color(0, 0, 255), 50);
      break;
    case 5://Amarillo
      LineaColor(TiraLed.Color(100, 100, 0), 50);
      break;
    case 6://Cian
      LineaColor(TiraLed.Color(0, 100, 100), 50);
      break;
    case 7://Azul
      LineaColor(TiraLed.Color(100, 0, 100), 50);
      break;
  }
}

// Fill the dots one after the other with a color
void LineaColor(uint32_t c, uint8_t wait) {
  // for(uint16_t i=0; i<strip.numPixels(); i++) {
  TiraLed.setPixelColor(I_Supremo, c);
  TiraLed.show();
  delay(wait);
  I_Supremo++;
  if (I_Supremo > TiraLed.numPixels())
    I_Supremo == 0;
  // }
}

void EjecutarFB() {
  ////mnt/sda1/ColorFB.py
  Process MiScript;
  MiScript.begin("/bin/ColorFB.py");
  MiScript.run();

  bool BanderaNombre = false;
  bool BanderaColor = false;
  String NombreFB = "";
  String ColorFB = "";

  while (MiScript.available()) {
    char Carracter = (char)MiScript.read();
    if ( Carracter == '#') {
      BanderaColor = true;
    }
    else if (Carracter == '@') {
      BanderaNombre = true;
    }
    else if (BanderaNombre) {
      if (Carracter == '\n') {
        BanderaNombre = false;
      }
      else {
        NombreFB += Carracter;
      }
    }
    else if (BanderaColor) {
      if (Carracter == ' ' || Carracter == '\n') {
        BanderaColor = false;
      }
      else {
        ColorFB += Carracter;
      }
      Serial.print(Carracter);
    }
  }

  if (ColorFB != "") {
    int AnimacionBuscar = -1;
    for (int i = 0; i < CantidadAnimaciones; i++) {
      if (ColorFB.equalsIgnoreCase(NombreAnimaciones[i])) {
        AnimacionBuscar = i;
      }
    }
    if (AnimacionBuscar > 0) {
      Serial.print("Animacion: ");
      Serial.println(AnimacionBuscar);
      LCD.clear();
      LCD.setCursor(0, 0);
      LCD.print("NeoArbol ALSW");
      NombreGlobal = NombreFB;
      ColorGlobal = ColorFB;
      LCD.setCursor(0, 1);
      LCD.print(NombreGlobal);
      I_Supremo = 0;
      EstadoLed = AnimacionBuscar;
    }


  }
  Serial.print("Nombre: ");
  Serial.println(NombreFB);
  Serial.print("Color: ");
  Serial.println(ColorFB);

}

