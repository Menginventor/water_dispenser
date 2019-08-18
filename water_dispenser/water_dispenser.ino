#include "button.h"
#include <EEPROM.h>
/**/
#define VALVE_PIN 6
#define FLOWSENSOR_PIN 2
#define STANBY_STATE 0
#define MANUAL_DISPENS_STATE 1
#define AUTO_DISPENS_STATE 2
#define SETUP_DISPENS_STATE 3
byte crr_state = STANBY_STATE;
Button blue_btn(8, INPUT_PULLUP, LOW);
Button yellow_btn(9, INPUT_PULLUP, LOW);
Button red_btn(10, INPUT_PULLUP, LOW);
/**/
unsigned long flow_counter = 0;
unsigned long prog_flow = 0;
void open_valve() {
  digitalWrite(VALVE_PIN, HIGH);
}
void close_valve() {
  digitalWrite(VALVE_PIN, LOW);
}
/**/
void flow_count() {
  flow_counter ++;
}
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(VALVE_PIN, OUTPUT);
  pinMode(FLOWSENSOR_PIN, INPUT);
  EEPROM.get(0, prog_flow);
  attachInterrupt(digitalPinToInterrupt(FLOWSENSOR_PIN), flow_count, FALLING);
}

void loop() {
  // put your main code here, to run repeatedly:
  blue_btn.update();
  yellow_btn.update();
  red_btn.update();
  switch (crr_state) {
    case STANBY_STATE:

      if (blue_btn.released()) {
        if (yellow_btn.read() == LOW) {
          crr_state = SETUP_DISPENS_STATE;
          Serial.println("SETUP_DISPENS_STATE");
          flow_counter = 0;
          open_valve();
        }
        else {
          crr_state = MANUAL_DISPENS_STATE;
          Serial.println("MANUAL_DISPENS_STATE");
          open_valve();
        }
      }
      else if (yellow_btn.released()) {
        crr_state = AUTO_DISPENS_STATE;
        Serial.println("AUTO_DISPENS_STATE");
        flow_counter = 0;
        open_valve();
      }
      break;
    case MANUAL_DISPENS_STATE:
      if (red_btn.pressed()) {
        crr_state = STANBY_STATE;
        close_valve();
        Serial.println("STANBY_STATE");
      }
      break;
    case AUTO_DISPENS_STATE:
      if (red_btn.pressed() || flow_counter >= prog_flow) {
        crr_state = STANBY_STATE;
        close_valve();
        Serial.println("STANBY_STATE");
      }
     gg
      else {
        //Serial.println(flow_counter);
      }

      break;
    case SETUP_DISPENS_STATE:
      if (red_btn.pressed()) {
        crr_state = STANBY_STATE;
        close_valve();
        prog_flow = flow_counter;
        EEPROM.put(0, prog_flow);
        Serial.println("STANBY_STATE");
      }
      else {
        //Serial.println(flow_counter);
      }
      break;

  }

}
