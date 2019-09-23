// ws2811.h

#ifndef _LEDCONTROL_H
#define _LEDCONTROL_H

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif
#include <FastLED.h>
#include "UKA.h"


#define DATA_PIN    3
#define LED_TYPE    WS2811
#define COLOR_ORDER BRG // Single ws2811 lights are rgb, strips are brg
//#define NUM_LEDS    144 // 50 LEDS PER STRIP // 11 LEDS PER PLANK IN BENCH
#define NUM_LEDS 350
#define NUM_LEDS_PER_ROW 50
#define NUM_LED_ROWS 7

#define BRIGHTNESS         255
#define FRAMES_PER_SECOND  600

#define LEDS_VALUE_MAX 255
#define LEDS_SATURATION_MAX 255
#define LEDS_HUE_MAX 255


#define OFFSET_BACK 72

#define DISCO_FULL_OFFSET 0
#define DISCO_CHANGE 0
#define DISCO_PULSE 1
#define DISCO_TOGGLE 2
#define NUM_DISCO_FULL 3

#define DISCO_FILL_OFFSET 3
#define DISCO_UPWARDS 0
#define DISCO_DOWNWARDS 1
#define DISCO_UPWARDS_FILL 2
#define DISCO_DOWNWARDS_FILL 3
#define NUM_DISCO_FILL 4

#define DISCO_SNAKE_OFFSET 8
#define DISCO_SINGLE_SNAKE 0
#define DISCO_DOUBLE_SNAKE_ROWS 1
#define NUM_DISCO_SNAKE 2

#define PAUSE_RAINBOW_LAYING 0
#define PAUSE_RAINBOW_STANDING 1
#define PAUSE_FILL_SNAKE 2
#define PAUSE_DOUBLE_SNAKE 3
#define PAUSE_UKA_LETTERS 4
#define PAUSE_FILL_SNAKE_RAINBOW 5
#define PAUSE_SINGLE_ROW_SNAKE 6
#define PAUSE_FILL_SINGLE_ROW_SNAKE 7

#define NUM_PAUSE_PATTERNS 8


class LEDControl {
public:
	LEDControl(CRGB* leds): leds(leds), 
							pausePattern(PAUSE_RAINBOW_LAYING), 
							discoPattern(DISCO_CHANGE), 
							nextDiscoPattern(DISCO_CHANGE), 
							toggleBeat(true),
							fillSnakeWithColor(false), 
							focusPosition(0),
							focusRow(0),
							snakeRunning(false),
							color({0, 255, 255}) {};

	void show_pause_pattern();

	void show_disco_pattern(bool beat);

	void show_user_low_pattern(bool beat);
	void show_user_full_pattern(bool beat);
	void show_user_snake_pattern();

	uint8_t switch_disco_pattern();
	uint8_t switch_pasue_pattern();

	void set_color(CHSV color);
	void set_color(uint8_t gHue, uint8_t gSat, uint8_t gVal);
	void set_color_val(uint8_t val);
	void increment_color_hue();
	void increment_color_hue(uint8_t step);
	void fade_color();

	void set_ledsAreShown();

	 // Set color functions
	void set_row(uint8_t row);
	void set_row(uint8_t row, CHSV& color);
	void set_column(uint8_t column_number);
	void set_column(uint8_t column_number, CHSV& color);
	void set_pixel(uint8_t row, uint8_t column);
	void set_pixel(uint16_t pos);
	void set_rainbow_standing();
	void set_rainbow_laying();

	// Fill color functions
	void fill_all_leds_solid();
	void clear_all_leds();
	void fill_strip(uint8_t strip_number, uint16_t delay_time);
	void fill_tile(uint8_t tile_number, uint16_t delay_time);
	void fill_column_seat(uint8_t column_number, uint16_t delay_time);
	void fill_column_back(uint8_t column_number, uint16_t delay_time);
	void fill_column(uint8_t column_number, uint16_t delay_time);

	// Show functions
	void strole_strip_downwards(uint16_t delay_time);
	void strole_strip_upwards(uint16_t delay_time);
	void strole_column_downwards(uint16_t delay_time);
	void strole_column_upwards(uint16_t delay_time);
	void fill_strip_downwards(uint16_t delay_time);
	void fill_strip_upwards(uint16_t delay_time);
	void fill_from_left(uint16_t delay_time);
	void fill_from_right(uint16_t delay_time);
	void empty_strip_downwards(uint16_t delay_time);
	void empty_strip_upwards(uint16_t delay_time);


	void double_row_snake(CHSV& color);
	void single_row_snake(CHSV& color);
	void single_snake(CHSV& color);
	void uka_letters(uint8_t offset);

private:
	CRGB* leds;	

	CHSV color;

	uint8_t pausePattern;
	uint8_t discoPattern, nextDiscoPattern;
	uint8_t pseudoRandomNumber;

	bool ledsAreShown;
	bool toggleBeat;
	bool fillSnakeWithColor;
	uint16_t focusPosition = 0;
	uint8_t focusRow = 0;
	uint8_t focusColumn = 0;
	bool snakeRunning;
	uint8_t maxColorValue;
};






#endif

