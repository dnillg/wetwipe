#include "LedController.h"

LedController::LedController(uint8_t pinR, uint8_t pinG, uint8_t pinB) {
	this->hsvState.h = 0;
	this->hsvState.s = 1;
	this->hsvState.v = 1;
	this->pinR = pinR;
	this->pinG = pinG;
	this->pinB = pinB;
}

LedController::~LedController() {
}

void LedController::tick(uint8_t elapsedMillis) {
	if (elapsedMillis == 0) {
		return;
	}
	this->hsvState.h = this->hsvState.h + elapsedMillis * this->hueIncrementPerMillis;
	if (this->hsvState.h > 1) {
		this->hsvState.h -= 1;
	}
	if (this->rainbowEnabled) {
		this->rgbConverter.HsvToRgb(
				this->hsvState.h, this->hsvState.s, this->hsvState.v,
				this->currentColor.r, this->currentColor.g, this->currentColor.b);
		writeColor();
	}
}

void LedController::setLedColor(uint8_t r, uint8_t g, uint8_t b) {
	this->rainbowEnabled = false;
	this->currentColor.r = r;
	this->currentColor.g = g;
	this->currentColor.b = b;
	writeColor();
}

void LedController::writeColor() {
	analogWrite(pinR, currentColor.r);
	analogWrite(pinG, currentColor.g);
	analogWrite(pinB, currentColor.b);
}

void LedController::setHueIncrementPerMillis(double value) {
	this->hueIncrementPerMillis = value;
}

void LedController::enableRainbow() {
	this->rainbowEnabled = true;
}
