// ws2811.h

#ifndef _WS2811_h
#define _WS2811_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif
#include <FastLED.h>


#define DATA_PIN    3
#define LED_TYPE    WS2811
#define COLOR_ORDER BRG // Single ws2811 lights are rgb, strips are brg
//#define NUM_LEDS    144 // 50 LEDS PER STRIP // 11 LEDS PER PLANK IN BENCH
#define NUM_LEDS 100
#define NUM_LEDS_PER_ROW 50
#define NUM_LED_ROWS 2

#define BRIGHTNESS         255
#define FRAMES_PER_SECOND  600


#define OFFSET_BACK 72



class LEDControl {
public:
	LEDControl(CRGB* leds): leds(leds), pausePattern(RAINBOW), discoPattern(DISCO_CHANGE), color({0, 255, 255}) {};

	void show_pause_pattern();

	void show_disco_pattern(bool beat);

	void switch_disco_pattern();

	void set_color(CHSV color);
	void set_color(uint8_t gHue, uint8_t gSat, uint8_t gVal);
	void set_color_val(uint8_t val);
	void increment_color_hue();
	void increment_color_hue(uint8_t step);
	void fade_color();
	uint8_t get_color_hue();
	const CHSV& get_color();

	 // Set color functions
	void set_strip(uint8_t strip_number);
	void set_each_strip_with_color_diff(uint16_t delay_time, uint8_t hue, uint8_t hue_diff);
	void set_tile(uint8_t tile_number);
	void set_column_seat(uint8_t column_number);
	void set_column_back(uint8_t column_number);
	void set_pixel(uint8_t row, uint8_t column);
	void set_pixel(uint16_t pos);
	void set_rainbow_standing();
	void set_rainbow_laying();

	// Fill color functions
	void fill_num_leds_solid(uint16_t numToFill);
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

	void four_tile_show(uint16_t delay_time);

private:
	CRGB* leds;	

	int num_leds_col;
	int num_leds_row;

	CHSV color;

	enum {RAINBOW, SNAKE, STROLE} pausePattern;
	enum {DISCO_CHANGE, DISCO_PULSE, DISCO_SNAKE, DISCO_UPWARDS} discoPattern;
};






#endif

