//Auto-generated code
#include <Arduino.h>
#include <SmartThing.h>
#include <ArduinoOTA.h>

#define SENSOR_BUTTON_PIN 12

void addStates();
void addSensors();
void addActions();
void addConfigEntries();

void setup() {
  addStates();
  addSensors();
  addActions();
  addConfigEntries();



  if (SmartThing.init("test_device")) {
    LOGGER.info("main", "SmartThing successfully initialized");
  } else {
    LOGGER.error("main", "Failed to init SmartThing!");
  }
  
  if (SmartThing.wifiConnected()) {
    ArduinoOTA.onStart([]() {
      LOGGER.warning("main", "Ota update started");
    });
    ArduinoOTA.onError([](ota_error_t err) {
      LOGGER.warning("main", "Ota update error! code=%d", err);
    });
    ArduinoOTA.onEnd([]() {
      LOGGER.warning("main", "Ota update finsihed!");
    });
    
    #ifdef ARDUINO_ARCH_ESP32
    ArduinoOTA.setMdnsEnabled(false);
    #endif
    #ifdef ARDUINO_ARCH_ESP8266
    SmartThing.callAction("led_on");
    #endif
    ArduinoOTA.begin();
  }

  LOGGER.info("main", "Setup finished");
}

void loop() {
  if (SmartThing.wifiConnected()) {
    ArduinoOTA.handle();
  }
  //Your loop logic here
  #ifdef ARDUINO_ARCH_ESP8266
  SmartThing.loop();
  #endif
  delay(250);
}

void addActions() {
  #if ENABLE_ACTIONS 
  SmartThing.addActionHandler("led_off", "Turn led off", []() {
    digitalWrite(LED_PIN, LOW);
    return ActionResult(true);
  });
  SmartThing.addActionHandler("led_on", "Turn led on", []() {
    digitalWrite(LED_PIN, HIGH);
    return ActionResult(true);
  });
  #endif
}

void addSensors() {
  #if ENABLE_SENSORS
  SmartThing.addDigitalSensor("button", SENSOR_BUTTON_PIN);
  #endif
}

void addStates() {
  #if ENABLE_STATES 
  SmartThing.addDeviceState("led", []() {
    // Пример возможной логики вычисления значения
    return digitalRead(LED_PIN) == HIGH ? "on" : "off"; 
  });
  #endif
}

void addConfigEntries() {
  SmartThing.addConfigEntry("testb", "test boolean value", "boolean");
  SmartThing.addConfigEntry("testn", "test number value", "number");
  SmartThing.addConfigEntry("tests", "test string value", "string");
}
