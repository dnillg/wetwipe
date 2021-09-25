#include "StripeController.h"

StripeController::StripeController(CRGB* leds, uint8_t numLeds) {
	this->hsvState.h = 0;
	this->hsvState.s = 1;
	this->hsvState.v = 1;

	this->ledCount = numLeds;

	this->leds = leds;
	for (int i = 0; i < numLeds; i++) {
		this->leds[i] = CRGB(255, 0, 0);
	}
}

void StripeController::tick(uint8_t elapsedMillis) {
	if (elapsedMillis == 0) {
		return;
	}

	this->hsvState.h = this->hsvState.h + elapsedMillis * this->hueIncrementPerMillis;
	if (this->hsvState.h > 1) {
		this->hsvState.h -= 1;
	}

	if (greetingModeMillis > 0) {
		if (greetingModeMillis <= elapsedMillis) {
			greetingModeMillis = 0;
		} else {
			greetingModeMillis -= elapsedMillis;
		}
		uint16_t colCode = ((greetingModeMillis/100) % 9);
		this->rgbConverter.HsvToRgb(0.1 * colCode, 1, 1, this->currentColor.r, this->currentColor.g, this->currentColor.b);
		writeColor();
	} else if (this->rainbowEnabled) {
		this->rgbConverter.HsvToRgb(
				this->hsvState.h, this->hsvState.s, this->hsvState.v,
				this->currentColor.r, this->currentColor.g, this->currentColor.b);
		writeColor();
	}
}

void StripeController::setLedColor(uint8_t r, uint8_t g, uint8_t b) {
	this->rainbowEnabled = false;
	this->currentColor.r = r;
	this->currentColor.g = g;
	this->currentColor.b = b;
	writeColor();
}

void StripeController::writeColor() {
	for (int i = 0; i < this->ledCount; i++) {
			this->leds[i] = CRGB(currentColor.r, currentColor.g, currentColor.b);
		}
	FastLED.show();
}

void StripeController::setHueIncrementPerMillis(double value) {
	this->hueIncrementPerMillis = value;
}

void StripeController::enableRainbow() {
	this->rainbowEnabled = true;
}

void StripeController::addGreetingModeMillis(uint32_t millis) {
	this->greetingModeMillis += millis;
}


StripeController::~StripeController() {
}

