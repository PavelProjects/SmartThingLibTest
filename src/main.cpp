//Auto-generated code
#include <Arduino.h>
#include <SmartThing.h>
#include <ArduinoOTA.h>

#define SENSOR_BUTTON_PIN 12

void addSensors();
void addActions();
void addConfigEntries();

void setup() {
  addSensors();
  addActions();
  addConfigEntries();

  if (SmartThing.init("test_device")) {
    st_log_info("main", "SmartThing successfully initialized");
  } else {
    st_log_error("main", "Failed to init SmartThing!");
  }
  
  if (SmartThing.wifiConnected()) {
    ArduinoOTA.onStart([]() {
      st_log_warning("main", "Ota update started");
    });
    ArduinoOTA.onError([](ota_error_t err) {
      st_log_warning("main", "Ota update error! code=%d", err);
    });
    ArduinoOTA.onEnd([]() {
      st_log_warning("main", "Ota update finsihed!");
    });
    
    #ifdef ARDUINO_ARCH_ESP32
    ArduinoOTA.setMdnsEnabled(false);
    #endif
    #ifdef ARDUINO_ARCH_ESP8266
    ActionsManager.call("led_on");
    #endif
    ArduinoOTA.begin();
  }

  st_log_info("main", "Setup finished");
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
  #if ENABLE_NUMBER_SENSORS
  SensorsManager.addDigitalSensor("button", SENSOR_BUTTON_PIN);
  #endif
  #if ENABLE_TEXT_SENSORS 
  SensorsManager.addSensor("led", []() {
    // Пример возможной логики вычисления значения
    return digitalRead(LED_PIN) == HIGH ? "on" : "off"; 
  });
  #endif
}

void addConfigEntries() {
  SettingsRepository.addConfigEntry("testb", "test boolean value", CONFIG_BOOLEAN);
  SettingsRepository.addConfigEntry("testn", "test number value", CONFIG_INTEGER);
  SettingsRepository.addConfigEntry("tests", "test string value", CONFIG_STRING);
}
