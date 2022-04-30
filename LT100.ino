#include <CorsairLightingProtocol.h>
#include <FastLED.h>

#define DATA_PIN_CHANNEL_1 2

CRGB ledsChannel1[37];

CorsairLightingFirmware firmware = corsairLT100Firmware();
FastLEDController ledController(true);
CorsairLightingProtocolController cLP(&ledController, &firmware);
CorsairLightingProtocolHID cHID(&cLP);

void setup() {
	FastLED.addLeds<WS2812B, DATA_PIN_CHANNEL_1, GRB>(ledsChannel1, 37);
	ledController.addLEDs(0, ledsChannel1, 378);
}

void loop() {
	cHID.update();

	if (ledController.updateLEDs()) {
		FastLED.show();
	}
}
