#ifndef TRIGGERCONTROLLER_H_
#define TRIGGERCONTROLLER_H_

#include "Config.h"

#include <Arduino.h>
#ifdef SENSOR_ADPS
#include <Adafruit_APDS9960.h>
#endif
#ifdef SENSOR_VCNL
#include "Adafruit_VCNL4010.h"
#endif
#include <Wire.h>

#define RUNNING_AVERAGE_TICKS 10

#define PROXIMITY_TROLL 20000
#define PROXIMITY_NORMAL 2800

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
	ColorData color;
#ifdef SENSOR_ADPS
	Adafruit_APDS9960 apds;
#endif
#ifdef SENSOR_VCNL
	Adafruit_VCNL4010 vcnl;
#endif
	uint32_t averageLightLevel = 35;
	uint16_t lightLevelTreshold = 35;
	TriggerData lightTrigger;
	TriggerData proximityNormalTrigger;
	TriggerData proximityTrollTrigger;
	void reduceCooldown(TriggerData& triggerData, const uint16_t& value);
public:
	TriggerController();
	virtual ~TriggerController();
	void tick(uint8_t elapsedMillis);
	bool readDoorOpenTriggered();
	bool readLightTurnedOnTriggered();
	bool readTrollModeTriggered();
	void adjustLightAverage(const uint16_t& value);
};

#endif /* TRIGGERCONTROLLER_H_ */
