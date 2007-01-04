/*
 *  imageMagickLines.h
 *  HW02At05
 *
 *  Created by Daniel Beatty on Mon Feb 25 2002.
 *  Copyright (c) 2001 Texas Tech University. All rights reserved.
 *
 *   Original functions: getImage and saveImage
 *    acquired from 
 *	Eric Sinzinger, PhD. 
 *       Computer Science, Texas Tech University.
 */

#include "EdgeAndLines.h"

class imageAcquistion {
    
    imageType source;
    public:
    imageType getImage (const char* filename);
    bool saveImage (const char * filename);
    void setImage (imageType &value) { source = value;}

};

