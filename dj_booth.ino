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

FASTLED_USING_NAMESPACE
#if defined(FASTLED_VERSION) && (FASTLED_VERSION < 3001000)
	#warning "Requires FastLED 3.1 or later; check github for latest code."
#endif


#define LED_DATA_PIN 3
#define CHIPSET     WS2811

#define MUSIC_ON_LIMIT 200 // show pause pattern if more than 6 seconds between beats

#define LED_DITHER  255  // try 0 to disable flickering
#define CORRECTION  TypicalLEDStrip

#define pinAnalogLeft A0
#define pinAnalogRight A0
#define pinReset 6
#define pinStrobe 4
#define MSGEQ7_INTERVAL ReadsPerSecond(1000)
#define MSGEQ7_SMOOTH false

#define NUM_BUTTONS 3
#define LOW_BUTTON 1
#define UKA_BUTTON 0
#define SNAKE_BUTTON 2

CMSGEQ7<MSGEQ7_SMOOTH, pinReset, pinStrobe, pinAnalogLeft, pinAnalogRight> MSGEQ7;
 
CRGB leds[NUM_LEDS];
LEDControl ledControl(leds);

#define BUTTON_RED_PIN 12
#define BUTTON_GREEN_PIN 11
#define BUTTON_BLUE_PIN 10

uint8_t buttonPins[NUM_BUTTONS] {BUTTON_RED_PIN, BUTTON_GREEN_PIN, BUTTON_BLUE_PIN};
uint8_t buttonPressed[NUM_BUTTONS] {0, 0, 0};

uint8_t beatOutputPin = 8;

uint8_t checkButton(uint8_t buttonPin)
{
	// button is active low
	return !digitalRead(buttonPin);
}
uint8_t anyButtonPressed()
{
	for (int button = 0; button < NUM_BUTTONS; button++)
	{
		if (buttonPressed[button]) 
		{
			return 1;
		}
	}
	return 0;
}



void setup() {
	// 3 second delay for recovery
	delay(3000); 
	Serial.begin(115200);

	for (uint8_t button = 0; button < NUM_BUTTONS; button++)
	{
		pinMode(buttonPins[button], INPUT_PULLUP);
	}
	pinMode(13, OUTPUT);
	pinMode(beatOutputPin, OUTPUT); 

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
uint8_t noBeatCount = 0;
bool pauseMode = false;


void loop()
{
	// Analyze without delay
	if (MSGEQ7.read(MSGEQ7_INTERVAL))
	{
		// Led strip output
		if (!beat) beat = isBeat(MSGEQ7.get(MSGEQ7_BASS));
	}

	EVERY_N_MILLIS(20)
	{
		// button debounce check
		for (int button = 0; button < NUM_BUTTONS; button++)
		{
			buttonPressed[button] = checkButton(buttonPins[button]);
		}
		
		if (beat) 
		{
			digitalWrite(beatOutputPin, HIGH);
			noBeatCount = 0;
			pauseMode = false;
		}
		else
		{
			if (!pauseMode)
			{
				noBeatCount++;
				pauseMode = noBeatCount > MUSIC_ON_LIMIT; // 6 seconds since last beat
			}
		}
		
		if (buttonPressed[LOW_BUTTON])
		{
			ledControl.show_user_low_pattern(beat);
		} 
		else if (buttonPressed[UKA_BUTTON])
		{
			ledControl.show_user_full_pattern(beat);
		}
		else if (buttonPressed[SNAKE_BUTTON])
		{
			ledControl.show_user_snake_pattern();
		}
		else if (pauseMode)
		{
			ledControl.show_pause_pattern();
		}
		else 
		{
			ledControl.show_disco_pattern(beat);
		}
		FastLED.show();
		ledControl.set_ledsAreShown();
		beat = false;
		digitalWrite(beatOutputPin, LOW);
	}


	EVERY_N_SECONDS(10)
	{
		if (!anyButtonPressed() && !pauseMode)
		{
			uint8_t state = ledControl.switch_disco_pattern();
			//Serial.println(state);
		}
	}
	EVERY_N_SECONDS(20)
	{
		if (pauseMode)
		{
			uint8_t state = ledControl.switch_pause_pattern();
			//Serial.println(state);
		}
	}
}