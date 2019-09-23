// 
// 
// 

#include "LEDControl.h"
#define LOW_BLINK_VALUE 70
#define LOW_NO_BLINK_VALUE 50

void LEDControl::show_pause_pattern()
{
	switch (pausePattern)
	{
	case PAUSE_RAINBOW_LAYING:
		increment_color_hue();
		set_color_val(LEDS_VALUE_MAX);
		set_rainbow_laying();
		break;

	case PAUSE_RAINBOW_STANDING:
		increment_color_hue();
		set_color_val(LEDS_VALUE_MAX);
		set_rainbow_standing();
		break;

	case PAUSE_DOUBLE_SNAKE:
		clear_all_leds();
		set_color_val(LEDS_VALUE_MAX);
		double_row_snake(color);
		break;

	case PAUSE_FILL_SNAKE:
		if (focusPosition == 0)
		{
			if (!fillSnakeWithColor)
			{
				// make sure all LEDs are off, in case we come from another pattern
				clear_all_leds();
				increment_color_hue(40);
				fillSnakeWithColor = true;
			}
			else
			{
				fillSnakeWithColor = false;
			}
		}
		set_color_val(fillSnakeWithColor ? LEDS_VALUE_MAX : 0);
		single_snake(color);
		break;

	case PAUSE_UKA_LETTERS:
		color.hue = 0; // base color for UKA letters is red
		color.value = LEDS_VALUE_MAX;
		uka_letters(focusColumn);
		if (toggleBeat)
		{
			if (++focusColumn > (NUM_LEDS_PER_ROW + 32 -1)) focusColumn = 0;

		}
		toggleBeat = !toggleBeat;

		break;

	case PAUSE_FILL_SNAKE_RAINBOW:
		if (focusPosition == 0)
		{
			if (!fillSnakeWithColor)
			{
				// make sure all LEDs are off, in case we come from another pattern
				clear_all_leds();
				increment_color_hue(40);
				fillSnakeWithColor = true;
			}
			else
			{
				fillSnakeWithColor = false;
			}
		}
		increment_color_hue(3);
		set_color_val(fillSnakeWithColor ? LEDS_VALUE_MAX : 0);
		single_snake(color);
		break;

	case PAUSE_SINGLE_ROW_SNAKE:
		clear_all_leds();
		set_color_val(LEDS_VALUE_MAX);
		single_row_snake(color);
		break;

	case PAUSE_FILL_SINGLE_ROW_SNAKE:
		set_color_val(LEDS_VALUE_MAX);
		single_row_snake(color);
		break;

	default:
		break;
	}
}

void LEDControl::show_disco_pattern(bool beat)
{
	if (snakeRunning)
	{
		show_user_snake_pattern();
		return;
	}
	if (beat && nextDiscoPattern != discoPattern)
	{
		if (discoPattern == DISCO_FILL_OFFSET + DISCO_UPWARDS
		|| discoPattern == DISCO_FILL_OFFSET + DISCO_UPWARDS_FILL
		|| discoPattern == DISCO_FILL_OFFSET + DISCO_DOWNWARDS
		|| discoPattern == DISCO_FILL_OFFSET + DISCO_DOWNWARDS_FILL)
		{
			if (focusRow == 0) discoPattern = nextDiscoPattern;
			increment_color_hue(50);
			set_color_val(LEDS_VALUE_MAX);
			color.sat = LEDS_SATURATION_MAX;
		}
		else
		{
			discoPattern = nextDiscoPattern;
			increment_color_hue(50);
			set_color_val(LEDS_VALUE_MAX);
			color.sat = LEDS_SATURATION_MAX;
			focusPosition = 0;
			focusRow = 0;
			focusColumn = 0;
		}
	}
	switch(discoPattern)
	{
		case DISCO_FULL_OFFSET + DISCO_CHANGE:
			if (beat)
			{
				increment_color_hue(50);
				set_color_val(LEDS_VALUE_MAX);
				fill_all_leds_solid();
			}
			break;

		case DISCO_FULL_OFFSET + DISCO_PULSE:
			if(beat){
				ledsAreShown = false;
				increment_color_hue(20);
				set_color_val(LEDS_VALUE_MAX);
				fill_all_leds_solid();
			}
			else
			{
				if(ledsAreShown)
				{
					fade_color();
					fill_all_leds_solid();
					ledsAreShown = false;		
				}
			}
			break;

		case DISCO_FULL_OFFSET + DISCO_TOGGLE:
			if(beat && ledsAreShown){
				ledsAreShown = false;
				if (toggleBeat)
				{
					increment_color_hue(10);
					set_color_val(LEDS_VALUE_MAX);
					fill_all_leds_solid();
				}
				else
				{
					clear_all_leds();
				}
				toggleBeat = !toggleBeat;
			}
			break;

		case DISCO_SNAKE_OFFSET + DISCO_SINGLE_SNAKE:
			increment_color_hue();
			if(beat)
			{
				ledsAreShown = 0;
				color.val = LEDS_VALUE_MAX;
				fill_all_leds_solid();
			}
			else
			{
				if(ledsAreShown)
				{
					ledsAreShown = 0;
					set_color_val(40);
					// Set all LEDs to a low brightness state
					fill_all_leds_solid();
					if (++focusPosition > (NUM_LEDS-1)) { focusPosition = 0; }
					set_color_val(LEDS_VALUE_MAX);
					set_pixel(focusPosition);
					set_pixel(NUM_LEDS-focusPosition-1);
				}
			}
			break;

		case DISCO_FILL_OFFSET + DISCO_UPWARDS:
			if(beat)
			{
				// Turn off all LEDs
				clear_all_leds();		
				// Turn on specific row
				increment_color_hue(40);
				set_color_val(LEDS_VALUE_MAX);
				set_row(focusRow);
				if(++focusRow > (NUM_LED_ROWS-1)) {focusRow = 0;}
			}
			break;

		case DISCO_FILL_OFFSET + DISCO_DOWNWARDS:
			if(beat)
			{
				// Turn off all LEDs
				clear_all_leds();		
				// Turn on specific row
				set_color_val(LEDS_VALUE_MAX);
				set_row(NUM_LED_ROWS - 1 - focusRow);
				if(++focusRow > (NUM_LED_ROWS-1)) {focusRow = 0;}
			}
			break;


		case DISCO_FILL_OFFSET + DISCO_UPWARDS_FILL:
			if (focusRow == 0)
			{
				clear_all_leds();
			}
			if (beat)
			{
				set_color_val(LEDS_VALUE_MAX);
				set_row(focusRow);
				if(++focusRow > (NUM_LED_ROWS-1)) {focusRow = 0;}
			}
			break;

		case DISCO_FILL_OFFSET + DISCO_DOWNWARDS_FILL:
			if (focusRow == 0)
			{
				clear_all_leds();
			}
			if (beat)
			{
				set_color_val(LEDS_VALUE_MAX);
				set_row(NUM_LED_ROWS - 1 - focusRow);
				if(++focusRow > (NUM_LED_ROWS-1)) {focusRow = 0;}
			}
			break;

		case DISCO_SNAKE_OFFSET + DISCO_DOUBLE_SNAKE_ROWS:
			if(beat)
			{
				increment_color_hue(50);
				ledsAreShown = 0;
				color.val = 30;
				maxColorValue = LEDS_VALUE_MAX;
				fill_all_leds_solid();
				set_color_val(LEDS_VALUE_MAX);
				maxColorValue = LEDS_VALUE_MAX;
				double_row_snake(color);
			}
			else
			{
				if(ledsAreShown)
				{
					set_color_val(30);
					fill_all_leds_solid();
					set_color_val(maxColorValue);
					fade_color();
					maxColorValue = color.val;
					double_row_snake(color);
				}
			}
			break;
	}
}

void LEDControl::show_user_low_pattern(bool beat)
{
	if (snakeRunning)
	{
		show_user_snake_pattern();
		return;
	}
	if (beat)
	{
		color.value = LOW_BLINK_VALUE;
		fill_all_leds_solid();
		ledsAreShown = false;
	}
	else
	{
		if (ledsAreShown)
		{
			color.value = LOW_NO_BLINK_VALUE;
			fill_all_leds_solid();
		}
	}
	
}

void LEDControl::show_user_full_pattern(bool beat)
{
	if (snakeRunning)
	{
		show_user_snake_pattern();
		return;
	}
	if(beat && ledsAreShown){
		ledsAreShown = false;
		color.value = LEDS_VALUE_MAX;
		color.hue = 0;
		if (toggleBeat)
		{
			uka_letters((NUM_LEDS_PER_ROW / 2) + (NUM_LETTER_COLS / 2));
		}
		else
		{
			color.sat = LEDS_SATURATION_MAX;
			fill_all_leds_solid();
		}
		toggleBeat = !toggleBeat;
	}
}

void LEDControl::show_user_snake_pattern()
{
	if (!snakeRunning)
	{
		snakeRunning = true;
		focusPosition = 0;
	}

	if (ledsAreShown)
	{
		ledsAreShown = false;
		color.value = 0;
		fill_all_leds_solid();
		color.value = LEDS_VALUE_MAX;
		for (uint8_t i = 0; i < 6; i++)
		{
			set_pixel(focusPosition++);
		}
	}
	if (focusPosition >= NUM_LEDS-6)
	{
		snakeRunning = false;
	}
}

uint8_t LEDControl::switch_pasue_pattern()
{
	pseudoRandomNumber += 23;
	if (pseudoRandomNumber > 200)
	{
		pseudoRandomNumber -= 200; 
	}
	focusColumn = 0;
	focusRow = 0;
	focusPosition = 0;
	fillSnakeWithColor = false;
	set_color_val(LEDS_VALUE_MAX);
	color.sat = LEDS_SATURATION_MAX;
	pausePattern = pseudoRandomNumber % NUM_PAUSE_PATTERNS;
	//pausePattern = (pausePattern + 1) % NUM_PAUSE_PATTERNS;
	return pausePattern;
}

uint8_t LEDControl::switch_disco_pattern()
{
	pseudoRandomNumber += 17;
	if (pseudoRandomNumber > 200)
	{
		pseudoRandomNumber -= 200; 
	}
	switch (discoPattern)
	{
		// cases where all leds are shown simultaneously
	case DISCO_CHANGE:
	case DISCO_PULSE:
	case DISCO_TOGGLE:	
		nextDiscoPattern = DISCO_FILL_OFFSET + pseudoRandomNumber % NUM_DISCO_FILL;
		break;
	case DISCO_SNAKE_OFFSET + DISCO_SINGLE_SNAKE:
	case DISCO_SNAKE_OFFSET + DISCO_DOUBLE_SNAKE_ROWS:
		nextDiscoPattern = DISCO_FULL_OFFSET + pseudoRandomNumber % NUM_DISCO_FULL;
		break;
	case DISCO_FILL_OFFSET + DISCO_UPWARDS:
	case DISCO_FILL_OFFSET + DISCO_DOWNWARDS:
	case DISCO_FILL_OFFSET + DISCO_UPWARDS_FILL:
	case DISCO_FILL_OFFSET + DISCO_DOWNWARDS_FILL:
		nextDiscoPattern = DISCO_SNAKE_OFFSET + pseudoRandomNumber % NUM_DISCO_SNAKE;
		break;
	default:
		break;
	}
	return nextDiscoPattern;
}
void LEDControl::set_ledsAreShown()
{
	ledsAreShown = true;
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

void LEDControl::fade_color()
{
	if (color.value > 150)    	{ color.value-=6;   }
	else if (color.value > 30) 	{ color.value-=3;   }
	else 						{ color.value = 30; }
}

void LEDControl::fill_all_leds_solid()
{
	fill_solid(leds, NUM_LEDS, color);
}

void LEDControl::clear_all_leds()
{
	uint8_t prevColorVal = color.val;
	color.val = 0;
	fill_solid(leds, NUM_LEDS, color);
	color.val = prevColorVal;
}

void LEDControl::set_row(uint8_t row)
{
	for (uint8_t i = 0; i<NUM_LEDS_PER_ROW; i++)
	{
		leds[row * NUM_LEDS_PER_ROW + i] = this->color;
	}
}
void LEDControl::set_row(uint8_t row, CHSV& color)
{
	if (row > NUM_LED_ROWS-1) return; 
	for (uint8_t i = 0; i<NUM_LEDS_PER_ROW; i++)
	{
		leds[row * NUM_LEDS_PER_ROW + i] = color;
	}
}

//11 ---15
//10 --- 6
// 1 --- 5

void LEDControl::set_column(uint8_t column_number)
{
	if (column_number > NUM_LEDS_PER_ROW - 1) return;
	for (uint8_t row = 0; row < NUM_LED_ROWS; row++)
	{
		if (row % 2 == 1)
		{
			leds[NUM_LEDS_PER_ROW * (row + 1) - 1 - column_number] = this->color;
		}
		else
		{
			leds[NUM_LEDS_PER_ROW * row + column_number] = this->color;
		}
	}
}

void LEDControl::set_column(uint8_t column_number, CHSV& color)
{
	if (column_number > NUM_LEDS_PER_ROW - 1) return;
	for (uint8_t row = 0; row < NUM_LED_ROWS; row++)
	{
		if (row % 2 == 1)
		{
			leds[NUM_LEDS_PER_ROW * (row + 1) - 1 - column_number] = color;
		}
		else
		{
			leds[NUM_LEDS_PER_ROW * row + column_number] = color;
		}
	}
}

void LEDControl::set_pixel(uint8_t row, uint8_t column)
{
	if (column > NUM_LEDS_PER_ROW - 1) return;
	if (row > NUM_LED_ROWS - 1) return;
	if ((row % 2) == 0)
		this->leds[column + (row * NUM_LEDS_PER_ROW)] = color;
	else
		this->leds[NUM_LEDS_PER_ROW - 1 - column + (row * NUM_LEDS_PER_ROW)] = color;
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
}


void LEDControl::strole_strip_downwards(uint16_t delay_time)
{
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


void LEDControl::fill_from_left(uint16_t delay_time)
{
	for (uint8_t column = 0; column < NUM_LEDS_PER_ROW; column++)
	{
		set_column(column);
	}
}

void LEDControl::fill_from_right(uint16_t delay_time)
{
	for (int8_t column = NUM_LEDS_PER_ROW; column >= 0; column--)
	{

		set_column(column);
	}
}
void LEDControl::empty_strip_downwards(uint16_t delay_time)
{
	
}

void LEDControl::empty_strip_upwards(uint16_t delay_time)
{
}

void LEDControl::set_rainbow_standing()
{
	CHSV color_rainbow = color;
	for (uint8_t column = 0; column < NUM_LEDS_PER_ROW; column++)
	{
		set_column(column, color_rainbow);
		if (color_rainbow.hue > LEDS_HUE_MAX - LEDS_HUE_MAX / NUM_LEDS_PER_ROW)
		{
			color_rainbow.hue = color_rainbow.hue - LEDS_HUE_MAX + LEDS_HUE_MAX / NUM_LEDS_PER_ROW;
		}
		else
		{
			color_rainbow.hue += LEDS_HUE_MAX / NUM_LEDS_PER_ROW;
		}
	}
}

void LEDControl::set_rainbow_laying()
{
	CHSV color_rainbow = color;
	for (uint8_t row = 0; row < NUM_LED_ROWS; row++)
	{
		set_row(row, color_rainbow);
		if (color_rainbow.hue > LEDS_HUE_MAX - LEDS_HUE_MAX / NUM_LED_ROWS)
		{
			color_rainbow.hue = color_rainbow.hue - LEDS_HUE_MAX + LEDS_HUE_MAX / NUM_LED_ROWS;
		}
		else
		{
			color_rainbow.hue += LEDS_HUE_MAX / NUM_LED_ROWS;
		}
		//color_rainbow.hue + 21 > 255 ? color_rainbow.hue = color_rainbow.hue + 21 - 255 : color_rainbow.hue += 21;
	}
}

void LEDControl::double_row_snake(CHSV& color)
{
	if (focusColumn == 0)
	{
		increment_color_hue(20);
	}
	CHSV fadeColor = color;
	set_column(focusColumn);
	set_column(NUM_LEDS_PER_ROW-focusColumn-1);
	for (int i = 0; i < 5; i++)
	{
		if (focusColumn - i >= 0 && focusColumn + i < NUM_LEDS_PER_ROW)
		{
			fadeColor.val = color.val / (i+1);
			set_column(focusColumn-i, fadeColor);
			set_column(NUM_LEDS_PER_ROW-focusColumn-1+i, fadeColor);
		}
	}
	if(++focusColumn > NUM_LEDS_PER_ROW-1) {focusColumn = 0;}
}

void LEDControl::single_row_snake(CHSV& color)
{
	if (focusColumn == 0)
	{
		increment_color_hue(50);
	}
	CHSV fadeColor = color;
	if (toggleBeat)
	{
		set_column(focusColumn);
	}
	else
	{
		set_column(NUM_LEDS_PER_ROW-focusColumn-1);
	}
	
	for (int i = 0; i < 5; i++)
	{
		if (focusColumn - i >= 0 && focusColumn + i < NUM_LEDS_PER_ROW)
		{
			fadeColor.val = color.val / (i+1);
			if (toggleBeat)
			{
				set_column(focusColumn-i, fadeColor);
			}
			else
			{
				set_column(NUM_LEDS_PER_ROW-focusColumn-1+i, fadeColor);
			}
		}
	}
	if(++focusColumn > NUM_LEDS_PER_ROW-1)
	{
		focusColumn = 0;
		toggleBeat = !toggleBeat;
	}
}

void LEDControl::single_snake(CHSV& color)
{
	if (focusPosition < NUM_LEDS)
	{
		set_pixel(focusPosition);
	}
	if (++focusPosition > (NUM_LEDS-1)) { focusPosition = 0; }
}

void LEDControl::uka_letters(uint8_t offset)
{
	if (offset >= 0 && offset < NUM_LEDS_PER_ROW + 32)
		for (uint8_t row = 0; row < NUM_LED_ROWS; row++)
		{
			for (uint8_t col = 0; col < NUM_LEDS_PER_ROW; col++)
			{
				//if (NUM_LEDS_PER_ROW + 32 - col < 32)
				if (col > NUM_LEDS_PER_ROW - offset -1
				&& col < NUM_LEDS_PER_ROW - offset + 32)
				{
					color.sat = getBit(row, col + 1 + offset - NUM_LEDS_PER_ROW) ? 0 : LEDS_SATURATION_MAX;
				}
				else
				{
					color.sat = LEDS_SATURATION_MAX;
				}
				set_pixel(row, col);
			}
		}
}
