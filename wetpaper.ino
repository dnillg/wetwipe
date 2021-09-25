#include "Arduino.h"
#include "Servo.h"
#include <Wire.h>
#include <Adafruit_APDS9960.h>
#include <FastLED.h>

#include "FrequencyController.h"
#include "StripeController.h"
#include "DoorController.h"
#include "TriggerController.h"
#include "Config.h"


typedef struct {
	Servo* servo;
	FrequencyController* frequencyController;
	StripeController* stripeController;
	DoorController* doorController;
	TriggerController* triggerController;
	CRGBArray<NUM_LEDS> leds;
} Context;

Context context;

void playWelcomeSequence() {
	context.stripeController->addGreetingModeMillis(1000);
	delay(500);
	context.doorController->setDoorPosition(0);
	delay(500);
	context.doorController->setDoorPosition(60);
	delay(500);
	context.doorController->setDoorPosition(40);
	delay(100);
	context.doorController->setDoorPosition(60);
	delay(100);
	context.doorController->closeDoor();
	delay(1000);
	context.stripeController->enableRainbow();
}

void setup() {
	FastLED.addLeds<APA102, PIN_STRIPE_DATA, PIN_STRIPE_CLOCK, RGB>(context.leds, NUM_LEDS);
	Serial.begin(SERIAL_BAUD);
	while (!Serial) {
	}
	Wire.begin();

	pinMode(PIN_SERVO_CONTROL, OUTPUT);
	pinMode(PIN_SPEAKER, OUTPUT);

	Serial.println("StripeController...");
	context.stripeController = new StripeController(context.leds, NUM_LEDS);
	Serial.println("DoorController...");
	context.doorController = new DoorController(PIN_SERVO_CONTROL);
	Serial.println("TriggerController...");
	context.triggerController = new TriggerController();
	Serial.println("FrequencyRegulator...");
	context.frequencyController = new FrequencyController(TARGET_FREQ);
	Serial.println("WelcomeSequence...");
	playWelcomeSequence();
	Serial.println("Ready!");
}

void loop() {
	uint16_t lastTickMillis = context.frequencyController->lastTickMillis();
	context.stripeController->tick(lastTickMillis);
	context.doorController->tick(lastTickMillis);
	context.triggerController->tick(lastTickMillis);

	if (context.triggerController->readDoorOpenTriggered()) {
		Serial.println("Door Trigger Read!");
		context.doorController->addOpenTime(SERVO_OPEN_MILLIS);
	}
	if (context.triggerController->readLightTurnedOnTriggered()) {
		Serial.println("Light turned on!");
		context.stripeController->addGreetingModeMillis(5000);
	}
	if (context.triggerController->readTrollModeTriggered()) {
		Serial.println("Troll mode!");
		// Not implemented
	}

	if (context.doorController->isDoorOpen()) {
		context.stripeController->setLedColor(255, 255, 255);
	} else {
		context.stripeController->enableRainbow();
	}

	context.frequencyController->waitForTick();
}

