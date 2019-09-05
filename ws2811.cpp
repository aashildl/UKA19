// 
// 
// 

#include "ws2811.h"

void LEDControl::show_pause_pattern()
{
	switch (pausePattern)
	{
	case RAINBOW:
		set_rainbow_laying();
		break;
	
	case SNAKE:
		fadeToBlackBy(leds, NUM_LEDS, 20);
		//int pos = beatsin16(13, 0, NUM_LEDS - 1);
		//leds[pos] += CHSV(gHue, 255, 192);
		break;


	default:
		break;
	}
}

void LEDControl::show_disco_pattern(bool beat)
{
	switch(discoPattern)
	{
		case DISCO_CHANGE:
		if (beat)
		{
			increment_color_hue(50);
			fill_num_leds_solid(NUM_LEDS);
			break;
		}
		case DISCO_PULSE:
		if (beat)
		{
			
		}
	}
}

void LEDControl::set_color(CHSV color)
{
	this->color = color;
}

void LEDControl::set_color(uint8_t gHue, uint8_t gSat, uint8_t gVal)
{
	this->color = CHSV(gHue, gSat, gVal);
}

void LEDControl::set_color_val(uint8_t val)
{
	color.val = val;
}

void LEDControl::increment_color_hue()
{
	color.hue++;
}

void LEDControl::increment_color_hue(uint8_t step)
{
	color.hue += step;
}

uint8_t LEDControl::get_color_hue()
{
	return color.hue;
}

const CHSV& LEDControl::get_color()
{
	return color;
}

void LEDControl::fade_color()
{
	if (color.value >150)    	{ color.value-=2; }
	else if (color.value > 40) 	{ color.value--;}
	else 					{ color.value = 40; }
}

void LEDControl::fill_num_leds_solid(uint16_t numToFill)
{
	fill_solid(leds, numToFill, color);
}

void LEDControl::set_strip(uint8_t strip_number)
{
	
	for (uint8_t i = 0; i<NUM_LEDS_PER_ROW; i++)

	{
		leds[strip_number * NUM_LEDS_PER_ROW + i] = color;
	}
	//FastLED.show();

}

void LEDControl::set_each_strip_with_color_diff(uint16_t delay_time, uint8_t hue, uint8_t hue_diff)
{
	for (uint8_t i = 0; i<12; i++)
	{
		CHSV color_output;
		hue + (hue_diff * i) > 255 ? color_output.hue = hue + (hue_diff * i) - 255 : color_output.hue = hue + (hue_diff * i);
		//LEDControl::set_strip(i, color_output);
		delay(delay_time);
	}
	
	}

void LEDControl::set_tile(uint8_t tile_number)
{
	for (uint8_t j = 0; j <= 4; j += 2) // EVERY SECOND ROW
	{
		for (uint8_t i = 0; i < 6; i++) // COLUMN
		{
			switch (tile_number)
			{
			case 0:
				this->leds[i + (j * 12)] = color;
				this->leds[i + 6 + ((j+1) * 12)] = color;
				break;
			case 1:
				this->leds[i + 6 + (j * 12)] = color;
				this->leds[i + ((j + 1) * 12)] = color;
				break;
			case 3:
				this->leds[i + (j * 12) + OFFSET_BACK] = color;
				this->leds[i + 6 + ((j + 1) * 12) + OFFSET_BACK] = color;
				break;
			case 2: 
				this->leds[i + 6 + (j * 6) + OFFSET_BACK] = color;
				this->leds[i + ((j + 1) * 12) + OFFSET_BACK] = color;
				break;
			}
		}
	}
	//FastLED.show();
}

void LEDControl::set_column_seat(uint8_t column_number)
{
	this->leds[column_number] = color;
	this->leds[23 - column_number] = color;
	this->leds[24 + column_number] = color;
	this->leds[47 - column_number] = color;
	this->leds[48 + column_number] = color;
	this->leds[71 - column_number] = color;
	//FastLED.show();
}

void LEDControl::set_column_back(uint8_t column_number)
{
	this->leds[column_number + OFFSET_BACK] = color;
	this->leds[23 - column_number + OFFSET_BACK] = color;
	this->leds[24 + column_number + OFFSET_BACK] = color;
	this->leds[47 - column_number + OFFSET_BACK] = color;
	this->leds[48 + column_number + OFFSET_BACK] = color;
	this->leds[71 - column_number + OFFSET_BACK] = color;
	//FastLED.show();
}

void LEDControl::set_pixel(uint8_t row, uint8_t column)
{
	if ((row % 2) == 0)
		this->leds[column + (row * 12)] = color;
	else
		this->leds[11 - column + (row * 12)] = color;
	//FastLED.show();
}
void LEDControl::set_pixel(uint16_t pos)
{
	if (pos < NUM_LEDS)
	{
		leds[pos] = color;
	}
}

void LEDControl::fill_strip(uint8_t strip_number, uint16_t delay_time)
{
	for (uint8_t i = 0; i < 12; i++)
	{
		LEDControl::set_strip(i);
	}
}

void LEDControl::fill_tile(uint8_t tile_number, uint16_t delay_time)
{
}

void LEDControl::fill_column_seat(uint8_t column_number, uint16_t delay_time)
{
}

void LEDControl::fill_column_back(uint8_t column_number, uint16_t delay_time)
{
}

void LEDControl::fill_column(uint8_t column_number, uint16_t delay_time)
{
	for (uint8_t i = 0; i < 255; i++)
	{
		//set_column_seat(column_number, CHSV(color.hue, color.sat, i));
		//set_column_back(column_number, CHSV(color.hue, color.sat, i));
		delayMicroseconds(delay_time);
		
	}
}


void LEDControl::strole_strip_downwards(uint16_t delay_time)
{
	for (int8_t row = 11; row >= -1; row--)
	{
		if (row < 11)
			//LEDControl::set_strip(row + 1, CHSV(0, 0, 0));
		if (row >= 0)
			//LEDControl::set_strip(row, color);
		//FastLED.show();
		delay(delay_time);
	}

}
void LEDControl::strole_column_downwards(uint16_t delay_time)
{
	for (int8_t i = 11; i >= -1; i--)
	{
		//if (i < 11) { set_column_seat(i + 1, CHSV(0, 0, 0)); } // Clean up last column
		//if (i >= 0) { set_column_seat(i, color); }; // Set column
		//FastLED.show();
		delay(delay_time);
	}
}

void LEDControl::strole_strip_upwards(uint16_t delay_time)
{
	for (uint8_t row = 0; row < 13; row++)
	{
		if(row < 12)
			//LEDControl::set_strip(row, color);
		if (row > 0)
			//LEDControl::set_strip(row - 1, CHSV(0, 0, 0));
		//FastLED.show();
		delay(delay_time);
	}
}

void LEDControl::strole_column_upwards(uint16_t delay_time)
{
	for (uint8_t i = 0; i < 13; i++)
	{
		//if (i > 0) { set_column_seat(i - 1, CHSV(0, 0, 0)); } // Clean up last column
		//if (i < 12) { set_column_seat(i, color); }; // Set column
		//FastLED.show();
		delay(delay_time);
	}

}

void LEDControl::fill_strip_downwards(uint16_t delay_time)
{
	for (uint8_t i = 0; i < 12; i++)
	{
		LEDControl::set_strip(11-i);
		delay(delay_time);
	}
}

void LEDControl::fill_strip_upwards(uint16_t delay_time)
{
	{
		for (uint8_t i = 0; i < 12; i++)
		{
			LEDControl::set_strip(i);
			delay(delay_time);
		}
	}
}

void LEDControl::fill_from_left(uint16_t delay_time)
{
	for (uint8_t column = 0; column < 12; column++)
	{
		set_column_back(column);
		set_column_seat(column);
		delay(delay_time);
	}
}

void LEDControl::fill_from_right(uint16_t delay_time)
{
	for (int8_t column = 11; column >= 0; column--)
	{
		set_column_back(column);
		set_column_seat(column);
		delay(delay_time);
	}
}
void LEDControl::empty_strip_downwards(uint16_t delay_time)
{
	
}

void LEDControl::empty_strip_upwards(uint16_t delay_time)
{
}

void LEDControl::four_tile_show(uint16_t delay_time)
{
	set_tile(0);
	set_tile(1);
	set_tile(2);
	set_tile(3);
	delay(delay_time);
	// Set all, then blink inverterted
	for (uint8_t i = 0; i < 2; i++) {
		for(uint8_t tile = 0; tile < 4; tile++)
		{
			//set_tile(tile, CHSV(0, 0 ,0));
			if (tile > 0)
				set_tile(tile - 1);
			if (tile == 3)
				set_tile(0);
			delay(delay_time);
		}
	}
}

void LEDControl::set_rainbow_standing()
{
	CHSV color_rainbow = color;
	for (uint8_t column = 0; column < 12; column++)
	{
		//set_column_seat(column, color_rainbow);
		//set_column_back(column, color_rainbow);
	    color_rainbow.hue + 21 > 255 ? color_rainbow.hue = color_rainbow.hue + 21 - 255 : color_rainbow.hue += 21;
	}
}

void LEDControl::set_rainbow_laying()
{
	CHSV color_rainbow = color;
	for (uint8_t row = 0; row < 12; row++)
	{
		//LEDControl::set_strip(row, color_rainbow);
		color_rainbow.hue + 21 > 255 ? color_rainbow.hue = color_rainbow.hue + 21 - 255 : color_rainbow.hue += 21;
	}
}


