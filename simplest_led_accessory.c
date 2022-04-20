#include <Arduino.h>
#include <homekit/types.h>
#include <homekit/homekit.h>
#include <homekit/characteristics.h>
#include <stdio.h>
#include <port.h>

//const char * buildTime = __DATE__ " " __TIME__ " GMT";

#define ACCESSORY_NAME  ("NODEMCU")
#define ACCESSORY_SN  ("SN_0123456")  //SERIAL_NUMBER
#define ACCESSORY_MANUFACTURER ("Arduino Homekit")
#define ACCESSORY_MODEL  ("ESP8266")

#define PIN_LED  2 //D4

bool led_power1 = false; //true or flase
bool led_power2 = false; //true or flase

homekit_value_t led_on_get1() {
	return HOMEKIT_BOOL(led_power1);
}

void led_on_set1(homekit_value_t value) {
	if (value.format != homekit_format_bool) {
		printf("Invalid on-value format: %d\n", value.format);
		return;
	}
	led_power1 = value.bool_value;
	led_update1();
}

homekit_value_t led_on_get2() {
  return HOMEKIT_BOOL(led_power2);
}

void led_on_set2(homekit_value_t value) {
  if (value.format != homekit_format_bool) {
    printf("Invalid on-value format: %d\n", value.format);
    return;
  }
  led_power2 = value.bool_value;
  led_update2();
}

homekit_characteristic_t name = HOMEKIT_CHARACTERISTIC_(NAME, ACCESSORY_NAME);
homekit_characteristic_t serial_number = HOMEKIT_CHARACTERISTIC_(SERIAL_NUMBER, ACCESSORY_SN);
homekit_characteristic_t led_on1 = HOMEKIT_CHARACTERISTIC_(ON, false, .getter=led_on_get1, .setter=led_on_set1);
homekit_characteristic_t led_on2 = HOMEKIT_CHARACTERISTIC_(ON, false, .getter=led_on_get2, .setter=led_on_set2);

void led_update1() {
	if (led_power1) {
   printf("ON\n");
   digitalWrite(2, LOW);
	} else {
		printf("OFF\n");
		digitalWrite(2, HIGH);
	}
}

void led_update2() {
  if (led_power2) {
   printf("ON\n");
   digitalWrite(16, LOW);
  } else {
    printf("OFF\n");
    digitalWrite(16, HIGH);
  }
}

void led_toggle() {
	led_on1.value.bool_value = !led_on1.value.bool_value;
	led_on1.setter(led_on1.value);
	homekit_characteristic_notify(&led_on1, led_on1.value);

  led_on2.value.bool_value = !led_on2.value.bool_value;
  led_on2.setter(led_on2.value);
  homekit_characteristic_notify(&led_on2, led_on2.value);
}

void accessory_identify(homekit_value_t _value) {
	printf("accessory identify\n");
	for (int j = 0; j < 3; j++) {
		led_power1 = true;
		led_update1();
		delay(100);
		led_power2 = false;
		led_update2();
		delay(100);
	}
}

homekit_accessory_t *accessories[] =
		{
				HOMEKIT_ACCESSORY(
						.id = 1,
						.category = homekit_accessory_category_lightbulb,
						.services=(homekit_service_t*[]){
						  HOMEKIT_SERVICE(ACCESSORY_INFORMATION,
						  .characteristics=(homekit_characteristic_t*[]){
						    &name,
						    HOMEKIT_CHARACTERISTIC(MANUFACTURER, ACCESSORY_MANUFACTURER),
						    &serial_number,
						    HOMEKIT_CHARACTERISTIC(MODEL, ACCESSORY_MODEL),
						    HOMEKIT_CHARACTERISTIC(FIRMWARE_REVISION, "0.0.1"),
						    HOMEKIT_CHARACTERISTIC(IDENTIFY, accessory_identify),
						    NULL
						  }),
						  HOMEKIT_SERVICE(LIGHTBULB, .primary=true,
						  .characteristics=(homekit_characteristic_t*[]){
						    HOMEKIT_CHARACTERISTIC(NAME, "Led1"),
						    &led_on1,
						    NULL
						  }),
						  NULL
						}),

        HOMEKIT_ACCESSORY(
            .id = 2,
            .category = homekit_accessory_category_lightbulb,
            .services=(homekit_service_t*[]){
              HOMEKIT_SERVICE(ACCESSORY_INFORMATION,
              .characteristics=(homekit_characteristic_t*[]){
                &name,
                HOMEKIT_CHARACTERISTIC(MANUFACTURER, ACCESSORY_MANUFACTURER),
                &serial_number,
                HOMEKIT_CHARACTERISTIC(MODEL, ACCESSORY_MODEL),
                HOMEKIT_CHARACTERISTIC(FIRMWARE_REVISION, "0.0.1"),
                HOMEKIT_CHARACTERISTIC(IDENTIFY, accessory_identify),
                NULL
              }),
              HOMEKIT_SERVICE(LIGHTBULB, .primary=true,
              .characteristics=(homekit_characteristic_t*[]){
                HOMEKIT_CHARACTERISTIC(NAME, "Led2"),
                &led_on2,
                NULL
              }),
              NULL
            }),           
				NULL
		};

homekit_server_config_t config = {
		.accessories = accessories,
		.password = "111-11-111",
		//.on_event = on_homekit_event,
		.setupId = "ABCD"
};

void accessory_init() {
	pinMode(2, OUTPUT);
  pinMode(16, OUTPUT);
	led_update1();
  led_update2();
}
