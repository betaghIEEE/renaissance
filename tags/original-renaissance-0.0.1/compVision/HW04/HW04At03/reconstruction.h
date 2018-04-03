/*
 *  reconstruction.h
 *  HW04At02
 *
 *  Created by Daniel Beatty on Sun May 12 2002.
 *  Copyright (c) 2002 Texas Tech University. All rights reserved.
 *
 */

#include <Carbon/Carbon.h>
#include "correlate.h"

/*
struct worldPoint
{
    double x;
    double y;
    double z;
    double I;
    double color[3];
    worldPoint *next;
    worldPoint *prev;
    
};
*/

struct worldPoint
{
    double x;
    double y;
    double z;
    double I;
    double color[3];
    int next;
    int prev;
    
};

class reconstruction
{
    private:
        worldPoint *wp;
        int worldCount;
        int m_rows, m_cols;
        imageType left;
        imageType right;
        imageType R, T;
        imageType x;
        // imageType EsL, EsR;
        // imageType EdL, EdR;
        // imageType LeftCorners, RightCorners;
        disparity Id;
        calibrate *leftAlign;
        calibrate *rightAlign;
        correlate *pointObtain;
        double fl, fr;
        
    public:
        reconstruction (const char* alignL, const char *alignR, const char* leftImage, const char* rightImage);
        reconstruction ();
        void computeWorldPoints ();
        imageType computePoint (imageType left, imageType right);
        imageType cross (imageType left, imageType right);
        void setWorldPoint (int i, int j, worldPoint aPoint);
        worldPoint getWorldPoint (int i, int j);
        void allocateWorldPoints (int rows, int cols);
        void produceVRML (const char* filename);
        void rawCorrespondence ();
};