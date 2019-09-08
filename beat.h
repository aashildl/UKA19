#ifndef BEAT_H_
#define BEAT_H_

#include <avr/io.h>
#define N_FILTER 1
#define NO_BEAT_COUNT 70
#define MIN_BEAT_VAL 300
#define BEAT_OFFSET 0
#define LONG_RESET_TIME 1024
//uses the lowest bandpass-filters on MSGEQ7 up to "filter" and analyses them for beat.
//this function should be run very often (whith timer maybe?)
uint8_t isBeat(uint16_t filterval);

//uses all the filters on MSGEQ7 and tries to make sense of them.
//should let through up to 7 filters. if less filters is choosen, it will
//use some kind of average values of all the filters divided in equally parts
//returns pointer to int array. [length, value1,value2,...,value7]
//int * getEQ(int length);



#endif /* BEAT_H_ */