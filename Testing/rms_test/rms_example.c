#include <stdio.h>
#include <stdint.h>
#include <math.h>

double rms(uint8_t *elements, uint16_t size){
	if(size > 1){
		int cnt = size-1;
		double sum = 0;
		for(; cnt >= 0; cnt--){		
			sum += (double) (elements[cnt] * elements[cnt]);
		}
		sum /= (int) size;
		return sqrt(sum);
	} else if(size == 1){
		return *elements;
	}
	return 0;
}



int main(int argc, char **argv){

	uint8_t elements[10];
	uint8_t i;
	uint16_t size = sizeof(elements)/sizeof(elements[0]);

	for(i = 0; i < size; i++){
		elements[i] = i;
	}
   double result = rms(elements, size);
	printf("RMS: %.4f", result);
	return 0;
}
