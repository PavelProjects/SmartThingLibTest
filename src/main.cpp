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
    ST_LOG_INFO("main", "SmartThing successfully initialized");
  } else {
    ST_LOG_ERROR("main", "Failed to init SmartThing!");
  }
  
  if (SmartThing.wifiConnected()) {
    ArduinoOTA.onStart([]() {
      ST_LOG_WARNING("main", "Ota update started");
    });
    ArduinoOTA.onError([](ota_error_t err) {
      ST_LOG_WARNING("main", "Ota update error! code=%d", err);
    });
    ArduinoOTA.onEnd([]() {
      ST_LOG_WARNING("main", "Ota update finsihed!");
    });
    
    #ifdef ARDUINO_ARCH_ESP32
    ArduinoOTA.setMdnsEnabled(false);
    #endif
    #ifdef ARDUINO_ARCH_ESP8266
    ActionsManager.call("led_on");
    #endif
    ArduinoOTA.begin();
  }

  ST_LOG_INFO("main", "Setup finished");
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
  ActionsManager.add("led_off", "Turn led off", []() {
    digitalWrite(LED_PIN, LOW);
    return ActionResult(true);
  });
  ActionsManager.add("led_on", "Turn led on", []() {
    digitalWrite(LED_PIN, HIGH);
    return ActionResult(true);
  });
  #endif
}

void addSensors() {
  #if ENABLE_SENSORS
  ObservablesManager.addDigitalSensor("button", SENSOR_BUTTON_PIN);
  #endif
}

void addStates() {
  #if ENABLE_STATES 
  ObservablesManager.addDeviceState("led", []() {
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
