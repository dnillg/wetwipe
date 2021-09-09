#include "Arduino.h"
#include "Servo.h"
#include <Wire.h>
#include <Adafruit_APDS9960.h>

#include "FrequencyRegulator.h"
#include "FrequencyController.h"
#include "LedController.h"
#include "DoorController.h"
#include "TriggerController.h"

#define PIN_BUTTON_INPUT 4
#define PIN_SPEAKER 3
#define PIN_SERVO_CONTROL 9
#define PIN_BUTTON_HIGH 7
#define PIN_LED_R 5
#define PIN_LED_G 6
#define PIN_LED_B 11
#define SERVO_POS_CLOSED 10
#define SERVO_POS_OPEN 174
#define SERVO_OPEN_MILLIS 5000
#define TARGET_FREQ 100u


typedef struct {
	Servo* servo;
	FrequencyController* frequencyRegulator;
	LedController* ledController;
	DoorController* doorController;
	TriggerController* triggerController;
} Context;

Context context;

void playSound(uint16_t val, uint16_t len) {
	tone(PIN_SPEAKER, val, len);
}

void playWelcomeNote() {
	playSound(500, 100);
	delay(300);
	playSound(500, 50);
	delay(150);
	playSound(500, 50);
	delay(150);
	playSound(2000, 100);
}

void playWelcomeSequence() {
	context.ledController->setLedColor(255, 255, 255);
	context.doorController->setDoorPosition(0);
	playWelcomeNote();
	context.doorController->setDoorPosition(60);
	delay(500);
	context.doorController->setDoorPosition(40);
	delay(100);
	context.doorController->setDoorPosition(60);
	delay(100);
	context.doorController->closeDoor();
	delay(1000);
	context.ledController->enableRainbow();
}

void setup() {
	Serial.begin(115200);
	while (!Serial) {
	}
	Wire.begin();

	pinMode(PIN_BUTTON_INPUT, INPUT);
	pinMode(PIN_BUTTON_HIGH, OUTPUT);
	pinMode(PIN_LED_R, OUTPUT);
	pinMode(PIN_LED_G, OUTPUT);
	pinMode(PIN_LED_B, OUTPUT);
	pinMode(PIN_SERVO_CONTROL, OUTPUT);
	pinMode(PIN_SPEAKER, OUTPUT);

	Serial.println("LedController...");
	context.ledController = new LedController(PIN_LED_R, PIN_LED_G, PIN_LED_B);
	Serial.println("DoorController...");
	context.doorController = new DoorController(PIN_SERVO_CONTROL);
	Serial.println("TriggerController...");
	context.triggerController = new TriggerController(PIN_BUTTON_INPUT);
	Serial.println("FrequencyRegulator...");
	context.frequencyRegulator = new FrequencyController(TARGET_FREQ);
	digitalWrite(PIN_BUTTON_HIGH, HIGH);
	Serial.println("WelcomeSequence...");
	playWelcomeSequence();
	//Serial.println("Closing Door...");
	//context.doorController->closeDoor();
	Serial.println("Ready!");
}

void loop() {
	uint16_t lastTickMillis = context.frequencyRegulator->lastTickMillis();
	context.ledController->tick(lastTickMillis);
	context.doorController->tick(lastTickMillis);
	context.triggerController->tick(lastTickMillis);


	if (context.triggerController->readDoorOpenTriggered()) {
		Serial.println("Door Trigger Read!");
		context.doorController->addOpenTime(SERVO_OPEN_MILLIS);
	}
	if (context.triggerController->readLightTurnedOnTriggered()) {
		Serial.println("Light turned on!");
	}
	if (context.triggerController->readTrollModeTriggered()) {
		Serial.println("Troll mode!");
	}

	if (context.doorController->isDoorOpen()) {
		context.ledController->setLedColor(255, 255, 255);
	} else {
		context.ledController->enableRainbow();
	}

	context.frequencyRegulator->waitForTick();
}

