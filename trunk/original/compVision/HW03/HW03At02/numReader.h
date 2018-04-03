/*
 *  numReader.h
 *  readingTest
 *
 *  Created by Daniel Beatty on Sat May 25 2002.
 *  Copyright (c) 2002 __MyCompanyName__. All rights reserved.
 *
 */

//#include <Carbon/Carbon.h>
#include <fstream.h>
#include <iostream.h>

class numReader
{
    private:
        ifstream input;
        int position;
        int i;
        char *buffer;
        double Xw;
        double Yw;
        double Zw;
        double Xi;
        double Yi;
        bool open;
    public:
        bool openFile (const char* filename);
        bool closeFile ();
        bool readLine ();
        bool parseLine();
        bool getdouble (double &value);
        double getXw () { return Xw;};
        double getYw () { return Yw;};
        double getZw () { return Zw;};
        double getXi () { return Xi;};
        double getYi () { return Yi;};
        void setBuffer (const char* test);
};