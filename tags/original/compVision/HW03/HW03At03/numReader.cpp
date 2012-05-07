/*
 *  numReader.cpp
 *  readingTest
 *
 *  Created by Daniel Beatty on Sat May 25 2002.
 *  Copyright (c) 2002 __MyCompanyName__. All rights reserved.
 *
 */

#include "numReader.h"

#include <string.h>
//using namespace std;

bool numReader::openFile (const char * filename)
{
    input.open (filename);
    //buffer = new char[128];
    return (open = input.good());
}

bool numReader::closeFile ()
{
    if (open) {
        input.close();
        return true;
    } 
    return false;
}

bool numReader::readLine()
{
    buffer = new char[128];
    if (input.eof())
        return false;
    input.getline (buffer, 128, '\n');
    
    cout << buffer;
   // cout << sizeof(buffer);
    parseLine ();
    
   // cout << " :";
   // cout << " Xw = " << Xw ;
   // cout << " Yw = " << Yw ;
   // cout << " Zw = " << Zw ;
   // cout << " Xi = " << Xi ;
   // cout << " Yi = " << Yi ;
   // cout << endl;
    //ifstream infile();
    delete []buffer;
    return true;
}

bool numReader::parseLine ()
{
    i = 0;
    if ( getdouble (Xw) )
        if ( getdouble (Yw))
            if (getdouble (Zw))
                if (getdouble (Xi))
                    if ( getdouble(Yi))
                        return true;
    return false;
}

bool numReader::getdouble (double &value)
{
    if ( i >= 128) return false;
    char ch;
    double temp=0;
    double plus;
    double sign = 1;
    bool number = false;
    //int i = 0;
    bool stateRepeat = true;
    do {
        ch = buffer[i++];
        switch (ch)
        {
            case '0': 
                        plus = 0;
                        number = true;
                        stateRepeat = true;
                        temp = temp * 10 + plus;
                        break;
            case '1': 
                        plus = 1;
                        number = true;
                        stateRepeat = true;
                        temp = temp * 10 + plus;
                        break;
            case '2': 
                        plus = 2;
                        number = true;
                        stateRepeat = true;
                        temp = temp * 10 + plus;
                        break;
            case '3': 
                        plus = 3;
                        number = true;
                        stateRepeat = true;
                        temp = temp * 10 + plus;
                        break;
            case '4': 
                        plus = 4;
                        number = true;
                        stateRepeat = true;
                        temp = temp * 10 + plus;
                        break;
            case '5': 
                        plus = 5;
                        number = true;
                        stateRepeat = true;
                        temp = temp * 10 + plus;
                        break;
            case '6': 
                        plus = 6;
                        number = true;
                        stateRepeat = true;
                        temp = temp * 10 + plus;
                        break;
            case '7': 
                        plus = 7;
                        number = true;
                        stateRepeat = true;
                        temp = temp * 10 + plus;
                        break;
            case '8': 
                        plus = 8;
                        number = true;
                        stateRepeat = true;
                        temp = temp * 10 + plus;
                        break;
            case '9': 
                        plus = 9;
                        number = true;
                        stateRepeat = true;
                        temp = temp * 10 + plus;
                        break;
            case '.' : 
            case '\0':
                        stateRepeat = false;
                        break;
            case '-' :	
                    if (number) 
                        {
                            number = false;
                            stateRepeat = false;
                        } else
                        {
                            number = false;
                            stateRepeat = false;
                            sign = -1;
                        }
            case ' ' :
            case '\t': 
                        if ( number)
                            stateRepeat = false;
                            else 
                            stateRepeat = true;
                        break;
            default:
                        number = false;
                        stateRepeat = false;
        }
        if ( i >= 128 ) stateRepeat = false;
    }while (stateRepeat);
    if (number)
    {
        value = sign *temp;
        return true;
    } 
        value = 0;
        return false;
}

void numReader::setBuffer (const char* test)
{
    int i, imax = strlen (test);
    delete []buffer;
    buffer = new char [128];
    for ( i = 0; i < imax; i++)
        buffer[i] = test[i];
}