/*
 *  slopeClass.cpp
 *  HW02At05
 *
 *  Created by Daniel Beatty on Mon Feb 25 2002.
 *  Copyright (c) 2001 __MyCompanyName__. All rights reserved.
 *
 */

#include "slopeClass.h"


void SlopeList::insert (double m, double b,int value){
    count= size;
    slope[count].m=m;
    slope[count].b=b;
    slope[count].count = value;
    size++;
}
void SlopeList::remove (){
    slope[count].m=0;
    slope[count].b=0;
    slope[count].count = 0;
}

void SlopeList::sort (){
    int l = 0;
    int r = size - 1;
    if (r > 0 )
        quickSort (l,r);
    
}

void SlopeList::quickSort (int l,int r)
{
    int pivot;
    int i; 
    int j;
    if (r < l) 
    {
        pivot = slope[r].key();
        i = l -1;
        j = r;
        while (i < j) {
            if (slope[i].key() >= pivot) i++;
            else if (slope[j].key() <= pivot) j--;
            else swap (i,j);
            
        }
        swap (i,r);
        quickSort (l, i-1);
        quickSort (i+1, r);
    }
}

void SlopeList::swap(int i, int j) {
    SlopeClass temp;
    temp.m = slope[i].m;
    temp.b = slope[i].b;
    temp.count = slope[i].count;
    slope[i].m = slope[j].m;
    slope[i].b = slope[j].b;
    slope[i].count = slope[j].count;
    slope[j].m = temp.m;
    slope[j].b = temp.b;
    slope[j].count = temp.count;    
    
}

void SlopeList::slopeThresholdCleanUp ()
{
        // Optional function.
        // Narrow the list of lines by threshold.
        // Objective:  Check list for similar m 
        // Upon finding, add to count simM.count;  zeroize similar M.
        // Find next non-zero and find similar
    
}

double SlopeList::getCurrentM ()
{
    return slope[count].m;
}
double SlopeList::getCurrentB (){
    return slope[count].b;
}

int SlopeList::getCurrentHit (){
    return slope[count].count;
}