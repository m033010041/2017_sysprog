#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>


uint32_t func(uint32_t x) {
    uint32_t n = x;
    n = ((n & 0xffff0000) >> 16) | ((n & 0x0000ffff) << 16);
    n = ((n & 0xff00ff00) >>  8) | ((n & 0x00ff00ff) <<  8);
    n = ((n & 0xf0f0f0f0) >>  4) | ((n & 0x0f0f0f0f) <<  4);
    n = ((n & 0xcccccccc) >>  2) | ((n & 0x33333333) <<  2);
    n = ((n & 0xaaaaaaaa) >>  1) | ((n & 0x55555555) <<  1);
    return n;
}

uint32_t func1(uint32_t x) {
    uint32_t r = x & 1;
    uint32_t shift = sizeof(x) * 8 - 1;

    for(x = x >> 1; x; x>>=1)
    {
	r = r << 1;
	r = r | (x & 1);
	shift--;
    }
    printf("shift = %d\n", shift);
    r <<= shift;
    return r;
}

uint16_t func2(uint16_t x) {
    uint16_t r = x & 1;
    uint16_t shift = sizeof(x) * 8 - 1;

    for(x = x >> 1; x; x>>=1)
    {
	r = r << 1;
	r = r | (x & 1);
	shift--;
    }
    printf("shift = %d\n", shift);
    r <<= shift;
    return r;
}


int main(void)
{
	uint32_t input, output;
	printf("input: ");
	scanf("%d", &input);
	output = func1(input);
	printf("origin=%08x, reverse=%08x\n", input, output);
	return 0;
}
