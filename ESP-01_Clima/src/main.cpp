#include <Arduino.h>

void setup() {
  // put your setup code here, to run once:
  pinMode(LED_BUILTIN, OUTPUT);
}

void LED(bool _state, int _delay){
  digitalWrite(LED_BUILTIN, !_state);
  delay(_delay);
}

void loop() {
  LED(true, 75);
  LED(false, 150);
  LED(true, 75);
  LED(false, 750);
}

