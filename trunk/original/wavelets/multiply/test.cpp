/*
 *  test.cpp
 *  
 *
 *  Created by Daniel Beatty on Mon Jun 23 2003.
 *  Copyright (c) 2003 __MyCompanyName__. All rights reserved.
 *
 */

#include "chainlink.h"
#include <iostream.h>

int main ()
{
    int i,j;
    chainlink fence;
    fence.allocateHooks (10,5);
    for ( i = 0; i < 10; i++)
        for ( j = 0; j < 5; j++)
        {
            fence.setHookLink (i,j, i * 5 + j);
            fence.setHookKey ( i, j, j + i * j);
        }
    /*
    for ( i = 0; i < 10; i++) {
        cout << endl;
        for ( j = 0; j < 5; j++)
        {
            cout << " The value of " ;
            cout << fence.getHookKey ( i, j);
            cout << " = ";
            cout << fence.getHookLink (i,j);
            cout << "\t";
            
        }
    }
	cout << endl;
      */  
   cout << endl;
   cout << endl;
   cout << "Beginning a direct method" << endl;
   cout << endl;
   cout << endl;
   
    for ( i = 0; i < 2; i++) {
        cout << endl;
        for ( j = 0; j < 5; j++)
        {
            cout << " The value of " ;
            cout << fence[i].getKey(j);
            cout << " = ";
            cout << fence[i].getChLink (j);
            cout << "\t";
            
        }
    }
	cout << endl;
   
                
    return 0;
}

