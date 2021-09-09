#include "TriggerController.h"

TriggerController::TriggerController(uint8_t pinButton) {
	this->apds.begin();
	this->apds.enableProximity(true);
	//this->apds.enableColor(true);
	this->pinButton = pinButton;
}

TriggerController::~TriggerController() {
}

void TriggerController::tick(uint8_t elapsedMillis) {
	reduceCooldown(this->proximityNormalTrigger, elapsedMillis);
	reduceCooldown(this->proximityTrollTrigger, elapsedMillis);
	reduceCooldown(this->lightTrigger, elapsedMillis);
	reduceCooldown(this->buttonTrigger, elapsedMillis);

	uint16_t prevLevel = this->averageLightLevel;
	this->apds.getColorData(&this->color.r, &this->color.g, &this->color.b, &this->color.v);
	adjustLightAverage(this->color.v);

	if (this->averageLightLevel >= this->lightLevelTreshold
			&& prevLevel < this->lightLevelTreshold
			&& this->lightTrigger.cooldownMillis == 0) {
		this->lightTrigger.isTriggered = true;
		this->lightTrigger.cooldownMillis = 10ul * 1000;
	}

	uint8_t proximity = apds.readProximity();
	if (proximity > 5 && this->proximityNormalTrigger.cooldownMillis == 0) {
		Serial.println("Proximity Normal Triggered");
		this->proximityNormalTrigger.isTriggered = true;
		this->proximityNormalTrigger.cooldownMillis = 3000;
	}
	if (proximity > 50 && this->proximityTrollTrigger.cooldownMillis == 0) {
		Serial.println("Proximity Troll Triggered");
		this->proximityTrollTrigger.isTriggered = true;
		this->proximityTrollTrigger.cooldownMillis = 10000;
	}
	if (HIGH == digitalRead(pinButton) && buttonTrigger.cooldownMillis == 0) {
		Serial.println("Button Pushed");
		this->buttonTrigger.isTriggered = true;
		this->buttonTrigger.cooldownMillis = 333;
	}
}

bool TriggerController::readDoorOpenTriggered() {
	bool isTriggeredProximity = this->proximityNormalTrigger.isTriggered;
	this->proximityNormalTrigger.isTriggered = false;
	bool isTriggeredButton = this->buttonTrigger.isTriggered;
		this->buttonTrigger.isTriggered = false;
	return isTriggeredProximity || isTriggeredButton;
}

bool TriggerController::readLightTurnedOnTriggered() {
	bool isTriggered = this->lightTrigger.isTriggered;
	this->lightTrigger.isTriggered = false;
	return isTriggered;
}

bool TriggerController::readTrollModeTriggered() {
	bool isTriggered = this->proximityTrollTrigger.isTriggered;
	this->proximityTrollTrigger.isTriggered = false;
	return isTriggered;
}

void TriggerController::adjustLightAverage(const uint16_t& value) {
	this->averageLightLevel =
			((uint32_t)this->averageLightLevel * (RUNNING_AVERAGE_TICKS - 1) + value) / RUNNING_AVERAGE_TICKS;
}

void TriggerController::reduceCooldown(TriggerData& triggerData, const uint16_t& value) {
	if (triggerData.cooldownMillis == 0) {
		return;
	}
	if (triggerData.cooldownMillis <= value) {
		triggerData.cooldownMillis = 0;
	} else {
		triggerData.cooldownMillis -= value;
	}
}
