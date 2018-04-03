/*
 *  chainlink.h
 *  
 *
 *  Created by Daniel Beatty on Sat Jun 14 2003.
 *  Copyright (c) 2003 DRILL, Texas Tech University. All rights reserved.
 *
 */
#include "hook.h"

#ifndef __CHAINLINK_H__
#define __CHAINLINK_H__

class chainlink 
{
    private:
        int length;
        hook *theHooks;
    public:
        void allocateHooks (int size);
        void allocateHooks (int hooksize, int linksize);
        void deallocateHooks ();
        int getLength ();
        void setLength (int s);
        int getLinkLength (int i) { return theHooks[i].getLength();};
        double getL2Norm (int i);
        double calcL2Norm (int i);
        double getHookLink (int i, int j);
        void setHookLink (int i, int j, double val);
        int getHookKey (int i, int j);
        void setHookKey (int i, int j, int key);
    	void addHookLink (int i, int key, double val);
        hook operator[](int i) { return theHooks[i];}; 
    
};

#endif