/*
 *  disparity.h
 *  
 *
 *  Created by Daniel Beatty on Tue May 07 2002.
 *  Copyright (c) 2002 Texas Tech University. All rights reserved.
 *
 */

//#include <Carbon/Carbon.h>

struct dispar 
{
    int d1, d2;
    double cdMax;
    double I;
};

class disparity 
{
    private:
        dispar **m_data;
        int m_rows, m_cols;
    public:
        //void makeMatrix (int rows, int cols);
        disparity () ;
        disparity (int rows, int cols) ;
        ~disparity ();
        bool allocateStorage (int rows, int cols);
        bool allocateStorage ();
        bool deleteStorage();
        const dispar* operator [](int row) const { return m_data[row]; }
  dispar* operator [](int row) { return m_data[row]; }
  
        int getD1 (int i, int j) { return m_data[i][j].d1;}
        int getD2 (int i, int j) { return m_data[i][j].d2;}
        double getCdmax (int i, int j) { return m_data[i][j].cdMax;}
        void setD1 (int i, int j, int value) { m_data[i][j].d1 = value;}
        void setD2 (int i, int j, int value) { m_data[i][j].d2 = value;}
        void setCdmax (int i, int j, double value) { m_data[i][j].cdMax = value;}
        void insert ( int i, int j, int d1, int d2, double cd) { insert (i,j,d1,d2,cd, 0.5);};
        void insert ( int i, int j, int d1, int d2, double cd, double intensity);
        
        int getRowSize () { return m_rows;};
        int getColSize () { return m_cols;};
        
        
};
