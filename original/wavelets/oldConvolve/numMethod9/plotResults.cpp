#include "plotResults.hpp"
#include <fstream.h>
#include <iostream.h>
//using namespace std;

void plotResults::plotVal (char *output, myVector A, myVector B)
{
	int i;
	int n = A.Size();
	int m = B.Size();
	int o, p;
	double iD;
	ofstream outfile(output);
	outfile << "# Plot results from renaissance " << endl;
	if ( m > n)
		{
			o = n;
			p = m;
		}
		else
		{
			o = m;
			p = n;
		}
	for ( i = 0; i < o; i++)
	{
		iD = i;
		outfile << iD << '\t' << A[i] << '\t' << B[i] << endl;
	}
	if ( m < n )
		for ( i = m; i < n ; i++)
		{
			iD = i;
			outfile << iD << '\t' << A[i] << '\t' << 0 << endl;
		}
	else 
		for ( i = n; i < m ; i++)
		{
			iD = i;
			outfile << iD << '\t' << 0 << '\t' << B[i] << endl;
		}
	outfile.close();
	
}
/*

void plotResults::plotVal (char *output, vector<double> A, vector<double> B)
{
	int i;
	int n = A.size();
	int m = B.size();
	int o, p;
	double iD;
	ofstream outfile(output);
	outfile << "# Plot results from renaissance " << endl;
	if ( m > n)
		{
			o = n;
			p = m;
		}
		else
		{
			o = m;
			p = n;
		}
	for ( i = 0; i < o; i++)
	{
		iD = i;
		outfile << iD << '\t' << A[i] << '\t' << B[i] << endl;
	}
	if ( m < n )
		for ( i = m; i < n ; i++)
		{
			iD = i;
			outfile << iD << '\t' << A[i] << '\t' << 0 << endl;
		}
	else 
		for ( i = n; i < m ; i++)
		{
			iD = i;
			outfile << iD << '\t' << 0 << '\t' << B[i] << endl;
		}
	outfile.close();
	
}
*/

void plotResults::plotVal (char *output, /*const*/ myVector &A)
{
	int i;
	int n = A.Size();
	//int m = B.size();
	//int o, p;	double iD;
	ofstream outfile(output);
	outfile << "# Plot results from renaissance " << endl;
	outfile << "# X           Y   values";
        cout << endl;
	for ( i = 0; i < n; i++)
	{
		iD = i;
		outfile << iD << '\t' << A[i] << endl;
	}
	outfile.close();
	
}

/*
void plotResults::plotVal (char *output, const vector<double> &A)
{
	int i;
	int n = A.size();
	//int m = B.size();
	//int o, p;
	double iD;
	ofstream outfile(output);
	outfile << "# Plot results from renaissance " << endl;
	outfile << "# X           Y   values";
        cout << endl;
	for ( i = 0; i < n; i++)
	{
		iD = i;
		outfile << iD << '\t' << A[i] << endl;
	}
	outfile.close();
	
}
*/
