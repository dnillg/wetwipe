#ifndef DOORCONTROLLER_H_
#define DOORCONTROLLER_H_

#include <Arduino.h>
#include <Servo.h>

#define DOOR_SERVO_POS_CLOSED 10
#define DOOR_SERVO_POS_OPEN 174
#define DOOR_BOUNCE_TIME_OFFSET_MILLIS 1000

class DoorController {
private:
	uint16_t doorOpenMillis = 0;
	uint16_t doorBounceMillis = 0;
	uint8_t servoPosition = 0;
	uint8_t servoPin;
	Servo servo;
	bool servoDirection = false; //true: increasing, false: decreasing
	void bounce();
public:
	DoorController(uint8_t servoPin);
	void tick(uint8_t elapsedMillis);
	void addOpenTime(uint16_t durationMillis);
	void openDoor();
	void closeDoor();
	bool isDoorOpen();
	void setDoorPosition(uint8_t posPercentage);
	virtual ~DoorController();
};

#endif /* DOORCONTROLLER_H_ */
