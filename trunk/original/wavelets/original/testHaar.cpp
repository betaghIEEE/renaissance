/*
	*	Program Component Test Haar
	*	By Dan Beatty
	*	This module tests the Haar Wavelet under different convolution schemes and puts the results
	*	In to numbered files.  
 */

#include <iostream>
using namespace std;
#include "convolve.hpp"
#include "plotResults.hpp"
#include <strings.h>
#include "haarGen.hpp"
#include "tester.hpp"
#ifndef H_SIZE
#define H_SIZE 9
#endif
#ifndef S_SIZE
#define S_SIZE 128
#endif


void testerHaar::testHaar()
{
	
	
	//myVector sample(S_SIZE);
	//vector<double> haarDiff;
	//vector<double> haarAve;
	//vector<double> resultAve;
	//vector<double> resultDiff;
	convolveThis myConvolve;
	vectormanipulation inEtOut;
	plotResults myPlot;
	char output[12] = "haarTest";
	//char *tempOutput;
	char tempI[2];
	//cout << "Hello World, this is CodeWarrior!" << endl;
	//int S_SIZE = 128;
	//int H_SIZE = 16;
	int i, j;
	int size, halfsize, limit;
	double penergy, energy, ratio, scale;
	//penergy = 0;
	//energy = 0;
	//scale = pow(128.0,2.0);
	
	haarGen haarWav;
	myVector haarDiff = haarWav.genDiff ();
	myVector haarAve = haarWav.genAve ();
        myVector sample = haarWav.genSample (128);
	myVector temp;
	
        myPlot.plotVal ("sample", sample);
        myVector resultDiff;
	myVector results(128);
	
	myConvolve.convolve ( sample, haarDiff, resultDiff);
	myVector resultAve; 
	myConvolve.convolve (sample, haarAve, resultAve);
	size = sample.Size();
	halfsize = sample.Size()/2;
	InEtOut.insert(halfsize -1, resultAve, results);
	InEtOut.insert(halfsize, resultDiff, results);
	
	myPlot.plotVal (output, resultDiff[0], resultAve);
	penergy = sample.L2();
	energy = resultAve.L2();
	cout << "Energy of the original = " << penergy << endl; 
        cout << "Energy first round = " << energy << endl;
	cout << "Energy ratio : " << energy/penergy << endl;
	penergy = energy;
	limit = log(size)/log(2.0);
	for (i = 2; i < limit; i++)
	{
                cout << " The count is " << i << endl;
		halfsize = size/pow(2,double(i));
		
		InEtOut.extract(0,halfsize/2,results, temp);
		myConvolve.convolve ( temp, haarDiff, resultDiff);
                cout << "Obtain average " << endl;
		myConvolve.convolve (temp, haarAve, resultAve);
                cout << "Average obtained" << endl;
		tempI[0] = i%10 + 48;
		tempI[1] = i/10 + 48;
		//tempOutput = strcat(tempI, output);
                output[9] = tempI[0];
                output[8] = tempI[1];
                output[10] = '\0';
                cout << "Adjusted file " << i << endl;
		myPlot.plotVal (output, resultDiff[i], resultAve);
		//myPlot.plotVal (tempOutput, resultDiff);
		energy = 0.0;
		energy = resultAve.L2();
		//if ( (penergy >= 0.000001) && ( penergy <= -0.000001))
			ratio = energy/penergy;
			//else 
			//ratio = 0.0;
		cout << "Energy at round " << i << " = " << energy;
		cout << "\nEnergy previous round = " << penergy;
		
		cout << " Ratio = " << ratio;
		penergy = energy;
		cout << "-------------" << endl;
		
	}	
	
	//return 0;
}
