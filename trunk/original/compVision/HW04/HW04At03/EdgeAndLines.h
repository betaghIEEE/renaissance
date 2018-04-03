/*
 *  EdgeAndLines.h
 *  HW02At05
 *
 *  Created by Daniel Beatty on Mon Feb 25 2002.
 *  Copyright (c) 2002 Texas Tech University. All rights reserved.
 *
 */

 #include "imageType.h"
 #include "slopeClass.h"

class EdgeAndLines {
    private:
        
            imageType GaussianImage;
            imageType IN;
            imageType image;
            imageType imageLine;
            imageType Jrow;
            imageType Jcolumn;
            imageType EdgeStrength;
            imageType EdgeDirection;
            imageType EdgeCardDirection;
            imageType edges;
            imageType hysterisisOutline;
            double quarterPi;
            double halfPi;
            double threeQuarterPi;
            double thigh;
            double tlow;
            double PI ;
            double eighthPi;
            
            // hough line variables
            imageType EdgeMatrix;
            imageType SlopeMatrix;
            imageType SlopeMatrixPlus;
            imageType SlopeMatrixMinus;
            SlopeList longSlopeList;
            SlopeList shortSlopeList;
            double maxSlope;
            double minSlope;
            double maxIntercept;
            double minIntercept;
            double countThreshold;
            int minM;
            int maxM;
            int bMed;
            int bMax;
            int bMin;
        
    public:
        EdgeAndLines();
        void setConstants ();
        void LineConstants();
        imageType FindE0();
        imageType FindE0Round();
        imageType FindEdgeStrength();
        imageType FindIN();
        imageType ComputeJx();
        imageType ComputeJy();
        imageType FindHysterisisThreshold();
        imageType FindGaussian ();
        void setImage(imageType aImage);
        void computeImageEdges ();
        imageType getGaussianImage ()  {return GaussianImage ;}
        imageType getIN () {return IN ;}
        imageType getImage () {return image ;}
        imageType getJrow () {return Jrow ;}
        imageType getJcolumn () {return Jcolumn ;}
        imageType getEdgeStrength () {return EdgeStrength ;}
        imageType getEdgeDirection () {return EdgeDirection ;}
        imageType getEdgeCardDirection () {return EdgeCardDirection ;}
        imageType getEdges () {return edges ;}
        imageType getHysterisisOutline () {return hysterisisOutline ;}
        imageType getImageLine () { return imageLine;}
        
        // hough lines
        
    
        void parseMatrix ();
        void computeSlopeCount (int i, int j);
        void obtainSlopeList (double thresh);
        void plotLine ();
        void setEdgeImage (imageType &source) { EdgeMatrix = source;}
       
    
};
