#ifndef STRIPECONTROLLER_H_
#define STRIPECONTROLLER_H_

#include <Arduino.h>
#include "lib/ColorConverterLib.h"
#include <FastLED.h>

class StripeController {
public:
	StripeController(CRGB* leds, uint8_t numLeds);
	virtual ~StripeController();
	void tick(uint8_t elapsedMillis);
	void setLedColor(uint8_t r, uint8_t g, uint8_t b);
	void setHueIncrementPerMillis(double value);
	void enableRainbow();
	void addGreetingModeMillis(uint32_t millis);
private:
	struct HsvColor {
		float h = 0;
		float s = 0;
		float v = 0;
	};
	struct RgbColor {
		unsigned char r = 0;
		unsigned char g = 0;
		unsigned char b = 0;
	};
	CRGB* leds;
	uint8_t ledCount;
	RGBConverter rgbConverter;
	double hueIncrementPerMillis = 0.00002;
	HsvColor hsvState;
	RgbColor currentColor;
	bool rainbowEnabled = true;
	void writeColor();
	uint32_t greetingModeMillis = 0;
};

#endif /* STRIPECONTROLLER_H_ */
