#include "haarGen.hpp"
using namespace std;
#include <stddef.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>

const myVector& haarGen::genAve(int s)
{
	//double iD;
	double temp;
        double halfs = s/2;
        double sqhalf = pow (0.5,0.5);
        double pval = pow(2.0, halfs) * sqhalf;
	int i;
        int Size = 2 * int(pow(2,s));
        
	myVector ave (Size) ;// = valarray(Size);
	if ((Size%2) <= 0)
		return ave;
	for ( i = 0; i < Size; i++)
	{
                    temp = pval;
                ave[i] = temp;
	}
	return ave;
}



/*const*/ myVector& haarGen::genAve()
{
    int Size = 2;
    double s = 1 / pow (2.0, 0.5);
    myVector ave(Size);
    ave[1] = ave[0] = s;
    return ave;
}

/*const*/ myVector& haarGen::genDiff ()
{
    int Size = 2;
    double s = pow (0.5, 0.5);
    myVector diff(Size );
    //diff.Size(2);
    //diff[0] = diff[1] = 0;
    //diff[0] = 0;
    diff[0] = s;
    diff[1] = -s;
    return diff;
}

void haarGen::genAve(myVector& ave)
{
    int Size = 2;
    double s = 1 / pow (2.0, 0.5);
    //myVector ave(Size);
	ave.deallocate();
	ave.allocate(Size);
    ave[1] = ave[0] = s;
    //return ave;
}

void haarGen::genDiff (myVector& diff)
{
    int Size = 2;
    double s = pow (0.5, 0.5);
     diff.deallocate();
	diff.Size(Size);
    //myVector diff(Size );
    //diff.Size(2);
    //diff[0] = diff[1] = 0;
    //diff[0] = 0;
    diff[0] = s;
    diff[1] = -s;
    //return diff;
}

const myVector& haarGen::genDiff (int s)
{
	/*
        double iD;
	double s = Size/2;
	double temp;
	int i;
	myVector diff (Size) ;// = valarray(Size);
	if ((Size%2) == 0)
		return diff;
	for ( i = 0; i < Size; i++)
	{
		iD = i;
		temp = (pow(2,(-s/(3))) * iD );
		if ((temp < 1.5)&&(temp >= 0))
			diff[i] = pow(2,(s/(3*2)));
		 //else if ((temp >= 1.499)&&(temp < 1.5001))
			//diff[i] = 0;
		else
			diff[i] = -(pow(2,(s/(3*2))));
		
	}
        */
	return genDiff(s,0);
}

const myVector& haarGen::genDiff (int s, int shift)
{
	double iD;
	double temp;
        double halfs = s/2;
        double sqhalf = pow (0.5,0.5);
        double pval = pow(2.0, halfs) * sqhalf;
	int i;
        int Size = 2 * int(pow(2,s));
        
	myVector diff (Size) ;// = valarray(Size);
	if ((Size%2) <= 0)
		return diff;
	for ( i = 0; i < Size; i++)
	{
		iD = pow(2,-s) - shift;
                if ( (iD >= 0) && (iD < 1))
                    temp = pval;
                    else
                    temp = -pval;
                diff[i] = temp;
	}
	return diff;
}


const myVector& haarGen::genSample (int Size)
{
        //double iD;
	double s = Size/ 8;
	double temp, temp2, temp3, temp4;
	int i;
	myVector sample (Size) ;// = valarray(Size);
        
        for ( i = 0; i < Size; i++)
        {
            temp = i / s; 
            temp2 = 2 * temp;
            temp3 = 3 * temp;
            temp4 = 4 * temp;
            sample[i] = 10 * sin(temp) - 5 * sin(temp2) + 2 * sin(temp3) - sin(temp4);
        }
        return sample;
}

void haarGen::genSample (int Size, myVector &sample)
{
        //double iD;
	double s = Size/ 8;
	double temp, temp2, temp3, temp4;
	int i;
	sample.deallocate();
	sample.Size (Size) ;// = valarray(Size);
        
        for ( i = 0; i < Size; i++)
        {
            temp = i / s; 
            temp2 = 2 * temp;
            temp3 = 3 * temp;
            temp4 = 4 * temp;
            sample[i] = 10 * sin(temp) - 5 * sin(temp2) + 2 * sin(temp3) - sin(temp4);
        }
        //return sample;
}

const myVector& haarGen::genSample ()
{
        //double iD;
        int Size = 10;
	myVector sample (Size) ;// = valarray(Size);
        sample[0] = 5;
        sample[1] = 8;
        sample[2] = 10;
        sample[3] = 10;
        sample[4] = 9;
        sample[5] = 6;
        sample[6] = 3;
        sample[7] = -1;
        sample[8] = -1.5;
        sample[9] = -1;
        
        return sample;
}
