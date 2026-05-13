#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

// ── Pines ────────────────────────────────────────────────
#define IN1_MOTOR 2
#define IN2_MOTOR 3
#define BTN_OPEN  4
#define BTN_CLOSE 5
#define PIN_RGB   10

// ── Tiempos de giro (ms) ─────────────────────────────────
#define TIEMPO_ABRIR  2500
#define TIEMPO_CERRAR 3700

#define NUM_LEDS 12

// ── NeoPixel ─────────────────────────────────────────────
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, PIN_RGB, NEO_GRB + NEO_KHZ800);

uint8_t  r_act, g_act, b_act;
float    posLed = 0;
float    velLed = 0.3;
unsigned long tLed = 0;

void setColor(uint8_t r, uint8_t g, uint8_t b) {
  r_act = r; g_act = g; b_act = b;
}

void tickLed() {
  if (millis() - tLed < 8) return;
  tLed = millis();

  posLed += velLed;
  if (posLed >= NUM_LEDS) posLed -= NUM_LEDS;
  if (posLed < 0)         posLed += NUM_LEDS;

  strip.clear();
  for (int i = 0; i < NUM_LEDS; i++) {
    float dist = fabs(fmod((i - posLed + NUM_LEDS), NUM_LEDS));
    if (dist > NUM_LEDS / 2.0) dist = NUM_LEDS - dist;

    float brillo = 0;
    if (dist < 5.0) {
      brillo = pow(1.0 - (dist / 5.0), 1.5);
    }

    strip.setPixelColor(i, strip.Color(
      (uint8_t)(r_act * brillo),
      (uint8_t)(g_act * brillo),
      (uint8_t)(b_act * brillo)
    ));
  }
  strip.show();
}

// ── Motor ────────────────────────────────────────────────
void motorStop()  { digitalWrite(IN1_MOTOR, LOW);  digitalWrite(IN2_MOTOR, LOW);  }
void motorAbrir() { digitalWrite(IN1_MOTOR, HIGH); digitalWrite(IN2_MOTOR, LOW);  }
void motorCerrar(){ digitalWrite(IN1_MOTOR, LOW);  digitalWrite(IN2_MOTOR, HIGH); }

// ── Estado ───────────────────────────────────────────────
enum Estado { ABIERTO, CERRADO, ABRIENDO, CERRANDO };
Estado estado = ABIERTO;
unsigned long tiempoInicio = 0;

bool lastBtn1 = HIGH;
bool lastBtn2 = HIGH;

// ── Setup ────────────────────────────────────────────────
void setup() {
  pinMode(BTN_OPEN,  INPUT_PULLUP);
  pinMode(BTN_CLOSE, INPUT_PULLUP);
  pinMode(IN1_MOTOR, OUTPUT);
  pinMode(IN2_MOTOR, OUTPUT);
  motorStop();

  strip.begin();
  strip.setBrightness(150);
  strip.clear();
  strip.show();

  // Rutina inicio: abrir completamente sin importar posicion previa
  setColor(128, 0, 128);  // Morado
  velLed = 0.5;
  motorAbrir();
  unsigned long t = millis();
  while (millis() - t < TIEMPO_ABRIR) tickLed();
  motorStop();

  setColor(0, 255, 80);   // Verde neon = listo
  velLed = 0.15;
  estado = ABIERTO;
}

// ── Loop ─────────────────────────────────────────────────
void loop() {
  bool btn1 = digitalRead(BTN_OPEN)  == LOW;
  bool btn2 = digitalRead(BTN_CLOSE) == LOW;

  // Pulso OPEN — actua solo si esta cerrado
  if (lastBtn1 == HIGH && btn1 == LOW) {
    if (estado == CERRADO) {
      motorAbrir();
      tiempoInicio = millis();
      estado = ABRIENDO;
      setColor(180, 0, 255);  // Morado brillante
      velLed = 0.45;
    }
  }

  // Pulso CLOSE — actua solo si esta abierto
  if (lastBtn2 == HIGH && btn2 == LOW) {
    if (estado == ABIERTO) {
      motorCerrar();
      tiempoInicio = millis();
      estado = CERRANDO;
      setColor(0, 150, 255);  // Azul cyan
      velLed = -0.45;
    }
  }

  lastBtn1 = btn1;
  lastBtn2 = btn2;

  // Fin de apertura
  if (estado == ABRIENDO && millis() - tiempoInicio >= TIEMPO_ABRIR) {
    motorStop();
    estado = ABIERTO;
    setColor(0, 255, 80);   // Verde neon
    velLed = 0.12;
  }

  // Fin de cierre
  if (estado == CERRANDO && millis() - tiempoInicio >= TIEMPO_CERRAR) {
    motorStop();
    estado = CERRADO;
    setColor(0, 255, 80);   // Verde neon
    velLed = -0.12;
  }

  tickLed();
}
