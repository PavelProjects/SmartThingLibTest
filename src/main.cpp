#include <Arduino.h>
#include <SmartThing.h>

void addActions();
void addSensors();
void addConfigEntries();

void setup() {
	// Все действия по настройке библиотеки необходимо выполнить до ее инициализации
	addActions();
	addSensors();
	addConfigEntries();

	// Инициализация библиотеки
	// Необходимо указать тип устройства
	if (!SmartThing.init("test_device")) {
		st_log_error("main", "Failed to init SmartThing!");
		while(true) {
			delay(100);
		}
	}

	st_log_info("main", "Setup finished");
}

void loop() {
	// У ESP8266 нет поддержки FreeRTOS, поэтому необходимо вызывать SmartThing.loop() в главном loop()
	#ifdef ARDUINO_ARCH_ESP8266
		SmartThing.loop();
	#endif
	delay(250);
}

// Настройка действий устройства
void addActions() {
	#if ENABLE_ACTIONS // Проверка, что функционал действий не отключен
		/*
			При добавлении действия необходимо указать:
			- его системное название
			- текст для отображения в UI
			- лямбду с логикой действия, которая возвращает true или false
		*/
		ActionsManager.add("led_off", "Turn led off", []() {
			digitalWrite(LED_PIN, LOW);
			return true;
		});
		ActionsManager.add("led_on", "Turn led on", []() {
			digitalWrite(LED_PIN, HIGH);
			return true;
		});
	#endif
}

// Настройка сенсоров устройства
// Есть два вида сенсоров, которые различаются по возвращаемому результату - числовые и текстовые
void addSensors() {
	// У любого сенсора необходимо указать его название
	#if ENABLE_NUMBER_SENSORS // Проверка, что числовые сенсоры включены
		// Числовой сенсор по умолчанию хранит в себе long
		// Внутри настраивает пин и читает его значение с помощью digitalRead
		SensorsManager.addDigitalSensor("button", 12);
		// Читает значение с пина с помощью analogRead
		SensorsManager.addAnalogSensor("analog", 14);
		// Добавление сенсора с кастомной логикой вычисления значения
		SensorsManager.addSensor("millis", []() {
			return millis();
		});
	#endif
	
	#if ENABLE_TEXT_SENSORS // Проверка, что текстовые сенсоры включены
		// Текстовый сенсор по умолчанию хранит в себе String
		// Пример возможной логики вычисления значений
		SensorsManager.addSensor("led", []() {
			return digitalRead(LED_PIN) == HIGH ? "on" : "off";
		});
    SensorsManager.addSensor("wifi", []() {
      return SmartThing.wifiConnected() ? "connected" : "disconnected";
    });
	#endif
}

// Настройка возможной конфигурации устройства
void addConfigEntries() {
	#if ENABLE_CONFIG
		// Для добавления конфигурации необходимо указать только ее системное имя без пробелов
		ConfigManager.addConfigEntry("test-value");
		
		// На значении конфигурации можно ссылаться в некоторых полях хуков
		// Или получить его значение в коде, по названию конфигурации
		// ConfigManager.getConfigValue("test-value");

		// Можно добавить слушатель на обновление конфигурации
		ConfigManager.onConfigUpdate([]() {
			st_log_debug("main", "Config updated!");
		});
	#endif
}