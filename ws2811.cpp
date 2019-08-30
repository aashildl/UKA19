// 
// 
// 

#include "ws2811.h"

void LEDControl::set_strip(uint8_t strip_number, CHSV color)
{
	
	for (uint8_t i = 0; i<12; i++)

	{
		this.leds[strip_number * 12 + i] = color;
	}
	FastLED.show();

}

void LEDControl::set_each_strip_with_color_diff(uint16_t delay_time, uint8_t hue, uint8_t hue_diff)
{
	for (uint8_t i = 0; i<12; i++)
	{
		CHSV color_output;
		hue + (hue_diff * i) > 255 ? color_output.hue = hue + (hue_diff * i) - 255 : color_output.hue = hue + (hue_diff * i);
		LEDControl::(this.leds, i, color_output);
		delay(delay_time);
	}
	
	}

void LEDControl::set_tile(uint8_t tile_number, CHSV color)
{
	for (uint8_t j = 0; j <= 4; j += 2) // EVERY SECOND ROW
	{
		for (uint8_t i = 0; i < 6; i++) // COLUMN
		{
			switch (tile_number)
			{
			case 0:
				this.leds[i + (j * 12)] = color;
				this.leds[i + 6 + ((j+1) * 12)] = color;
				break;
			case 1:
				this.leds[i + 6 + (j * 12)] = color;
				this.leds[i + ((j + 1) * 12)] = color;
				break;
			case 3:
				this.leds[i + (j * 12) + OFFSET_BACK] = color;
				this.leds[i + 6 + ((j + 1) * 12) + OFFSET_BACK] = color;
				break;
			case 2: 
				this.leds[i + 6 + (j * 6) + OFFSET_BACK] = color;
				this.leds[i + ((j + 1) * 12) + OFFSET_BACK] = color;
				break;
			}
		}
	}
	FastLED.show();
}

void LEDControl::set_column_seat(uint8_t column_number, CHSV color)
{
	this.leds[column_number] = color;
	this.leds[23 - column_number] = color;
	this.leds[24 + column_number] = color;
	this.leds[47 - column_number] = color;
	this.leds[48 + column_number] = color;
	this.leds[71 - column_number] = color;
	FastLED.show();
}

void LEDControl::set_column_back(uint8_t column_number, CHSV color)
{
	this.leds[column_number + OFFSET_BACK] = color;
	this.leds[23 - column_number + OFFSET_BACK] = color;
	this.leds[24 + column_number + OFFSET_BACK] = color;
	this.leds[47 - column_number + OFFSET_BACK] = color;
	this.leds[48 + column_number + OFFSET_BACK] = color;
	this.leds[71 - column_number + OFFSET_BACK] = color;
	FastLED.show();
}

void LEDControl::set_pixel(uint8_t row, uint8_t column, CHSV color)
{
	if ((row % 2) == 0)
		this.leds[column + (row * 12)] = color;
	else
		this.leds[11 - column + (row * 12)] = color;
	FastLED.show();
}

void LEDControl::fill_strip(uint8_t strip_number, uint16_t delay_time, CHSV color)
{
	for (uint8_t i = 0; i < 12; i++)
	{
		LEDControl::(this.leds, i, color);
	}
}

void LEDControl::fill_tile(uint8_t tile_number, uint16_t delay_time, CHSV color)
{
}

void LEDControl::fill_column_seat(uint8_t column_number, uint16_t delay_time, CHSV color)
{
}

void LEDControl::fill_column_back(uint8_t column_number, uint16_t delay_time, CHSV color)
{
}

void LEDControl::fill_column(uint8_t column_number, uint16_t delay_time, CHSV color)
{
	for (uint8_t i = 0; i < 255; i++)
	{
		set_column_seat(this.leds, column_number, CHSV(color.hue, color.sat, i));
		set_column_back(this.leds, column_number, CHSV(color.hue, color.sat, i));
		delayMicroseconds(delay_time);
		
	}
}


void LEDControl::strole_strip_downwards(uint16_t delay_time, CHSV color)
{
	for (int8_t row = 11; row >= -1; row--)
	{
		if (row < 11)
			LEDControl::(this.leds, row + 1, CHSV(0, 0, 0));
		if (row >= 0)
			LEDControl::(this.leds, row, color);
		FastLED.show();
		delay(delay_time);
	}

}
void LEDControl::strole_column_downwards(uint16_t delay_time, CHSV color)
{
	for (int8_t i = 11; i >= -1; i--)
	{
		if (i < 11) { set_column_seat(this.leds, i + 1, CHSV(0, 0, 0)); } // Clean up last column
		if (i >= 0) { set_column_seat(this.leds, i, color); }; // Set column
		FastLED.show();
		delay(delay_time);
	}
}

void LEDControl::strole_strip_upwards(uint16_t delay_time, CHSV color)
{
	for (uint8_t row = 0; row < 13; row++)
	{
		if(row < 12)
			LEDControl::(this.leds, row, color);
		if (row > 0)
			LEDControl::(this.leds, row - 1, CHSV(0, 0, 0));
		FastLED.show();
		delay(delay_time);
	}
}

void LEDControl::strole_column_upwards(uint16_t delay_time, CHSV color)
{
	for (uint8_t i = 0; i < 13; i++)
	{
		if (i > 0) { set_column_seat(this.leds, i - 1, CHSV(0, 0, 0)); } // Clean up last column
		if (i < 12) { set_column_seat(this.leds, i, color); }; // Set column
		FastLED.show();
		delay(delay_time);
	}

}

void LEDControl::fill_strip_downwards(uint16_t delay_time, CHSV color)
{
	for (uint8_t i = 0; i < 12; i++)
	{
		LEDControl::(this.leds, 11-i, color);
		delay(delay_time);
	}
}

void LEDControl::fill_strip_upwards(uint16_t delay_time, CHSV color)
{
	{
		for (uint8_t i = 0; i < 12; i++)
		{
			LEDControl::(this.leds, i, color);
			delay(delay_time);
		}
	}
}

void LEDControl::fill_from_left(uint16_t delay_time, CHSV color)
{
	for (uint8_t column = 0; column < 12; column++)
	{
		set_column_back(this.leds, column, color);
		set_column_seat(this.leds, column, color);
		delay(delay_time);
	}
}

void LEDControl::fill_from_right(uint16_t delay_time, CHSV color)
{
	for (int8_t column = 11; column >= 0; column--)
	{
		set_column_back(this.leds, column, color);
		set_column_seat(this.leds, column, color);
		delay(delay_time);
	}
}
void LEDControl::empty_strip_downwards(uint16_t delay_time, CHSV color)
{
	
}

void LEDControl::empty_strip_upwards(uint16_t delay_time, CHSV color)
{
}

void LEDControl::four_tile_show(uint16_t delay_time, CHSV color)
{
	set_tile(this.leds, 0, color);
	set_tile(this.leds, 1, color);
	set_tile(this.leds, 2, color);
	set_tile(this.leds, 3, color);
	delay(delay_time);
	// Set all, then blink inverterted
	for (uint8_t i = 0; i < 2; i++) {
		for(uint8_t tile = 0; tile < 4; tile++)
		{
			set_tile(this.leds, tile, CHSV(0, 0 ,0));
			if (tile > 0)
				set_tile(this.leds, tile - 1, color);
			if (tile == 3)
				set_tile(this.leds, 0, color);
			delay(delay_time);
		}
	}
}

void LEDControl::set_rainbow_standing(CHSV color)
{
	CHSV color_rainbow = color;
	for (uint8_t column = 0; column < 12; column++)
	{
		set_column_seat(this.leds, column, color_rainbow);
		set_column_back(this.leds, column, color_rainbow);
	    color_rainbow.hue + 21 > 255 ? color_rainbow.hue = color_rainbow.hue + 21 - 255 : color_rainbow.hue += 21;
	}
}

void LEDControl::set_rainbow_laying(CHSV color)
{
	CHSV color_rainbow = color;
	for (uint8_t row = 0; row < 12; row++)
	{
		LEDControl::(this.leds, row, color_rainbow);
		color_rainbow.hue + 21 > 255 ? color_rainbow.hue = color_rainbow.hue + 21 - 255 : color_rainbow.hue += 21;
	}
}


