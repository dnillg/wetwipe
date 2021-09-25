#include "TriggerController.h"

TriggerController::TriggerController() {
	//this->apds.begin();
	//this->apds.enableProximity(true);
	//this->apds.enableColor(true);
	this->vcnl.begin();
	this->lightTrigger.cooldownMillis = 10000;
}

TriggerController::~TriggerController() {
}

void TriggerController::tick(uint8_t elapsedMillis) {
	reduceCooldown(this->proximityNormalTrigger, elapsedMillis);
	reduceCooldown(this->proximityTrollTrigger, elapsedMillis);
	reduceCooldown(this->lightTrigger, elapsedMillis);

	uint16_t prevLevel = this->averageLightLevel;
	//this->apds.getColorData(&this->color.r, &this->color.g, &this->color.b, &this->color.v);
	this->color.v = this->vcnl.readAmbient();
	adjustLightAverage(this->color.v);

	if (this->averageLightLevel >= this->lightLevelTreshold
			&& prevLevel < this->lightLevelTreshold
			&& this->lightTrigger.cooldownMillis == 0) {
		this->lightTrigger.isTriggered = true;
		this->lightTrigger.cooldownMillis = 2ul * 1000;
	}

	//uint16_t proximity = apds.readProximity();
	uint16_t proximity = vcnl.readProximity();
	//Serial.println(proximity);
	if (proximity > PROXIMITY_NORMAL && this->proximityNormalTrigger.cooldownMillis == 0) {
		Serial.println("Proximity Normal Triggered");
		this->proximityNormalTrigger.isTriggered = true;
		this->proximityNormalTrigger.cooldownMillis = 2500;
	}
	if (proximity > PROXIMITY_TROLL && this->proximityTrollTrigger.cooldownMillis == 0) {
		Serial.println("Proximity Troll Triggered");
		this->proximityTrollTrigger.isTriggered = true;
		this->proximityTrollTrigger.cooldownMillis = 10000;
	}
}

bool TriggerController::readDoorOpenTriggered() {
	bool isTriggeredProximity = this->proximityNormalTrigger.isTriggered;
	this->proximityNormalTrigger.isTriggered = false;
	return isTriggeredProximity;
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
			((uint32_t)this->averageLightLevel * (RUNNING_AVERAGE_TICKS - 1.0) + value) / RUNNING_AVERAGE_TICKS;
	Serial.println(averageLightLevel);
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
