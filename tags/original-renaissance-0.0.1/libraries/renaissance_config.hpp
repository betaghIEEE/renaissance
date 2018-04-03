/*  ----------------------------------------
		renaissance_config.hpp
		--------------------

		General code to make the library run smoothly.
		Stuff that should be defined under windows, 
		but isn't.
*/


#ifndef __RENAISSANCE_CONFIG_HPP__
#define __RENAISSANCE_CONFIG_HPP__

// one-argument macros 

// absolute value of a 
#define ABS(a)		(((a)<0) ? -(a) : (a))

// round a to nearest int 
#define ROUND(a)	((a)>0 ? (int)((a)+0.5) : -(int)(0.5-(a)))

// take sign of a, either -1, 0, or 1 
#define ZSGN(a)		(((a)<0) ? -1 : (a)>0 ? 1 : 0)	

// take binary sign of a, either -1, or 1 if >= 0 
#define SGN(a)		(((a)<0) ? -1 : 1)

// shout if something that should be true isn't 
#ifndef ASSERT
#define ASSERT(x)					\
if (!(x)) fprintf(stderr," Assert failed: x\n");
#endif


// square a 
#define SQR(a)		((a)*(a))	

//*********************
// two-argument macros 
//*********************

// find minimum of a and b 
#define MIN(a,b)	(((a)<(b))?(a):(b))	

// find maximum of a and b 
#define MAX(a,b)	(((a)>(b))?(a):(b))	

// swap a and b (see Gem by Wyvill) 
#define SWAP(a,b)	{ a^=b; b^=a; a^=b; }

// linear interpolation from l (when a=0) to h (when a=1)
// (equal to (a*h)+((1-a)*l) 
#define LERP(a,l,h)	((l)+(((h)-(l))*(a)))

// clamp the input to the specified range 
#define CLAMP(v,l,h)	((v)<(l) ? (l) : (v) > (h) ? (h) : v)

#endif // __RENAISSANCE_CONFIG_HPP__
