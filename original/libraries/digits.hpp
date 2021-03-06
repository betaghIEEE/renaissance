#ifndef __DIGITS_HPP__
#define __DIGITS_HPP__


unsigned char _digitI0[8][6] = {
	{ 255, 255, 255, 255, 255, 0}, 
	{ 227, 29, 0, 142, 255, 0}, 
	{ 85, 142, 227, 0, 255, 0}, 
	{ 85, 170, 255, 0, 255, 0}, 
	{ 85, 170, 255, 0, 255, 0}, 
	{ 85, 142, 255, 0, 255, 0}, 
	{ 227, 29, 0, 170, 255, 0}, 
	{ 255, 255, 255, 255, 255, 0} 
	};
unsigned char _digitI1[8][6] = {
	{ 255, 255, 255, 0, 0, 0}, 
	{ 85, 0, 255, 0, 0, 0}, 
	{ 255, 0, 255, 0, 0, 0}, 
	{ 255, 0, 255, 0, 0, 0}, 
	{ 255, 0, 255, 0, 0, 0}, 
	{ 255, 0, 255, 0, 0, 0}, 
	{ 255, 0, 255, 0, 0, 0}, 
	{ 255, 255, 255, 0, 0, 0} 
	};
unsigned char _digitI2[8][6] = {
	{ 255, 255, 255, 255, 255, 0}, 
	{ 227, 29, 0, 170, 255, 0}, 
	{ 85, 142, 227, 0, 255, 0}, 
	{ 255, 255, 227, 29, 255, 0}, 
	{ 255, 227, 29, 170, 255, 0}, 
	{ 227, 29, 170, 255, 255, 0}, 
	{ 85, 0, 0, 0, 255, 0}, 
	{ 255, 255, 255, 255, 255, 0} 
	};
unsigned char _digitI3[8][6] = {
	{ 255, 255, 255, 255, 255, 0}, 
	{ 227, 29, 0, 199, 255, 0}, 
	{ 199, 142, 170, 85, 255, 0}, 
	{ 255, 170, 0, 142, 255, 0}, 
	{ 255, 255, 142, 29, 255, 0}, 
	{ 114, 170, 227, 0, 255, 0}, 
	{ 170, 29, 0, 170, 255, 0}, 
	{ 255, 255, 255, 255, 255, 0} 
	};
unsigned char _digitI4[8][6] = {
	{ 255, 255, 255, 255, 255, 255}, 
	{ 255, 255, 114, 85, 255, 255}, 
	{ 255, 199, 0, 85, 255, 255}, 
	{ 255, 29, 142, 85, 255, 255}, 
	{ 114, 142, 170, 85, 255, 255}, 
	{ 0, 0, 0, 0, 170, 255}, 
	{ 255, 255, 170, 85, 255, 255}, 
	{ 255, 255, 255, 255, 255, 255} 
	};
unsigned char _digitI5[8][6] = {
	{ 255, 255, 255, 255, 255, 0}, 
	{ 199, 0, 0, 170, 255, 0}, 
	{ 170, 85, 255, 255, 255, 0}, 
	{ 114, 0, 0, 170, 255, 0}, 
	{ 199, 199, 227, 0, 255, 0}, 
	{ 114, 199, 227, 0, 255, 0}, 
	{ 170, 29, 0, 170, 255, 0}, 
	{ 255, 255, 255, 255, 255, 0} 
	};
unsigned char _digitI6[8][6] = {
	{ 255, 255, 255, 255, 255, 255}, 
	{ 255, 227, 29, 170, 255, 255}, 
	{ 255, 114, 85, 255, 255, 255}, 
	{ 199, 0, 0, 85, 255, 255}, 
	{ 85, 142, 227, 29, 170, 255}, 
	{ 85, 142, 255, 57, 199, 255}, 
	{ 227, 29, 0, 114, 255, 255}, 
	{ 255, 255, 255, 255, 255, 255} 
	};
unsigned char _digitI7[8][6] = {
	{ 255, 255, 255, 255, 255, 0}, 
	{ 170, 0, 0, 0, 255, 0}, 
	{ 255, 255, 142, 57, 255, 0}, 
	{ 255, 255, 57, 170, 255, 0}, 
	{ 255, 199, 29, 255, 255, 0}, 
	{ 255, 57, 142, 255, 255, 0}, 
	{ 227, 0, 227, 255, 255, 0}, 
	{ 255, 255, 255, 255, 255, 0} 
	};
unsigned char _digitI8[8][6] = {
	{ 255, 255, 255, 255, 255, 0}, 
	{ 255, 57, 0, 170, 255, 0}, 
	{ 170, 85, 170, 85, 255, 0}, 
	{ 227, 0, 29, 142, 255, 0}, 
	{ 114, 85, 142, 29, 255, 0}, 
	{ 85, 142, 227, 0, 255, 0}, 
	{ 227, 29, 0, 142, 255, 0}, 
	{ 255, 255, 255, 255, 255, 0} 
	};
unsigned char _digitI9[8][6] = {
	{ 255, 255, 255, 255, 255, 255}, 
	{ 227, 29, 0, 114, 255, 255}, 
	{ 85, 142, 227, 29, 199, 255}, 
	{ 85, 142, 227, 57, 170, 255}, 
	{ 199, 0, 0, 29, 255, 255}, 
	{ 255, 227, 0, 199, 255, 255}, 
	{ 255, 57, 142, 255, 255, 255}, 
	{ 255, 255, 255, 255, 255, 255} 
	};
int digitRows[] = {8, 8, 8, 8, 8, 8, 8, 8, 8, 8};
int digitCols[] = {5, 3, 5, 5, 6, 5, 6, 5, 5, 6};
#endif // __DIGITS_HPP__
