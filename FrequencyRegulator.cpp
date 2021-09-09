#include "FrequencyRegulator.h"

FrequencyRegulator::FrequencyRegulator(uint8_t frq) {
	setFrequency(frq);
}

void FrequencyRegulator::waitTick() {
	unsigned long nextTickMillis = lastMillis + millisPerTick;
	calculateEffectiveFrq(nextTickMillis);
	lastMillis = millis();
	if(nextTickMillis > lastMillis) {
		delay(nextTickMillis - lastMillis);
		lastMillis = nextTickMillis;
	}
}

uint8_t FrequencyRegulator::getEffectiveFrequency() {
	return lastEffectiveFrq;
}

void FrequencyRegulator::setFrequency(uint8_t frq) {
	this->frq = frq;
	this->millisPerTick = round(1000.0/frq);
}

void FrequencyRegulator::calculateEffectiveFrq(unsigned long nextTickMillis) {
	if (currentEffectiveFrq == frq) {
		lastEffectiveFrq = ((10000ul * frq / (nextTickMillis - lastEffectiveCalcMillis)) + 5ul) / 10ul;
		currentEffectiveFrq = 0;
		lastEffectiveCalcMillis = nextTickMillis;
	}
	currentEffectiveFrq++;
}

