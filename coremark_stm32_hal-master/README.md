# coremark_stm32_hal
coremark for stm32 MCU, using hal library

steps:

1. with core_portme.h file, change ITERATIONS to an appropriate value.
2. with core_portme.c file, replace specific MCU platform header file, then change uart handle for communication.
3. in you project's main() function, add a function call to core_main(), following peripherals' initialization code.

enjoy!

#Readme:
	How to transplant coremark to stm32 platform?
	
1. Add hardware init code to function portable_init() in file core_portme.c
	eg. GPIO, USART, TIMER configurations and retargets printf()
2. Configure a timer , get an interrupt per millisecond
3. Modify macro GETMYTIME() in file core_portme.c, to get current time (in ms)
4. Modify macro EE_TICKS_PER_SEC to 1000, 1 ms interrupt means 1000Hz Ticks
5. Add macro ITERATIONS, usually define to 10000 ~20000
6. Modify macro COMPILER_VERSION and COMPILER_FLAGS in core_portme.h, 
	to indicate compiler version and flags, used for print out to user
	
	OK, it's done!