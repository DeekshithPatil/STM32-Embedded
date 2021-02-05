//This program explains the significance of volatile keyword by using an example of reading data from SRAM
#include<stdint.h>

#if !defined(__SOFT_FP__) && defined(__ARM_FP)
  #warning "FPU is not initialized, but the project is compiling for an FPU. Please initialize the FPU before use."
#endif

#define SRAM_ADDRESS1 0x20000004U //Address to read from

int main(void)
{
	uint32_t value = 0;
	uint32_t volatile *p;
	p = (uint32_t *) SRAM_ADDRESS1; //Pointer pointing to the desired address

	while(1)
	{
		value = *p;
		if(value) break; //If value is non-zero
	}

	while(1);

	return 0;
}
