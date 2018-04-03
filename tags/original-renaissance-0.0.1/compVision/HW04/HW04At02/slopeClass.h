/*
 *  slopeClass.h
 *  HW02At05
 *
 *  Created by Daniel Beatty on Mon Feb 25 2002.
 *  Copyright (c) 2002 Texas Tech University. All rights reserved.
 *
 */


class SlopeClass {
    public:
        double m;
        double b;
        int count;
        int key () { return count;}
};

class SlopeList {
    private:
        SlopeClass slope [65535];
        int count;
        int size;
    public:
        void insert (double m, double b,int value);
        void remove ();
        void sort ();
        void quickSort (int l,int r);
        void swap (int i, int j);
        void slopeThresholdCleanUp ();
        double getCurrentM ();
        double getCurrentB ();
        int getCurrentHit ();
        void first () { count = 0; }
        void last () { count = size - 1;}
        void next () { if (count < size) count++; else last();}
        void prev () { if (count > 0 ) count--; else first();}        
        int getCount () {return count;}
        int getSize() {return size;}
};

