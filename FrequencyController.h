#ifndef FREQUENCYCONTROLLER_H_
#define FREQUENCYCONTROLLER_H_

#include <Arduino.h>

class FrequencyController {
private:
	uint32_t lastMicros = micros();
	uint32_t lastElapsedMicros = 0;
	uint16_t targetFrequency = 100;
	uint32_t microsPerTick = 1000ul * 1000ul / 100ul;
public:
	FrequencyController(uint16_t targetFrequency);
	virtual ~FrequencyController();
	void waitForTick();
	void setFrequency(uint16_t targetFrequency);
	uint32_t lastTickMicros();
	uint16_t lastTickMillis();
	uint32_t getCurrentElapsed();
};

#endif /* FREQUENCYCONTROLLER_H_ */
