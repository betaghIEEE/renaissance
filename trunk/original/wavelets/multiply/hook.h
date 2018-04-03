/*
 *  hook.h
 *  
 *
 *  Created by Daniel Beatty on Sat Jun 14 2003.
 *  Copyright (c) 2003 DRILL, Texas Tech University. 
 *	All rights reserved.
 *
 */

#ifndef __HOOK_H__
#define __HOOK_H__
#include "chlink.h"
class hook 
{
    private:
        int length;
        int maxsize;
        double l2norm;
        chlink *links;
    public:
    	hook ();
    	hook (int size);
        ~hook();
        void allocateLinks (int size);
        void deallocateLinks ();
        int getLength ();
        void setLength (int s);
        double getl2norm ();
        double calcl2norm ();
        double getChLink (int i);
        void setChLink (int i, double value);
        int getKey (int i);
        void setKey (int i, int k);
        void addLink (int key, double val);
        chlink operator[](int i) { return links[i];};
    
};

#endif