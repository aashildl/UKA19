#include <avr/io.h>
#include "beat.h"
#include "msgeq7.h"
#include <stdio.h>

// If the music is within a beat or not
uint8_t isBeat(uint16_t filterval){
	static uint8_t beat=0; //1 if beat, 0 if not beat
	static uint16_t beatVal=200;
	static uint16_t noBeatCount=0;

	//find beat, iterate through output list
	
	// beat should be zero when 
	if (beat==0){
		if (filterval>beatVal-BEAT_OFFSET){ // high value and not already beat
			beatVal=filterval;
			noBeatCount=0;
			beat=1;
			return 1;
		}
		if(beatVal>MIN_BEAT_VAL){
			beatVal--;
		}
	}
	
	else {//beat==1
		//4 is nice! changed to 5
		// if filterval is less than beatVal/4
		if ((filterval*2.5<beatVal) && (noBeatCount >= NO_BEAT_COUNT)){ //if value on filter is X times lower than it was when the beat was detected, count as no beat.
			beat=0;
			noBeatCount=0;
			return 0;
		}
		
		else {
			noBeatCount++;
		}
		
		if (noBeatCount>=NO_BEAT_COUNT){
			if(filterval>beatVal){
				beatVal=filterval;
				noBeatCount=0;
				beat=1;
				return 1;
			}
		}

		if (noBeatCount>LONG_RESET_TIME){ //no beat for very long time, increase indexbeat to so that next iteration will check all filters 
			beat=0;
		}
	}
	return 0;
}

