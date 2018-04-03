/*
 *  reconstruction.cpp
 *  HW04At02
 *
 *  Created by Daniel Beatty on Sun May 12 2002.
 *  Copyright (c) 2002 Texas Tech University. All rights reserved.
 *
 */

#include "reconstruction.h"

void reconstruction::computeWorldPoints ()
{
    int iMax = Id.getRowSize();
    int jMax = Id.getColSize();
    imageType pl (1,3);
    imageType pr (1,3);
    imageType world (1,3);
    worldPoint worldP;
    double tempI;
    double tempJ;
    double Int;
    int i;
    int j;
    int tI, tJ;
    
    for (i = 0; i < iMax; i++)
        for (j = 0; j < jMax; j++)
        {
            
            tempI = i;
            tempJ = j;
            pl.setValue(0,0,tempI);
            pl.setValue(0,1,tempJ);
            pl.setValue(0,2,1.0);
            tI = i + Id.getD1(i,j);
            tJ = j + Id.getD2(i,j);
            tempI = tI;
            tempJ = tJ;
            pr.setValue(0,0, tempI);
            pr.setValue(0,1, tempJ);
            pr.setValue(0,2, 1.0);
            world = computePoint( pl,pr);
            Int = (left.getValue(i,j) + right.getValue(tI, tJ))/2;
            worldP.x = world.getValue(0,0);
            worldP.y = world.getValue(0,1);
            worldP.z = world.getValue(0,2);
            
            worldP.I = Int;
            setWorldPoint(i,j, worldP);
        }
}

imageType reconstruction::computePoint (imageType leftPoint, imageType rightPoint)
{
    imageType tempVectorA (1,3);
    imageType tempVectorB (1,3);
    imageType tempVectorC (1,3);
    imageType tempMatrix (3,3);
    imageType A(3,3);
    imageType AT(3,3);
    imageType ATA(3,3);
    imageType ATAinv (3,3);
    imageType xTemp;
    imageType estLeft(1,3);
    imageType estRight(1,3);
    imageType point (1,3);
    worldPoint onePoint;
    double alpha = fl; 
    double beta = fr;
    double a,b;
    double tempA, tempB, tempC;
    int i;
    
    // set up matrix A
    tempMatrix = R.transpose();
    tempVectorA = alpha * left;
    tempVectorB = beta * ( tempMatrix * right);
    tempVectorC = cross(tempVectorA, tempVectorB);
    for ( i = 0; i < 3; i++)
        {
            tempA = tempVectorA.getValue(0,i);
            tempB = tempVectorB.getValue(1,i);
            tempC = tempVectorC.getValue(2,i);
            A.setValue (i,0, tempA);
            A.setValue (i,1,tempB);
            A.setValue (i,2, tempC);
        }
        
        
    AT = A.transpose();
    ATA = AT * A;
    ATAinv = ATA.inverse();
    xTemp = ATAinv * AT * T;
    a = xTemp.getValue(0,0) * alpha;
    b = xTemp.getValue (0,1) * beta;
    estLeft = a * left;
    estRight = b * right;
    a = estLeft.getValue(0,0);
    b = estRight.getValue(0,0);
    tempA = (a + b)/2;
    point.setValue(0,0, tempA);
    a = estLeft.getValue(0,1);
    b = estRight.getValue(0,1);
    tempA = (a + b)/2;
    point.setValue(0,1, tempA);
    a = estLeft.getValue(0,2);
    b = estRight.getValue(0,2);
    tempA = (a + b)/2;
    point.setValue(0,2, tempA);
    return point;
} 

imageType reconstruction::cross ( imageType left, imageType right)
{
    //int rows = left.getRowSize();
    //int cols = left.getColSize();
    imageType temp(1,3);
    double x,y,z;
    z = left.getValue(0,0) * right.getValue(0,1);
    z -= left.getValue(0,1) * right.getValue(0,0);
    y = left.getValue(0,2) * right.getValue(0,0);
    y -= left.getValue(0,0) * right.getValue(0,2);
    x = left.getValue(0,1) * right.getValue(0,2);
    x -= left.getValue(0,2) * right.getValue(0,1);
    temp.setValue(0,0,x);
    temp.setValue(0,1,y);
    temp.setValue(0,2,z);
    
}
  

void reconstruction::setWorldPoint (int i, int j, worldPoint aPoint)
{
    // 
    int wpPoint = (i * m_cols) + j;
    int index;
    wp[wpPoint].x = aPoint.x;
    wp[wpPoint].y = aPoint.y;
    wp[wpPoint].z = aPoint.z;
    wp[wpPoint].I = aPoint.I;
    wp[wpPoint].next = aPoint.next;
    wp[wpPoint].prev = aPoint.prev;
    for (index = 0; index < 3; i++)
        wp[wpPoint].color[index] = aPoint.color[index];
    
}

worldPoint reconstruction::getWorldPoint (int i, int j)
{
    int wpPoint = (i * m_cols) + j;
    return wp[wpPoint];
}

void reconstruction::allocateWorldPoints (int rows, int cols)
{
    m_rows = rows;
    m_cols = cols;
    if ((rows > 0) && (rows < 2000))
        if ((cols > 0) && (cols < 2000))
            wp = new worldPoint[rows*cols];
}

reconstruction::reconstruction (const char* alignL , const char *alignR , const char* leftImage, const char* rightImage)
{
    leftAlign = new calibrate(alignL);
    rightAlign = new calibrate(alignR);
    pointObtain = new correlate();
    pointObtain->setIntParams(leftAlign, true);
    pointObtain->setIntParams(rightAlign, false);
    pointObtain->setExtParams(leftAlign, true);
    pointObtain->setExtParams(rightAlign, false);
    left = imageType(leftImage);
    right = imageType(rightImage);
    fl = leftAlign->getFocalLength();
    fr = rightAlign->getFocalLength();
    
}

void reconstruction::rawCorrespondence ()
{
    pointObtain->compRawCorr(left,right);
    Id = pointObtain->getCorrPairs ();
    //fl = pointObtain->getFocalLeft();
    //fr = pointObtain->getFocalRight ();
    
    computeWorldPoints();
    produceVRML ("rawCorrespondence.wrl");
}
               
void reconstruction::produceVRML (const char* filename)
{
}