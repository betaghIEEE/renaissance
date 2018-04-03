/*
 *  correlate.cpp
 *  HW04At01
 *
 *  Created by Daniel Beatty on Mon Apr 15 2002.
 *  Copyright (c) 2001 __MyCompanyName__. All rights reserved.
 *
 */

#include "correlate.h"
#include <math.h>
#include <iostream>
using namespace std;

void correlate::setIntParams (calibrate *cal, bool left)
{
    double temp;
    if (left){
       /* 
       Mlint = new imageType(3,3);
        temp = cal->getFx();
        Mlint->setValue (0,0,temp);
        temp = cal->getFy();
        Mlint->setValue (1,1,temp);
        temp = cal->getOx();
        Mlint->setValue(0,2,temp);
        temp = cal->getOy();
        Mlint->setValue(1,2,temp);
        Mlint->setValue(2,2,1);
        */
        Mlint = cal->getMint();
    } else {
        /*
        Mrint = new imageType(3,3);
        temp = cal->getFx();
        Mrint->setValue (0,0,temp);
        temp = cal->getFy();
        Mrint->setValue (1,1,temp);
        temp = cal->getOx();
        Mrint->setValue(0,2,temp);
        temp = cal->getOy();
        Mrint->setValue(1,2,temp);
        Mrint->setValue(2,2,1);
        */ 
        Mrint = cal->getMint();
    }
}

void correlate::setExtParams (calibrate *cal, bool left)
{
    if (left) {
        Tleft = cal->getT();
        Rleft = cal->getR();
    } else {
        Tright = cal->getT();
        Rleft = cal->getR();
    }
    
}

void correlate::compExtParam ()
{
    
    Tlr = Tleft - Tright;
    R= Rleft * Rright.transpose();
    
}


void correlate::getCorrPairs ()
{
  //  return corrPairs();
}

imageType correlate::compEpiPoles ()
{
    imageType Pr, pr;
    double Zr;
    
    Pr = -1 * (R*Tlr);
    Zr = Tlr.getValue(0,2);
    pr = (fright/Zr) * Pr;
    return pr;
    
}

imageType correlate::compEpiLines (double alpha, int i, int j)
{
    double id = i;
    double jd = j;
    double Zr;
    imageType pl (1,3);
    imageType Pl (1,3); 
    imageType Pr (1,3);
    imageType pr (1,3);
    
    pl.setValue(0,0,id);
    pl.setValue (0,1,jd);
    pl.setValue (0,2,fleft);
    Pl = (alpha/ fleft) * pl;
    Pr = R * (Pl - Tlr);
    Zr = Pr.getValue(0,2);
    pr = (fright/Zr) * Pr;
    return pr;
    
}

void correlate::compRawCorr (imageType &left, imageType &right)
{
    imageType er(1,3);
    er = compEpiPoles();
    int i,j,k,l,x,y;
    int ux, uy, vx, vy;
    int W = 20;
    double yest;
    double u,v;
    double alpha = 20 * fleft;
    int halfW = W/2;
    int iMax = left.getRowSize() - W;
    int jMax = left.getColSize() - W;
    int d1, d1Max;
    int d2, d2Max;
    double cd, cdMax;
    
    for (i = W; i < iMax; i++)
        for (j = W; j < jMax; j++) 
        {
            d1Max = 0;
            d2Max = 0;
            cdMax = -1;
            for (d1 = -halfW; d1 < halfW; d1++)
                for (d2 = -halfW; d2 < halfW; d2++)
                {
                    cd = 0;
                    for (k = -halfW; k < halfW; k++)
                        for (l = -halfW; l < halfW; l++)
                        {
                            ux = i+k;
                            uy = i+l;
                            vx= ux -d1;
                            vy = uy -d2;
                            u = left.getValue(ux,uy);
                            v = right.getValue (vx,vy);
                            cd += -(u-v)*(u-v);
                            
                        }
                    if (cdMax < cd) 
                    {
                        d1Max = d1;
                        d2Max = d2;
                        cdMax = cd;
                    }
                }
            //ID.insert (i,j,d1Max, d2Max, cdMax);
        }
    
}

int correlate::round (double x) 
{
    int temp;
    double tempX;
    int answer;
    temp = floor (x);
    tempX = x - temp;
    if (tempX < 0)
        answer = temp;
        else
        answer = temp++;
    return answer;
        
}

void correlate::compEpiCorr (imageType left, imageType right)
{
    
    
    int i,j,k,l,x,y;
    int ux, uy, vx, vy;
    int W = 20;
    double yest;
    double xest;
    double u,v;
    double eijy, eijx, erx, ery;
    double m;
    double alpha = 20 * fleft;
    int halfW = W/2;
    int iMax = left.getRowSize() - W;
    int jMax = left.getColSize() - W;
    int d1, d1Max;
    int d2, d2Max;
    double cd, cdMax;
    imageType er (1,3);
    imageType eij (1,3);
    er = compEpiPoles();
    erx = er.getValue(0,0);
    ery = er.getValue(0,1);
    
    for (i = W; i < iMax; i++)
        for (j = W; j < jMax; j++) 
        {
            d1Max = 0;
            d2Max = 0;
            cdMax = -1;
            eij = compEpiLines(alpha, i,j);
            eijx = eij.getValue(0,0);
            eijy = eij.getValue(0,1);
            m = (ery - eijy)/ (erx - eijx); 
                for ( x = W; x < iMax; x++)
                {
                    xest = x;
                    yest = m * (xest - erx) + ery;
                    y = round(yest);
                    if ( (y > 0) && (y < jMax))
                    {
                        cd = 0;
                        for (k = -halfW; k < halfW; k++)
                            for (l = -halfW; l < halfW; l++)
                            {
                                ux = i+k;
                                uy = i+l;
                                vx= x + k;
                                vy = y + l;
                                u = left.getValue(ux,uy);
                                v = right.getValue (vx,vy);
                                cd += -(u-v)*(u-v);
                            
                            }
                        if (cdMax < cd) 
                        {
                            d1Max = x;
                            d2Max = y;
                            cdMax = cd;
                        } 
                    }
                }
            
            
            
            //ID.insert (i,j,d1Max, d2Max, cdMax);
        }
    
}
