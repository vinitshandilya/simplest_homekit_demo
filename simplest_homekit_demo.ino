#include <Arduino.h>
#include <arduino_homekit_server.h>
#include "wifi_info.h"

#define LOG_D(fmt, ...)   printf_P(PSTR(fmt "\n") , ##__VA_ARGS__);

// Switches & Relay GPIO definition
#define BUILTIN_LED 2

#define S1 15
#define R1 5

#define S2 13
#define R2 4

#define S3 15
#define R3 14

#define S4 13
#define R4 12

#define DEBUG_SW 0

int sw1_flag = 0;
int sw2_flag = 0;
int sw3_flag = 0;
int sw4_flag = 0;

void setup() {
  Serial.begin(115200);
  wifi_connect(); // in wifi_info.h
  //homekit_storage_reset(); // to remove the previous HomeKit pairing storage when you first run this new HomeKit example
  my_homekit_setup();
}

void loop() {
  my_homekit_loop();
  delay(10);
  wifi_monitor();
  delay(100);
  
}

extern "C" homekit_server_config_t config;
extern "C" homekit_characteristic_t cha_switch1_on;
extern "C" homekit_characteristic_t cha_switch2_on;
extern "C" homekit_characteristic_t cha_switch3_on;
extern "C" homekit_characteristic_t cha_switch4_on;

static uint32_t next_heap_millis = 0;

//Called when the switch value is changed by iOS Home APP
void cha_switch1_on_setter(const homekit_value_t value) {
  bool on = value.bool_value;
  cha_switch1_on.value.bool_value = on;  //sync the value
  LOG_D("Switch: %s", on ? "ON" : "OFF");
  digitalWrite(R1, on ? LOW : HIGH);
  digitalWrite(BUILTIN_LED, on ? LOW : HIGH);
}

//Called when the switch value is changed by iOS Home APP
void cha_switch2_on_setter(const homekit_value_t value) {
  bool on = value.bool_value;
  cha_switch2_on.value.bool_value = on;  //sync the value
  LOG_D("Switch: %s", on ? "ON" : "OFF");
  digitalWrite(R2, on ? LOW : HIGH);
  digitalWrite(BUILTIN_LED, on ? LOW : HIGH);
}

//Called when the switch value is changed by iOS Home APP
void cha_switch3_on_setter(const homekit_value_t value) {
  bool on = value.bool_value;
  cha_switch3_on.value.bool_value = on;  //sync the value
  LOG_D("Switch: %s", on ? "ON" : "OFF");
  digitalWrite(R3, on ? LOW : HIGH);
  digitalWrite(BUILTIN_LED, on ? LOW : HIGH);
}

//Called when the switch value is changed by iOS Home APP
void cha_switch4_on_setter(const homekit_value_t value) {
  bool on = value.bool_value;
  cha_switch4_on.value.bool_value = on;  //sync the value
  LOG_D("Switch: %s", on ? "ON" : "OFF");
  digitalWrite(R4, on ? LOW : HIGH);
  digitalWrite(BUILTIN_LED, on ? LOW : HIGH);
}

void my_homekit_setup()
{
  pinMode(BUILTIN_LED, OUTPUT);
  
  pinMode(S1, INPUT);
  pinMode(R1, OUTPUT);

  pinMode(S2, INPUT);
  pinMode(R2, OUTPUT);

  pinMode(S3, INPUT);
  pinMode(R3, OUTPUT);

  pinMode(S4, INPUT);
  pinMode(R4, OUTPUT);
  
  //Add the .setter function to get the switch-event sent from iOS Home APP.
  //The .setter should be added before arduino_homekit_setup.
  //HomeKit sever uses the .setter_ex internally, see homekit_accessories_init function.
  //Maybe this is a legacy design issue in the original esp-homekit library,
  //and I have no reason to modify this "feature".
  cha_switch1_on.setter = cha_switch1_on_setter;
  cha_switch2_on.setter = cha_switch2_on_setter;
  cha_switch3_on.setter = cha_switch3_on_setter;
  cha_switch4_on.setter = cha_switch4_on_setter;
  
  arduino_homekit_setup(&config);
}


void my_homekit_loop() {
  arduino_homekit_loop();
  switch_feedback_function();
  const uint32_t t = millis();
  if (t > next_heap_millis) {
    // show heap info every 5 seconds
    next_heap_millis = t + 5 * 1000;
    LOG_D("Free heap: %d, HomeKit clients: %d",
          ESP.getFreeHeap(), arduino_homekit_connected_clients_count());

  }

}

void switch_feedback_function()
{

  if (digitalRead(S1) == LOW)
  {
    if (sw1_flag == 0 )
    {
      digitalWrite(R1, LOW);
      digitalWrite(BUILTIN_LED, LOW);
      if (DEBUG_SW) Serial.println("Relay1- ON");
      //report the switch value to HomeKit if it is changed (e.g. by a physical button)
      bool switch1_is_on = 1;
      cha_switch1_on.value.bool_value = switch1_is_on;
      homekit_characteristic_notify(&cha_switch1_on, cha_switch1_on.value);
      sw1_flag = 1;
    }
    if (DEBUG_SW) Serial.println("Switch1 -ON");

  }
  if (digitalRead(S1) == HIGH )
  {
    if (sw1_flag == 1)
    {
      digitalWrite(R1, HIGH);
      digitalWrite(BUILTIN_LED, HIGH);
      if (DEBUG_SW) Serial.println("Relay1 OFF");
      //report the switch value to HomeKit if it is changed (e.g. by a physical button)
      bool switch1_is_on = 0;
      cha_switch1_on.value.bool_value = switch1_is_on;
      homekit_characteristic_notify(&cha_switch1_on, cha_switch1_on.value);

      sw1_flag = 0;
    }
    if (DEBUG_SW)Serial.println("Switch1 OFF");
  }


  if (digitalRead(S2) == LOW)
  {
    if (sw2_flag == 0 )
    {
      digitalWrite(R2, LOW);
      digitalWrite(BUILTIN_LED, LOW);
      if (DEBUG_SW)  Serial.println("Relay2- ON");
      //report the switch value to HomeKit if it is changed (e.g. by a physical button)
      bool switch2_is_on = 1;
      cha_switch2_on.value.bool_value = switch2_is_on;
      homekit_characteristic_notify(&cha_switch2_on, cha_switch2_on.value);
      sw2_flag = 1;
    }
    if (DEBUG_SW) Serial.println("Switch2 -ON");

  }
  if (digitalRead(S2) == HIGH )
  {
    if (sw2_flag == 1)
    {
      digitalWrite(R2, HIGH);
      digitalWrite(BUILTIN_LED, HIGH);
      if (DEBUG_SW) Serial.println("Relay2 OFF");
      //report the switch value to HomeKit if it is changed (e.g. by a physical button)
      bool switch2_is_on = 0;
      cha_switch2_on.value.bool_value = switch2_is_on;
      homekit_characteristic_notify(&cha_switch2_on, cha_switch2_on.value);
      sw2_flag = 0;
    }
    if (DEBUG_SW)Serial.println("Switch2 OFF");
    //delay(200);
  }

  if (digitalRead(S3) == LOW)
  {
    if (sw3_flag == 0 )
    {
      digitalWrite(R3, LOW);
      digitalWrite(BUILTIN_LED, LOW);
      if (DEBUG_SW) Serial.println("Relay3- ON");
      //report the switch value to HomeKit if it is changed (e.g. by a physical button)
      bool switch3_is_on = 1;
      cha_switch3_on.value.bool_value = switch3_is_on;
      homekit_characteristic_notify(&cha_switch3_on, cha_switch3_on.value);
      sw3_flag = 1;
    }
    if (DEBUG_SW) Serial.println("Switch3 -ON");

  }
  if (digitalRead(S3) == HIGH )
  {
    if (sw3_flag == 1)
    {
      digitalWrite(R3, HIGH);
      digitalWrite(BUILTIN_LED, HIGH);
      if (DEBUG_SW) Serial.println("Relay3 OFF");
      //report the switch value to HomeKit if it is changed (e.g. by a physical button)
      bool switch3_is_on = 0;
      cha_switch3_on.value.bool_value = switch3_is_on;
      homekit_characteristic_notify(&cha_switch3_on, cha_switch3_on.value);

      sw3_flag = 0;
    }
    if (DEBUG_SW)Serial.println("Switch3 OFF");
  }

  if (digitalRead(S4) == LOW)
  {
    if (sw4_flag == 0 )
    {
      digitalWrite(R4, LOW);
      digitalWrite(BUILTIN_LED, LOW);
      if (DEBUG_SW) Serial.println("Relay4- ON");
      //report the switch value to HomeKit if it is changed (e.g. by a physical button)
      bool switch4_is_on = 1;
      cha_switch4_on.value.bool_value = switch4_is_on;
      homekit_characteristic_notify(&cha_switch4_on, cha_switch4_on.value);
      sw4_flag = 1;
    }
    if (DEBUG_SW) Serial.println("Switch4 -ON");

  }
  if (digitalRead(S4) == HIGH )
  {
    if (sw4_flag == 1)
    {
      digitalWrite(R4, HIGH);
      digitalWrite(BUILTIN_LED, HIGH);
      if (DEBUG_SW) Serial.println("Relay4 OFF");
      //report the switch value to HomeKit if it is changed (e.g. by a physical button)
      bool switch4_is_on = 0;
      cha_switch4_on.value.bool_value = switch4_is_on;
      homekit_characteristic_notify(&cha_switch4_on, cha_switch4_on.value);

      sw4_flag = 0;
    }
    if (DEBUG_SW)Serial.println("Switch4 OFF");
  }    

}
