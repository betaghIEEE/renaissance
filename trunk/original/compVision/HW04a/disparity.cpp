/*
 *  disparity.cpp
 *  
 *
 *  Created by Daniel Beatty on Tue May 07 2002.
 *  Copyright (c) 2002 Texas Tech University. All rights reserved.
 *
 */

#include "disparity.h"
#include <math.h>
#include <stdlib.h>
using namespace std;

disparity::disparity () 
{
    m_rows = -1;
    m_cols = -1;
    allocateStorage();
}

disparity::~disparity ()  {deleteStorage();}

disparity::disparity (int rows, int cols) 
{
    m_rows = rows;
    m_cols = cols;
    allocateStorage();
}

bool disparity::allocateStorage (int rows, int cols)
{
    m_rows = rows;
    m_cols = cols;
    allocateStorage();
}

bool disparity::allocateStorage ()
{
    if (m_rows <=0 || m_cols <=0) {
        m_data = NULL;
        return false;
    }

  m_data = new dispar*[m_rows];
  m_data[0] = new dispar[m_rows*m_cols];
  for (int i=1; i<m_rows; i++)
    m_data[i] = m_data[i-1]+m_cols;
  return true;
}


void disparity::insert ( int i, int j, int d1, int d2, double cd, double intensity)
{
    m_data[i][j].d1 = d1;
    m_data[i][j].d2 = d2;
    m_data[i][j].cdMax = cd;
    m_data[i][j].I = intensity;
}

bool disparity::deleteStorage()
{
  if (m_data != NULL) {
    delete []m_data[0];
    delete []m_data;
    m_data = NULL;
  }
  return true;
}