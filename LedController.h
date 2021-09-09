#ifndef LEDCONTROLLER_H_
#define LEDCONTROLLER_H_

#include <Arduino.h>
#include "ColorConverterLib.h"

class LedController {
public:
	LedController(uint8_t pinR, uint8_t pinG, uint8_t pinB);
	virtual ~LedController();
	void tick(uint8_t elapsedMillis);
	void setLedColor(uint8_t r, uint8_t g, uint8_t b);
	void setHueIncrementPerMillis(double value);
	void enableRainbow();
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
	uint8_t pinR;
	uint8_t pinG;
	uint8_t pinB;
	RGBConverter rgbConverter;
	double hueIncrementPerMillis = 0.00005;
	HsvColor hsvState;
	RgbColor currentColor;
	bool rainbowEnabled = true;
	void writeColor();
};

#endif /* LEDCONTROLLER_H_ */
