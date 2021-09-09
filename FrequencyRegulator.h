#ifndef FREQUENCYREGULATOR_H_
#define FREQUENCYREGULATOR_H_

#include "Arduino.h"
#include <Math.h>

class FrequencyRegulator {
private:
	uint8_t frq;
	uint8_t millisPerTick;
	unsigned long lastMillis = 0;

	uint8_t lastEffectiveFrq = 0;
	uint8_t currentEffectiveFrq = 0;
	unsigned long lastEffectiveCalcMillis = 0;
public:
	FrequencyRegulator(uint8_t frq);
	void waitTick();
	uint8_t getEffectiveFrequency();
	void addTimedJob(uint8_t periodMillis, void (*userFunc)(void));
private:
	void calculateEffectiveFrq(unsigned long nextTickMillis);
	void setFrequency(uint8_t frq);
};

#endif
