#include <stdio.h>
#include <stdint.h>


int main()
{	
	uint64_t a = 10;

	printf("%llu\n", UINT64_MAX - (UINT64_MAX % a));	
	return 0;	
}
