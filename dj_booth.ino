/*
 Name:		dj_booth.ino
 Created:	8/29/2019
 Author:	aashildl
*/

// the setup function runs once when you press reset or power the board
#include <Arduino.h>
#include "LEDControl.h"
#include <FastLED.h>
#include "MSGEQ7.h"
#include "beat.h"
#include "disco.h"

FASTLED_USING_NAMESPACE
#if defined(FASTLED_VERSION) && (FASTLED_VERSION < 3001000)
	#warning "Requires FastLED 3.1 or later; check github for latest code."
#endif


#define LED_DATA_PIN 3
#define CHIPSET     WS2811

#define MUSIC_ON_LIMIT 500 // show pause pattern if more than 5 seconds between beats

#define LED_DITHER  255  // try 0 to disable flickering
#define CORRECTION  TypicalLEDStrip

#define pinAnalogLeft A0
#define pinAnalogRight A0
#define pinReset 6
#define pinStrobe 4
#define MSGEQ7_INTERVAL ReadsPerSecond(1000)
#define MSGEQ7_SMOOTH false

CMSGEQ7<MSGEQ7_SMOOTH, pinReset, pinStrobe, pinAnalogLeft, pinAnalogRight> MSGEQ7;
 
CRGB leds[NUM_LEDS];
LEDControl ledControl(leds);


// Four buttons for controlling LEDS
uint8_t button_fade_pin; // Button 1: Fade
uint8_t button_strobe_pin;// Button 2: Strobe
uint8_t button_toggle_pin; // Button 3: Toggle
uint8_t button_surprise_pin; // Button 4: Surprise


bool last_sensor_value;
bool current_sensor_value;
bool sensor_must_turn_high_again = false;

void setup() {
	// 3 second delay for recovery
	delay(3000); 
	Serial.begin(115200);

    pinMode(button_fade_pin, INPUT); // pullup?
    pinMode(button_strobe_pin, INPUT); // pullup?
    pinMode(button_toggle_pin, INPUT); // pullup?
    pinMode(button_surprise_pin, INPUT); // pullup?

	// FastLED setup
	FastLED.addLeds<CHIPSET, LED_DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(CORRECTION);
	FastLED.setBrightness(BRIGHTNESS);
	FastLED.setDither(LED_DITHER);
	FastLED.show(); // needed to reset leds to zero

	MSGEQ7.begin();
	Serial.println("Start of DJ code");

	ledControl.set_color(50, 255, 255);
	ledControl.fill_all_leds_solid();

	FastLED.show();
	delay(1000);
	ledControl.set_color(100, 255, 100);
	ledControl.fill_all_leds_solid();
	FastLED.show();
	delay(2000);
	ledControl.set_color(200, 255, 500);
	ledControl.set_row(1);
	FastLED.show();
	
}


bool beat = false;
uint8_t ledsAreShown = 0;
bool buttonLowBlinkPressed = false;
bool buttonFullBlinkPressed = false;
bool buttonSnakePressed = false;
uint8_t noBeatCount = 0;
bool pauseMode = false;

void loop()
{
	// Analyze without delay
	if (MSGEQ7.read(MSGEQ7_INTERVAL))
	{
		// Led strip output
		//filterValue = MSGEQ7.get(MSGEQ7_BASS);
		if (!beat) beat = isBeat(MSGEQ7.get(MSGEQ7_BASS));
		//beat = isBeat(filterValue);
		// Was there a beat since last time we updated the leds?
	
	}

	// Change color every 20ms
	//EVERY_N_MILLIS(20) 
	//{ 
	//	if (!buttonLowBlinkPressed && !buttonFullBlinkPressed && !buttonSnakePressed)
	//	{
	//		ledControl.increment_color_hue();
	//	}

	//}

	EVERY_N_MILLIS(20)
	{
		
		if (beat) 
		{
			noBeatCount = 0;
			pauseMode = false;
		}
		else
		{
			if (!pauseMode)
			{
				noBeatCount++;
				pauseMode = noBeatCount > 200; // 6 seconds since last beat
			}
		}
		if (pauseMode)
		{
			ledControl.show_pause_pattern();
		}
		else if (buttonLowBlinkPressed)
		{
			ledControl.show_user_low_pattern(beat);
			//ledControl.show_user_snake_pattern();
		} 
		else if (buttonFullBlinkPressed)
		{
			ledControl.show_user_full_pattern(beat);
		}
		else if (buttonSnakePressed)
		{
			ledControl.show_user_snake_pattern();
		}
		else {
			ledControl.show_disco_pattern(beat);
		}
		FastLED.show();
		ledControl.set_ledsAreShown();
		beat = false;
	}


	EVERY_N_SECONDS(5)
	{
		if (!buttonLowBlinkPressed && !buttonFullBlinkPressed && !buttonSnakePressed && !pauseMode)
		{
			uint8_t state = ledControl.switch_disco_pattern();
			Serial.println(state);
		}
		if (pauseMode)
		{
			uint8_t state = ledControl.switch_pasue_pattern();
			Serial.println(state);
		}
		
	}
	

	// Call the current pattern function once, updating the 'leds' array
	//fill_sideways();
	//gPatterns[gCurrentPatternNumber]();


	// send the 'leds' array out to the actual LED strip
	//FastLED.show();
	// insert a delay to keep the framerate modest
	//FastLED.delay(1000 / FRAMES_PER_SECOND);


	//// do some periodic updates
	//if (!sensor_changed_to_low)
	//{
	//EVERY_N_MILLISECONDS(20) { gHue++; } // slowly cycle the "base color" through the rainbow
	

	// EVERY_N_SECONDS(10) { nextPattern(); } // change patterns periodically
	//EVERY_N_BSECONDS(1) 
	//{
	//	uint8_t _dummy;
	//}
}

// #define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

// void nextPattern()
// {
// 	// add one to the current pattern number, and wrap around at the end
// 	gCurrentPatternNumber = (gCurrentPatternNumber + 1) % ARRAY_SIZE(gPatterns);
// }

// void rainbow_standing()
// {
// 	set_rainbow_standing(leds, CHSV(gHue, 255, 255));
// }

// void rainbow_laying()
// {
// 	set_rainbow_laying(leds, CHSV(gHue, 255, 255));
// }

// void sinelon()
// {
// 	// a colored dot sweeping back and forth, with fading trails
// 	fadeToBlackBy(leds, NUM_LEDS, 20);
// 	int pos = beatsin16(13, 0, NUM_LEDS - 1);
// 	leds[pos] += CHSV(gHue, 255, 192);
// }

// void strole_up_and_down()
// {
// 	strole_strip_upwards(leds, 50, CHSV(gHue, 255, 255));
// 	delay(1000);
// 	strole_strip_downwards(leds, 50, CHSV(gHue, 255, 255));
// 	delay(1000);
// 	gHue += 100;
// }

// void fill_sideways()
// {
// 	fill_from_left(leds, 50, CHSV(gHue, 255, 255));
// 	delay(4000);
// 	gHue += 100;
// 	fill_from_right(leds, 50, CHSV(gHue, 255, 255));
// 	delay(4000);
	
// }

// void four_tiles_blink_inverted()
// {
// 	four_tile_show(leds, 300, CHSV(gHue, 255, 255));
// }