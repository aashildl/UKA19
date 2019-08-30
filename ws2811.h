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
#define NUM_LEDS    144 // 50 LEDS PER STRIP // 11 LEDS PER PLANK IN BENCH

#define BRIGHTNESS         255
#define FRAMES_PER_SECOND  600


#define OFFSET_BACK 72

class LEDControl {
public:
	LEDControl(CRGB* leds): leds(leds) {};

	 // Set color functions
	void set_strip(uint8_t strip_number, CHSV color);
	void set_each_strip_with_color_diff(uint16_t delay_time, uint8_t hue, uint8_t hue_diff);
	void set_tile(uint8_t tile_number, CHSV color);
	void set_column_seat(uint8_t column_number, CHSV color);
	void set_column_back(uint8_t column_number, CHSV color);
	void set_pixel(uint8_t row, uint8_t column, CHSV color);
	void set_rainbow_standing(CHSV color);
	void set_rainbow_laying(CHSV color);

	// Fill color functions
	void fill_strip(uint8_t strip_number, uint16_t delay_time, CHSV color);
	void fill_tile(uint8_t tile_number, uint16_t delay_time, CHSV color);
	void fill_column_seat(uint8_t column_number, uint16_t delay_time, CHSV color);
	void fill_column_back(uint8_t column_number, uint16_t delay_time, CHSV color);
	void fill_column(uint8_t column_number, uint16_t delay_time, CHSV color);

	// Show functions
	void strole_strip_downwards(uint16_t delay_time, CHSV color);
	void strole_strip_upwards(uint16_t delay_time, CHSV color);
	void strole_column_downwards(uint16_t delay_time, CHSV color);
	void strole_column_upwards(uint16_t delay_time, CHSV color);
	void fill_strip_downwards(uint16_t delay_time, CHSV color);
	void fill_strip_upwards(uint16_t delay_time, CHSV color);
	void fill_from_left(uint16_t delay_time, CHSV color);
	void fill_from_right(uint16_t delay_time, CHSV color);
	void empty_strip_downwards(uint16_t delay_time, CHSV color);
	void empty_strip_upwards(uint16_t delay_time, CHSV color);

	void four_tile_show(uint16_t delay_time, CHSV color);

private:
	CRGB* leds;	

	int num_leds_col;
	int num_leds_row;
};






#endif

