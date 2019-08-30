/*
 Name:		dj_booth.ino
 Created:	8/29/2019
 Author:	aashildl
*/

// the setup function runs once when you press reset or power the board
#include "ws2811.h"
#include <FastLED.h>

FASTLED_USING_NAMESPACE
#if defined(FASTLED_VERSION) && (FASTLED_VERSION < 3001000)
	#warning "Requires FastLED 3.1 or later; check github for latest code."
#endif


	// Define which box is in use
//#define BOX_ID 0
//#define N_NODES 3


CRGB leds[NUM_LEDS];

// Four buttons for controlling LEDS
int button_fade_pin; // Button 1: Fade
int button_strobe_pin;// Button 2: Strobe
int button_toggle_pin; // Button 3: Toggle
int button_surprise_pin; // Button 4: Surprise


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

	// tell FastLED about the LED strip configuration
	FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);

	// set master brightness control
	FastLED.setBrightness(BRIGHTNESS);

	set_each_strip_in_bench_with_color_diff(leds, 200, 70, 30);
	
}

// the loop function runs over and over again until power down or reset

// List of patterns to cycle through.  Each is defined as a separate function below.
typedef void(*SimplePatternList[])();
SimplePatternList gPatterns = { rainbow_standing, rainbow_standing, rainbow_standing, sinelon, rainbow_laying, rainbow_laying, rainbow_laying, strole_up_and_down, fill_sideways, rainbow_standing, rainbow_standing, strole_up_and_down };

uint8_t gCurrentPatternNumber = 0; // Index number of which pattern is current
uint8_t gHue = 0; // rotating "base color" used by many of the patterns


uint8_t last_sensor = 0;
void loop()
{

	

	// Call the current pattern function once, updating the 'leds' array
	//fill_sideways();
	gPatterns[gCurrentPatternNumber]();


	// send the 'leds' array out to the actual LED strip
	FastLED.show();
	// insert a delay to keep the framerate modest
	FastLED.delay(1000 / FRAMES_PER_SECOND);


	//// do some periodic updates
	//if (!sensor_changed_to_low)
	//{
	EVERY_N_MILLISECONDS(20) { gHue++; } // slowly cycle the "base color" through the rainbow
	

	EVERY_N_SECONDS(10) { nextPattern(); } // change patterns periodically
	EVERY_N_BSECONDS(1) 
	{
		Serial.print(F("SENSOR: "));
		Serial.println(digitalRead(IR_SENSOR_1_PIN));
		if ((digitalRead(IR_SENSOR_1_PIN) == LOW) && (last_sensor == HIGH))
		{
			last_sensor =  LOW;
			for (uint8_t j = 0; j < 4; j++)
			{
				for (uint8_t i = 0; i < 12; i++)
				{
					set_strip(leds, i, CHSV(gHue, 255, 255));
				}
				delay(150);
				for (uint8_t i = 0; i < 12; i++)
				{
					set_strip(leds, i, CHSV(0, 0, 0));
				}
				delay(500);
			}
		}
	 }
	else
		last_sensor = digitalRead(IR_SENSOR_1_PIN);
}

#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

void nextPattern()
{
	// add one to the current pattern number, and wrap around at the end
	gCurrentPatternNumber = (gCurrentPatternNumber + 1) % ARRAY_SIZE(gPatterns);
}

void rainbow_standing()
{
	set_rainbow_standing(leds, CHSV(gHue, 255, 255));
}

void rainbow_laying()
{
	set_rainbow_laying(leds, CHSV(gHue, 255, 255));
}

void sinelon()
{
	// a colored dot sweeping back and forth, with fading trails
	fadeToBlackBy(leds, NUM_LEDS, 20);
	int pos = beatsin16(13, 0, NUM_LEDS - 1);
	leds[pos] += CHSV(gHue, 255, 192);
}

void strole_up_and_down()
{
	strole_strip_upwards(leds, 50, CHSV(gHue, 255, 255));
	delay(1000);
	strole_strip_downwards(leds, 50, CHSV(gHue, 255, 255));
	delay(1000);
	gHue += 100;
}

void fill_sideways()
{
	fill_from_left(leds, 50, CHSV(gHue, 255, 255));
	delay(4000);
	gHue += 100;
	fill_from_right(leds, 50, CHSV(gHue, 255, 255));
	delay(4000);
	
}

void four_tiles_blink_inverted()
{
	four_tile_show(leds, 300, CHSV(gHue, 255, 255));
}