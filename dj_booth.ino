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


	// Define which box is in use
//#define BOX_ID 0
//#define N_NODES 3

#define LED_DATA_PIN 3
//#define COLOR_ORDER BRG
#define CHIPSET     WS2811

//#define NUM_LEDS    100
//#define NUM_LEDS_PER_ROW 50
//#define NUM_LED_ROWS NUM_LEDS/NUM_LEDS_PER_ROW
//#define NUM_LED_ROWS 2


//#define BRIGHTNESS  255  // reduce power consumption

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
	ledControl.fill_num_leds_solid(NUM_LEDS);

	FastLED.show();
	delay(1000);
	ledControl.set_color(100, 255, 100);
	ledControl.fill_num_leds_solid(NUM_LEDS);
	FastLED.show();
	delay(2000);
	ledControl.set_color(200, 255, 500);
	ledControl.fill_num_leds_solid(NUM_LEDS/2);
	FastLED.show();
	
}


// List of patterns to cycle through.  Each is defined as a separate function below.
//typedef void(*SimplePatternList[])();
// SimplePatternList gPatterns = { rainbow_standing, rainbow_standing, rainbow_standing, sinelon, rainbow_laying, rainbow_laying, rainbow_laying, strole_up_and_down, fill_sideways, rainbow_standing, rainbow_standing, strole_up_and_down };

uint8_t gCurrentPatternNumber = 0; // Index number of which pattern is current
//uint8_t gHue = 0; // rotating "base color" used by many of the patterns


uint8_t last_sensor = 0;


uint16_t filterValue;
//CRGB color = CHSV(gHue, 255, 255);
uint8_t musicMode = DISCO_CHANGE;
uint8_t beat = 0;
uint16_t focusPosition = 0;
uint8_t focusRow = 0;
uint8_t ledsAreShown = 0;
bool buttonLowBlinkPressed = false;
bool buttonFullBlinkPressed = false;
bool buttonSnakePressed = false;
unsigned long lastBeat;
void loop()
{
	// Analyze without delay
	if (MSGEQ7.read(MSGEQ7_INTERVAL))
	{
		// Led strip output
		filterValue = MSGEQ7.get(MSGEQ7_BASS);
		beat = isBeat(filterValue);
		lastBeat = millis();
	
	}

	// Change color every 20ms
	EVERY_N_MILLIS(20) 
	{ 
		if (!buttonLowBlinkPressed && !buttonFullBlinkPressed && !buttonSnakePressed)
		{
			ledControl.increment_color_hue();
		}

	}
	//EVERY_N_SECONDS(10)
	//{
		//buttonLowBlinkPressed = !buttonLowBlinkPressed;
		//buttonFullBlinkPressed = !buttonFullBlinkPressed;
		//buttonSnakePressed= !buttonSnakePressed;
	//}

	EVERY_N_MILLIS(30)
	{
		FastLED.show();
		ledControl.set_ledsAreShown();
	}


	EVERY_N_SECONDS(5)
	{
		if (!buttonLowBlinkPressed && !buttonFullBlinkPressed && !buttonSnakePressed)
		{
			ledControl.switch_disco_pattern();
		}
		
	}
	//if (millis() - lastBeat < MUSIC_ON_LIMIT)
	//{
		if (buttonLowBlinkPressed)
		{
			ledControl.show_user_low_pattern(beat);
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
	/*}
	else
	{
		ledControl.show_pause_pattern();
	}*/
	

	
	
	

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