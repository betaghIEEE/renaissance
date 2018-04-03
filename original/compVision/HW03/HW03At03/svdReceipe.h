/*
 *  svdReceipe.h
 *  
 *
 *  Created by Daniel Beatty on Thu May 16 2002.
 *  Copyright (c) 2002 Texas Tech University. All rights reserved.
 *
 */

#include <Carbon/Carbon.h>

class svdReceipie 
{
    private:
        double **a;
        double *w;
        double **v;
        double *rv1;
        double **A;
        int rows;
        int cols;
        double g;
        double anorm;
        double scale;
        
    public:
        void household ();
        void rightTransform ();
        void leftTransform ();
        void BiDiagonal ();
        int max (int a, int b);
        double max (double a, double b);
        double sign (double a, double b);
        int sign ( int a, int b);
        double pythag (double a, double b);
        void computeSVD(int m, int n, double **matrix);
        //void computeSVD 
        void computeSVD ();
        void setSourceMatrix (int m, int n, double **matrix);
        void setSourceMatrix (int m, int n, double *matrix);
        
        //int getNullSpace (double **matrix);
        //void getRangeMatrix (double **matrix);
        //int getDiagonal ( double *vector);
        double getNullSpace (int i, int j);
        double getRangeMatrix (int i, int j);
        double getDiagonal ( int i, int j);
        int getM ();
        int getN ();
        bool allocate (int M, int N, double **matrix);
        bool allocateA (int M, int N);
        bool allocatea (int M, int N);
        bool allocateV (int M, int N);
        
        bool deallocate(double **m_data);
        bool deallocatea();
};