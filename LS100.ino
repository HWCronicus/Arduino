#include <CorsairLightingProtocol.h>
#include <FastLED.h>

// Hint: The channels of the LS100 are swapped in iCUE, so the first channel in iCUE is here channel 2
#define DATA_PIN_CHANNEL_1 3
#define DATA_PIN_CHANNEL_2 4

#define BUTTON_PIN 2

// Hint: The ATmega32U4 does not have enough memory for 135 leds on both channels
CRGB ledsChannel1[72];
CRGB ledsChannel2[136];

const char mySerialNumber[] PROGMEM = "692B6949A967";

CorsairLightingFirmware firmware = corsairLS100Firmware();
FastLEDController ledController(true);
CorsairLightingProtocolController cLP(&ledController, &firmware);
CorsairLightingProtocolHID cHID(&cLP, mySerialNumber);

void setup() {

  CLP::disableBuildInLEDs();
  if (CLP::shouldReset(&firmware)) {
    CLP::reset(&firmware);
    ledController.reset();
  }
  
	FastLED.addLeds<WS2812B, DATA_PIN_CHANNEL_1, GRB>(ledsChannel1, 72);
	FastLED.addLeds<WS2812B, DATA_PIN_CHANNEL_2, GRB>(ledsChannel2, 136);
	ledController.addLEDs(0, ledsChannel1, 72);
	ledController.addLEDs(1, ledsChannel2, 136);
  
  ledController.onUpdateHook(1, []() {
  CLP::fixIcueBrightness(&ledController, 1);
  CLP::gammaCorrection(&ledController, 1);
  CLP::SegmentScaling segments[6] = {{15, 20}, {27, 24}, {27, 24}, {15, 20}, {27, 24}, {27, 26}};
  CLP::scaleSegments(&ledController, 1, segments, 6);
});
	pinMode(BUTTON_PIN, INPUT_PULLUP);
}

void loop() {
	static bool lightingEnabled = true;
	cHID.update();

	if (buttonClicked()) {
		lightingEnabled = !lightingEnabled;
		fill_solid(ledsChannel1, 72, CRGB::Black);
		fill_solid(ledsChannel2, 136, CRGB::Black);
		FastLED.show();
	}

	if (lightingEnabled && ledController.updateLEDs()) {
		FastLED.show();
	}
}

/**
 * Handle button of the LS100. The button is optional.
 *
 * @return true if the button was pressed and then released.
 */
bool buttonClicked() {
	static bool previousState = 1;
	bool state = digitalRead(BUTTON_PIN);
	if (previousState == 0 && state == 1) {
		previousState = state;
		return true;
	}
	previousState = state;
	return false;
}
