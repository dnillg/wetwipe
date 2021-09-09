#include "FrequencyController.h"

FrequencyController::FrequencyController(uint16_t targetFrequency = 100) {
	this->setFrequency(targetFrequency);
}

FrequencyController::~FrequencyController() {
}

void FrequencyController::waitForTick() {
	uint32_t currentElaped = getCurrentElapsed();

	if (currentElaped < this->microsPerTick) {
		uint32_t timeToWait = microsPerTick - currentElaped;
		if (timeToWait > 100) {
			delay(timeToWait/1000);
			delayMicroseconds(timeToWait % 1000);
		}
	}

	this->lastElapsedMicros = getCurrentElapsed();
	this->lastMicros = micros();
}

uint32_t FrequencyController::lastTickMicros() {
	return this->lastElapsedMicros;
}

uint16_t FrequencyController::lastTickMillis() {
	return (lastElapsedMicros + 500) / 1000;
}

void FrequencyController::setFrequency(uint16_t targetFrequency) {
	this->targetFrequency = targetFrequency;
	this->microsPerTick = 1000.0 * 1000.0 / targetFrequency;
}

uint32_t FrequencyController::getCurrentElapsed() {
	unsigned long currentMicros = micros();
	if (currentMicros < this->lastMicros) {
		return (((double)currentMicros) + 0xFFFFFFFF - this->lastMicros);
	} else {
		return currentMicros - this->lastMicros;
	}
}
