/*
 *  EdgeAndLines.cpp
 *  HW02At05
 *
 *  Created by Daniel Beatty on Mon Feb 25 2002.
 *  Copyright (c) 2001 __MyCompanyName__. All rights reserved.
 *
 */

#include "EdgeAndLines.h"

#include <math.h>

        EdgeAndLines::EdgeAndLines(){
            setConstants();
            LineConstants();
        }
        void EdgeAndLines::parseMatrix ()
        {
            LineConstants();
            int i;
            int j;
            int rows = edges.getRowSize();
            int cols = edges.getColSize();
            double tempEdge;
            
            for (i = 0; i < rows; i++)
                for ( j = 0; j < cols;  j++) {
                    tempEdge = edges.getValue(i,j);
                    if (tempEdge >= 1 )
                        computeSlopeCount(i,-j);
                }
        }
        
        
        void EdgeAndLines::computeSlopeCount (int i, int j)
        {
            int m;
            int bLimit = SlopeMatrixPlus.getColSize();
            double countPlus;
            double countMinus;
            int bPlus;
            int bMinus;
            double mValue;
            double fudge = 36/maxM;
            for (m =0; m < maxM; m++){
                mValue = pow(2,(m*fudge + 6)) ; //
                bPlus = ceil(mValue * (-i) + j)+ bMed;
                bMinus = ceil (mValue * i + j) + bMed;
                
                
                if ((bPlus < (bLimit)) && (bPlus > 0)){
                    countPlus = SlopeMatrixPlus.getValue(m,bPlus);
                    countPlus++;
                    SlopeMatrixPlus.setValue(m,bPlus, countPlus);
                    }
                if ((bMinus < (bMax + bMed)) && (bMinus > 0)){
                    countMinus = SlopeMatrixMinus.getValue(m,bMinus);
                    countMinus++;
                    SlopeMatrixMinus.setValue(m,bMinus, countMinus);
                    }
                
                
            }
            
        }
        
        
        void EdgeAndLines::obtainSlopeList (double thresh)
        {
            imageType mask = imageType (3,3);
            imageType result ;
            int i;
            int j;
            double b;
            int rows ;
            int cols ;
            double m;
            double tempDouble;
            // double n;
            int tempValue;
            mask.setValue (0, 0, 1/24);
            mask.setValue (0, 1, 2/24);
            mask.setValue (0, 2, 1/24);
            mask.setValue (2, 0, 1/24);
            mask.setValue (2, 1, 2/24);
            mask.setValue (2, 2, 1/24);
            mask.setValue (1, 0, 2/24);
            mask.setValue (1, 1, 12/24);
            mask.setValue (1, 2, 2/24);
            //result = SlopeMatrixPlus.convolve(mask);
            result = SlopeMatrixPlus;
            rows = result.getRowSize();
            cols = result.getColSize();
            
            for (i = 0; i < rows;  i++)
                for (j = 0; j < cols; j++){
                    tempDouble = result.getValue(i,j);
                    tempValue = int(tempDouble);
                    m = pow (2, (36*i/maxM +6));
                    b= j - bMed;
                    if (tempValue >= thresh) 
                        longSlopeList.insert(m,b,tempValue);
                }
                
            result = SlopeMatrixMinus.convolve(mask);
            rows = result.getRowSize();
            cols = result.getColSize();
            
            for (i = 0; i < rows;  i++)
                for (j = 0; j < cols; j++){
                    tempValue = int(result.getValue(i,j));
                    m = -pow (2, (36*m/maxM +6));
                    b = j - bMed;
                    if (tempValue >= thresh) 
                        longSlopeList.insert(m,b,tempValue);
                }
            longSlopeList.sort();
            
        }
        
 
        void EdgeAndLines::setConstants () {
            PI = 3.1415;
            quarterPi = PI/4;
            halfPi = quarterPi * 2;
            threeQuarterPi = quarterPi * 3;
            eighthPi = quarterPi/2;
            thigh = 1.3/255.0;
            tlow = 0;
            
        }  
        
        
        void EdgeAndLines::LineConstants() {
            int m;
            int b;
            bMax =  edges.getColSize();
            bMin = -edges.getColSize();
            bMed = edges.getColSize();
            maxM = 200;
            m = 200;
            b= 2 * bMax;
            SlopeMatrixPlus = imageType(m,b);
            SlopeMatrixMinus = imageType(m,b);
        }
        
        
        // For each element in Jrow and Jcolumn, EdgeDirection[i][j] = arctan (Jcolumn[i][j]/Jrow[i,j]);  if Jrow[i][j] == 0, arctan = pi/2;
        
        imageType EdgeAndLines::FindE0(){
            int row = Jrow.getRowSize();
            int column = Jrow.getColSize();
            int i;
            int j;
            double tempRow;
            double tempCol;
            double temp;
            // Test column values
            if ((Jcolumn.getRowSize() != row) || ( Jcolumn.getColSize() != column))
                return (EdgeDirection = 0);
            EdgeDirection = imageType (row,column);
            for (i = 0; i < row; i++)
                for (j = 0; j < column ; j++) {
                    tempRow = Jrow.getValue(i,j);
                    tempCol = Jcolumn.getValue(i,j);
                    if (tempRow == 0){
                        if (tempCol < 0) 
                            temp= halfPi;
                            else 
                            temp = -halfPi;
                    } else {
                        temp = atan(tempCol/tempRow);
                    }
                    EdgeDirection.setValue (i,j,temp);
                    
                        
                }
                return EdgeDirection;
        }
        
        
        //  
        imageType EdgeAndLines::FindE0Round(){
            double temp;
            double tempValue;
            int i, j;
            int iMax, jMax;
            iMax = EdgeDirection.getRowSize();
            jMax = EdgeDirection.getColSize();
            EdgeCardDirection = imageType (iMax,jMax);
            for (i = 0; i < iMax; i++)
                for ( j= 0; j < jMax; j++)
                    {
                        temp = EdgeDirection.getValue(i,j);
                        if (( temp >= -(halfPi)) && ( temp < -(halfPi+eighthPi))) {
                                tempValue = -(halfPi);                            			} else if (( temp >= -(halfPi + eighthPi)) && ( temp < -(quarterPi + eighthPi))) {
                            tempValue = -(quarterPi);
                        } else if (( temp >= -(quarterPi + eighthPi)) && ( temp < (quarterPi - eighthPi))) {
                            tempValue = 0;
                        } else if (( temp >= (quarterPi - eighthPi)) && ( temp < (quarterPi + eighthPi))) {
                            tempValue = (quarterPi);
                        } else {
                            tempValue = (halfPi);
                        }
                        EdgeCardDirection.setValue (i,j, tempValue);
                    }
            return EdgeCardDirection;
        }
        
        imageType EdgeAndLines::FindEdgeStrength(){
            int row = Jrow.getRowSize();
            int column = Jrow.getColSize();
            int i;
            int j;
            double tempRow;
            double tempCol;
            double temp;
            // Test column values
            if ((Jcolumn.getRowSize() != row) || ( Jcolumn.getColSize() != column))
                return (EdgeStrength = 0);
            EdgeStrength = imageType (row,column);
            for (i = 0; i < row; i++)
                for (j = 0; j < column ; j++) {
                    tempRow = Jrow.getValue(i,j);
                    tempCol = Jcolumn.getValue(i,j);
                    temp = tempRow *tempRow + tempCol * tempCol;
                    temp = sqrt (temp);
                    EdgeStrength.setValue (i,j,temp);
                }
                return EdgeStrength;
            
        }
        
        imageType EdgeAndLines::FindIN(){
            
            double tempDir;
            double tempPlus;
            double tempMinus;
            double tempStrength;
            int i;
            int j;
            int k;
            int l;
            int tempI;
            int tempJ;
            int rows = EdgeStrength.getRowSize();
            int cols = EdgeStrength.getColSize();
            imageType tempImage (rows, cols);
            
            for (i=0; i < rows; i++)
                for (j=0; j < cols ;  j++) {
                    tempDir = EdgeCardDirection.getValue (i,j);
                    if ((tempDir == (-halfPi)) || (tempDir == (halfPi))) {
                        k = 0;
                        l = 1;
                    } else if (tempDir == (-quarterPi)) {
                        k = 1;
                        l = -1;
                    } else if (tempDir == (quarterPi)){
                        k = 1;
                        l = 1;
                    } else {
                        k = 1;
                        l = 0;
                    }
                    
                    tempI = i+k;
                    tempJ= j+l;
                    tempPlus = 0;
                    tempMinus = 0;
                    if ((tempI >= 0) && (tempI < rows) && (tempJ >= 0) && (tempJ < cols))
                        tempPlus = EdgeStrength.getValue(tempI,tempJ);
                    
                    tempI = i-k;
                    tempJ= j-l;
                    if ((tempI >= 0) && (tempI < rows) && (tempJ >= 0) && (tempJ < cols))
                        tempMinus = EdgeStrength.getValue(tempI,tempJ);
                    tempStrength = EdgeStrength.getValue(i,j);  
                    if ((tempStrength > tempPlus) && (tempStrength > tempMinus))
                        tempImage.setValue(i,j, tempStrength);
                        else
                        tempImage.setValue(i,j, 0);
                }
                return (IN = tempImage);
        
        }
        
        imageType EdgeAndLines::ComputeJx(){
            imageType temp;
            temp = GaussianImage.calcRowGradientFast();
            //temp = image.calcRowGradientFast();
                Jrow = temp;
            return temp;
        }
        
        
        imageType EdgeAndLines::ComputeJy(){
            imageType temp;
            temp = GaussianImage.calcColGradientFast();
            //temp = image.calcColGradientFast();
                Jcolumn = temp;
            return temp;        
        }
        
        
        imageType EdgeAndLines::FindHysterisisThreshold(){
            //double tempDir;
            double tempDirCard;
           // double tempPlus;
            double tempVisited;
            double tempStrength;
            int i;
            int j;
            int k;
            int l;
            //int altK;
            //int altL;
            int m;
            int n;
            //int eState;
            
            int rows = EdgeStrength.getRowSize();
            int cols = EdgeStrength.getColSize();
            // imageType tempImage (rows, cols);
            imageType markedImage (rows, cols);
            
            
            for (i=0; i < rows; i++)
                for (j=0; j < cols ;  j++) 
                        {
                            tempStrength = IN.getValue(i,j);
                            tempVisited = markedImage.getValue(i,j);
                            if ((tempStrength >= thigh ) && (tempVisited <= 1)) {
                            m = i;
                            n = j;
                        tempDirCard = EdgeCardDirection.getValue(m,n);
                                
                                    if (tempDirCard == (-quarterPi)) {
                                        k =  1;
                                        l =  1;
                                    } else if (tempDirCard == (quarterPi)){
                                        k =  1;
                                        l =  -1;
                                    } else if (tempDirCard == (-halfPi)) {
                                        k = 1;
                                        l = 0;
                                    } else {
                                        k = 0;
                                        l = 1;
                                    }       

                                // positive Loop
                                //    eState = 1;
                                    //m= i+k;
                                    //n = j+l;
                                    //if ((m > 0) && (m < rows) && (n > 0) && (n < cols) ) tempStrength = IN.getValue(m,n); 
                                    //else tempStrength = 0;
                                while (tempStrength > tlow) {
                                    if ((m > 0) && (m < rows) && (n > 0) && (n < cols) ) markedImage.setValue(m,n,1);
                                    //tempImage.setValue(m,n,1);
                                        m += k;
                                        n += l;
                                       if ((m > 0) && (m < rows) && (n > 0) && (n < cols) )  tempStrength = IN.getValue(m,n); 
                                       else
                                       tempStrength = 0;
                                }
                            
                                
                            // Negative Loop
                                m= i-k;
                                n = j-l;  
                                if ((m > 0) && (m < rows) && (n > 0) && (n < cols) )  tempStrength = IN.getValue(m,n); 
                                       else
                                       tempStrength = 0;                          
                                while (tempStrength > tlow) {
                                    if ((m > 0) && (m < rows) && (n > 0) && (n < cols) ) markedImage.setValue(m,n,1);
                                    //tempImage.setValue(m,n,1);
                                    m -= k;
                                    n -= l;
                                    if ((m > 0) && (m < rows) && (n > 0) && (n < cols) )  tempStrength = IN.getValue(m,n); 
                                       else
                                       tempStrength = 0;
                                } // end of while loop
                        } // end of if
                } // end of for loop
                hysterisisOutline = markedImage;
                return (edges = markedImage);
        }  // end of Hysterisis Threshold
        
        
        imageType EdgeAndLines::FindGaussian (){
            imageType mask = imageType (3,3);
            //imageType result ;
            mask.setValue (0, 0, (1/24.0));
            mask.setValue (0, 1, (2/24.0));
            mask.setValue (0, 2, (1/24.0));
            mask.setValue (2, 0, (1/24.0));
            mask.setValue (2, 1, (2/24.0));
            mask.setValue (2, 2, (1/24.0));
            mask.setValue (1, 0, (2/24.0));
            mask.setValue (1, 1, (12/24.0));
            mask.setValue (1, 2, (2/24.0));
            GaussianImage = image.convolve(mask);
            return (GaussianImage);
        }
        
        
        void EdgeAndLines::setImage(imageType aImage){
            image = aImage;
        }
        
        void EdgeAndLines::computeImageEdges () {
            FindGaussian();
            ComputeJx ();
            ComputeJy ();
            FindEdgeStrength ();
            FindE0 ();
            FindE0Round ();
            FindIN();
            FindHysterisisThreshold();
        }
        
        void EdgeAndLines::plotLine () 
        {
            int maxLine = longSlopeList.getSize();
            int i;
            
            double m;
            double b;
            int x;
            int y;
            longSlopeList.first();
            imageLine = image;
            int xLimit = imageLine.getRowSize();
            int yLimit = imageLine.getColSize();
            
            for ( i =0; i < maxLine; i++)
                {
                    m = longSlopeList.getCurrentM();
                    b = longSlopeList.getCurrentB();
                    for (x = 0 ; x < xLimit; x++ )
                    {
                        y = int (- (m * x + b));
                        if ( (y > 0 ) && (y < yLimit))
                            imageLine.setValue (x,y, 0.5);
                    }
                    
                }
            
        }
