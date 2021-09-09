#ifndef TRIGGERCONTROLLER_H_
#define TRIGGERCONTROLLER_H_

#include <Arduino.h>
#include <Adafruit_APDS9960.h>

#define RUNNING_AVERAGE_TICKS 100

class TriggerController {
private:
	struct ColorData {
		uint16_t r;
		uint16_t g;
		uint16_t b;
		uint16_t v;
	};
	struct TriggerData {
		bool isTriggered = false;
		uint32_t cooldownMillis = 0;
	};
	uint8_t pinButton;
	ColorData color;
	Adafruit_APDS9960 apds;
	uint32_t averageLightLevel = 0;
	uint16_t lightLevelTreshold = 50;
	TriggerData lightTrigger;
	TriggerData buttonTrigger;
	TriggerData proximityNormalTrigger;
	TriggerData proximityTrollTrigger;
	void reduceCooldown(TriggerData& triggerData, const uint16_t& value);
public:
	TriggerController(uint8_t pinButton);
	virtual ~TriggerController();
	void tick(uint8_t elapsedMillis);
	bool readDoorOpenTriggered();
	bool readLightTurnedOnTriggered();
	bool readTrollModeTriggered();
	void adjustLightAverage(const uint16_t& value);
};

#endif /* TRIGGERCONTROLLER_H_ */
