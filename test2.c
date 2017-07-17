#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

uint32_t half_add(uint32_t a, uint32_t b) {
    if (b == 0) return a;
    uint32_t sum = a ^ b;
    uint32_t carry = (a & b) << 1;
    return half_add(sum, carry);
}



uint32_t mul32(uint32_t a, uint32_t b)
{
    uint32_t result=0, i;
    uint32_t j=0;
    for(i=b; b; b>>=1)
    {
	if(b&1)  //if bit i is 1
	    result = half_add(result, (a<<j));
	j++;
    }
    return result;
}

uint32_t mul32_recursive(uint32_t a, uint32_t b)
{
    if(b == 0)
	return 0;
    else
	return half_add(((b&1)?a:0), (mul32_recursive(a, b>>1)<< 1));
}



int main(void)
{
    uint32_t in1, in2, sum;

    scanf("%d", &in1);
    scanf("%d", &in2);

    sum = mul32_recursive(in1, in2);

    printf("result = %d\n", sum);
}
