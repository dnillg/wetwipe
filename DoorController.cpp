#include "DoorController.h"

DoorController::DoorController(uint8_t servoPin) {
	this->servoPin = servoPin;
	this->servo.attach(servoPin);
}

DoorController::~DoorController() {
}

void DoorController::tick(uint8_t elapsedMillis) {
	if (elapsedMillis == 0) {
		return;
	}
	if (elapsedMillis < this->doorOpenMillis) {
		this->doorOpenMillis -= elapsedMillis;
	} else if (this->doorOpenMillis != 0) {
		this->doorOpenMillis = 0;
		closeDoor();
	}

	if (elapsedMillis < this->doorBounceMillis) {
		this->doorBounceMillis -= elapsedMillis;
	} else if (this->doorBounceMillis != 0) {
		this->doorBounceMillis = 0;
		bounce();
	}
}

void DoorController::addOpenTime(uint16_t durationMillis) {
	openDoor();
	this->doorOpenMillis += durationMillis;
}

void DoorController::openDoor() {
	setDoorPosition(100);
	this->doorBounceMillis = DOOR_BOUNCE_TIME_OFFSET_MILLIS;
}

void DoorController::closeDoor() {
	setDoorPosition(0);
	this->doorBounceMillis = DOOR_BOUNCE_TIME_OFFSET_MILLIS;
}

void DoorController::setDoorPosition(uint8_t posPercentage) {
	this->doorBounceMillis = 0; //cancel scheduled bounce
	float newPosition = (DOOR_SERVO_POS_CLOSED) + (DOOR_SERVO_POS_OPEN - DOOR_SERVO_POS_CLOSED) * (uint16_t)posPercentage / 100.0f;
	uint8_t newPositionRounded = (uint8_t)(newPosition + 0.5f);
	this->servoDirection = newPositionRounded > this->servoPosition;
	this->servoPosition = newPositionRounded;
	this->servo.write(newPositionRounded);
}

void DoorController::bounce() {
	Serial.println("Bouncing door..");
	int8_t offset = this->servoDirection ? -2 : 2;
	this->servo.write(this->servoPosition + offset);
}

bool DoorController::isDoorOpen() {
	return this->doorOpenMillis != 0;
}
