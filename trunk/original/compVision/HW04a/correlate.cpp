/*
 *  correlate.cpp
 *  HW04At01
 *
 *  Created by Daniel Beatty on Mon Apr 15 2002.
 *  Copyright (c) 2002 Texas Tech University. All rights reserved.
 *
 */

#include "correlate.h"
#include <math.h>
//#include <iostream>
//#include <fstream.h>
//#include "calType.h"
//#include <string>
//#include "svdReceipe.h"

using namespace std;

correlate::correlate ( const char *leftFile, const char *rightFile)
{
    //computeViaRaw (leftFile, rightFile);
    calType *left = new calType ();
    calType *right = new calType ();
    left->getPointsFromFile(leftFile);
    right->getPointsFromFile(rightFile);
    //left->computeSVD();
    //right->computeSVD();
    left->computeParameter();
    right->computeParameter();
    T = left->getT() - right->getT();
    matrixType temp, Rright, Rleft;
    temp = right->getR();
    Rright = matrixType(3,3);
    Rleft = left->getR();
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            Rright.m_data[i][j] = temp.m_data[i][j];
    R= Rleft * Rright;
    fleft = left->computeFocalLength ();
    fright = right->computeFocalLength ();
    
}

correlate::correlate ()
{
    //Tleft = matrixType (3,1);
    //Tright = matrixType (3,1);
    //Tlr = matrixType (3,1);
    //Rleft = matrixType (4,3);
    //Rright = matrixType (4,3);
    //R = matrixType (4,3);
    //Mlint = matrixType (3,3);
    //Mrint = matrixType (3,3);
    //Mrext = imageType (4,3);
    //Mlext = imageType (4,3);
    
}


void correlate::computeViaRaw ( const char *leftFile, const char *rightFile)
{
   matrixType lmat, rmat;
   getImagePGM2 (leftFile, lmat);
   getImagePGM2 (rightFile, rmat);

    compRawCorr (lmat, rmat);
}



disparity correlate::getCorrPairs ()
{
    return ID;
}

void correlate::compRawCorr (matrixType &left, matrixType &right)
{
    int i,j,k,l; //,x,y;
    int ux, uy, vx, vy;
    int W = 20;
    //double yest;
    double u,v;
    //double alpha = 20 * fleft;
    int halfW = W/2;
    int iMax = left.m_rows;
    int jMax = left.m_cols;
    int Dmax = 100;
    int dxMax, dyMax;
    int dxMin, dyMin;
    int d1, d1Max;
    int d2, d2Max;
    double cd, cdMax;
    cout << "Raw Correspondence started " << endl;
    
    for (i = 0; i < iMax; i++)
        for (j = 0; j < jMax; j++) 
        {
            d1Max = 0;
            d2Max = 0;
            cdMax = -1;
            dxMax = i + Dmax;
            dyMax = j + Dmax;
            if ( dxMax > iMax) dxMax = iMax;
            if (dyMax > jMax) dyMax = jMax;
            dxMin = i - Dmax;
            dyMin = j - Dmax;
            if ( dxMax < 0) dxMax = 0;
            if (dyMax < 0) dyMax = 0;
            
            
            for (d1 = dxMin; d1 < dxMax; d1++)
                for (d2 = dyMin; d2 < dyMax; d2++)
                {
                    cd = 0;
                    for (k = -halfW; k < halfW; k++)
                        for (l = -halfW; l < halfW; l++)
                        {
                            ux = i+k;
                            uy = i+l;
                            vx= ux -d1;
                            vy = uy -d2;
                            if ( ux > 0 && ux < iMax && vx > 0 && vx < iMax && uy > 0 && uy < jMax && vx > 0 && vy < jMax  ) { 
                                u = left.m_data[ux][uy];
                                v = right.m_data[vx][vy];
                                cd += -(u-v)*(u-v);
                            }
                            
                        }
                    if (cdMax < cd) 
                    {
                        d1Max = d1;
                        d2Max = d2;
                        cdMax = cd;
                    }
                }
            ID.insert (i,j,d1Max, d2Max, cdMax);
        }
    cout << "Raw Correspondence Computed " << endl;
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


void correlate::getImagePGM2 (const char* filename, matrixType &matType)
{

	ifstream infile(filename);
        char temp;
        int tempInt;
        int rows;
        int cols;
        char buf[80];
        //string buf;
        double greyLevel;
        double value;
        
        
	if (infile.bad()) {
		cout <<"readPGM: Unable to open file! " ;
                cout << filename;
                    exit (1);
                }
	/*infile.getline (buf, 5, '\n');
	if (buf != "P2"){
		cout <<"readPGM: magic number is not P5!";
                exit (1);
                }
	*/
        infile >> temp;
        infile >> temp;
        cout << temp;
        tempInt = 0;
            infile >> tempInt;
            cout << tempInt;
            //}
        cols = tempInt;
        
	//infile >> temp;
        tempInt = 0;
            infile >> tempInt;
            //}
        rows = tempInt;
        
	//infile >> temp;
        tempInt = 0;
            infile >> tempInt;
            
            //}
        greyLevel = tempInt;
        matType.NUMBER_OF_GREY_LEVELS = tempInt;
        matType.m_rows=rows;
        matType.m_cols= cols;
        matType.allocateStorage();
        
        for ( int i = 0; i < rows; i++) 
            for (int j = 0; j < cols; j++) {
                infile >> tempInt;
                value = tempInt/greyLevel;
                matType.m_data[i][j] = value;
        }
	
	if (infile.bad()){
		cout << "Unable to read entire file!";
                exit (1);
                }
	infile.close();
        cout << "Read the file";
}

bool correlate::saveImagePGM2 (const char * filename, matrixType &matType)
{
	int i;
	ofstream outfile(filename);
	outfile << "P2" << endl;
	outfile << matType.m_cols << " " << matType.m_rows << endl;
	outfile << 255 << endl; //this is 255
	
	for (int r=0; r<matType.m_rows; r++) {
		for (int c=0; c<matType.m_cols; c++)
                {
                    i = ceil (matType.m_data[r][c] * 255);
                    if ( i > 255) i = 255;
                    if (i < 0 ) i = 0;
                    outfile << i;
                    outfile << " ";
                }
                outfile << endl;
            }

	outfile.close();
        return 1;
}


void correlate::computeWorldPoints ()
{
    cout << "Beginning World Point Calculations" << endl;
    
    int iMax = ID.getRowSize();
    int jMax = ID.getColSize();
    matrixType pl (3,1);
    matrixType pr (3,1);
    matrixType world (3,1);
    worldPoint worldP;
    double tempI;
    double tempJ;
    double Int;
    int i;
    int j;
    int tI, tJ;
    
    for (i = 0; i < iMax; i++)
    {    for (j = 0; j < jMax; j++)
        {
            
            tempI = i;
            tempJ = j;
            pl.m_data[0][0] = tempI;
            pl.m_data[1][0] = tempJ;
            pl.m_data[2][0] = 1.0;
            tI = i + ID.getD1(i,j);
            tJ = j + ID.getD2(i,j);
            tempI = tI;
            tempJ = tJ;
            pr.m_data[0][0] = tempI;
            pr.m_data[1][0] = tempJ;
            pr.m_data[2][0] = 1.0;
            cout << i << "," << j << " ";
            world = computePoint( pl,pr);
            //Int = ID.getI();
            Int = 0.5;
            worldP.x = world.m_data[0][0];
            worldP.y = world.m_data[1][0];
            worldP.z = world.m_data[2][0];
            
            worldP.I = Int;
            setWorldPoint(i,j, worldP);
        }
        cout << "\n";
    }
}

matrixType& correlate::computePoint ( matrixType &leftPoint, matrixType &rightPoint)
{
    matrixType tempVectorA (3,1);
    matrixType tempVectorB (3,1);
    matrixType tempVectorC (3,1);
    matrixType tempMatrix (3,3);
    matrixType A(3,3);
    matrixType AT(3,3);
    matrixType ATA(3,3);
    matrixType ATAinv (3,3);
    matrixType xTemp;
    matrixType estLeft(3,1);
    matrixType estRight(3,1);
    matrixType point (3,1);
    //worldPoint onePoint;
    double alpha = fleft; 
    double beta = fright;
    double a,b;
    double tempA, tempB, tempC;
    int i,j;
    
   // set up matrix A
    for ( i = 0; i < 3; i++)
        for ( j = 0; j < 3; j++)
            tempMatrix.m_data[i][j] = R.m_data[j][i];
    
    //cout << "M" ;
    tempVectorA = alpha * leftPoint;
    //cout << "N";
    cout << "Temp Matrix" ;

    tempVectorB = beta * ( tempMatrix * rightPoint);
    //cout <<"O" ;
    tempVectorC = cross(tempVectorA, tempVectorB);
    //cout << "P";
    for ( i = 0; i < 3; i++)
        {
            //tempA = tempVectorA.getValue(i,0);
           // tempB = tempVectorB.getValue(i,0);
            //tempC = tempVectorC.getValue(i,0);
            A.m_data[i][0] =  tempVectorA.m_data[i][0];
            A.m_data[i][1] = tempVectorB.m_data[i][0];
            A.m_data[i][2] = tempVectorC.m_data[i][0];
        }
        
     for ( i = 0; i < 3; i++)
        for ( j = 0; j < 3; j++)
            AT.m_data[i][j] = A.m_data[j][i];
    //AT = A.transpose();
    ATA = AT * A;
    ATAinv = computeInverseGJ(ATA);
    xTemp = ATAinv * AT * T;
    cout << "A";
    
    a = xTemp.m_data[0][0]; //* alpha;
    b = xTemp.m_data[0][1]; // * beta;
    estLeft = (a * leftPoint) * (1/ alpha);
    estRight = (b * tempMatrix * rightPoint) * (1/ beta);
    point = 0.5 * (estLeft + estRight);
    
    return point;
}

matrixType& correlate::cross ( matrixType &left, matrixType &right)
{
    matrixType temp(3,1);
    double x,y,z;
    z = left.m_data[0][0] * right.m_data[1][0];
    z -= left.m_data[1][0] * right.m_data[0][0];
    y = left.m_data[2][0] * right.m_data[0][0];
    y -= left.m_data[0][0] * right.m_data[2][0];
    x = left.m_data[1][0] * right.m_data[2][0];
    x -= left.m_data[2][0] * right.m_data[1][0];
    temp.m_data[0][0] = x;
    temp.m_data[1][0] = y;
    temp.m_data[2][0] = z;
    return temp;
}

void correlate::setWorldPoint (int i, int j, worldPoint &aPoint)
{
    wp[i][j].x = aPoint.x;
    wp[i][j].y = aPoint.y;
    wp[i][j].z = aPoint.z;
    wp[i][j].I = aPoint.I;
    wp[i][j].next = aPoint.next;
    wp[i][j].prev = aPoint.prev;
}


matrixType& correlate::computeInverseGJ (matrixType &a)
{
    int i,j,k;
    double l, temp;
    int n = a.m_rows;
    int row = n;
    int col = n*2;
    
    matrixType tinv(row,col);
    matrixType inv(row,row);
    
    cout << "Allocated TINV\n";
    for (i = 0; i<n; i++)
        for (j = 0; j<n ; j++)
            tinv.m_data[i][j]=a.m_data[i][j];
    
    cout << "Assigned TINV\n";
    for (i = 0 ; i < row; i++)
        for ( j = n; j < col; j++)
            tinv.m_data[i][j] = 0;
    cout << "Assigned TINV\n";
    i = 0;    
    for (j = n ; j < col; i++, j++)
        tinv.m_data[i][j] = 1;
    cout << "Assigned TINV\n";
    
    for ( i = 0; i < row; i++)
    {
        k = i+1;
        while ((k < row) && (tinv.m_data[k][i] == 0) )
            k++;
       if ( k < row)
            for ( j = i; j < col; j++)
            {
                temp = tinv.m_data[i][j];
                tinv.m_data[i][j] = tinv.m_data[k][j];
                tinv.m_data[k][j] = temp;
            }
        l = 1/tinv.m_data[i][i];
        for (j = i +1; j < col; j++)
            tinv.m_data[i][j] *= l;
        tinv.m_data[i][i] = 1.0;
        for ( k = i + 1; k < row; k++)
        {
            l = tinv.m_data[k][i];
            for ( j = i +1; j < col; j++)
                tinv.m_data[k][j] -= l * tinv.m_data[i][j];
            tinv.m_data[k][i] = 0.0;
        }  // end of for k
        
    } // end of for i
    for ( i = row -1; i >= 0; i--)
            for ( k = i-1; k >= 0; k--)
            {
                l = tinv.m_data[k][i];
                for ( j = i; j < col; j++)
                    tinv.m_data[k][j] -= l * tinv.m_data[i][j];
                tinv.m_data[k][i] = 0.0;
            } // end of for k
    for ( i = 0; i < n; i++)
        for ( j = 0; j < n; j++)
            inv.m_data[i][j] = tinv.m_data[i][j+n];
  return inv;
    
} // end of Compute Inverse GJ

bool correlate::allocateWorldPoints (int rows, int cols)
{
    w_rows = rows;
    w_cols = cols;
  if (w_rows <=0 || w_cols <=0) {
    wp = NULL;
    return false;
  }
  wp = new worldPoint*[w_rows];
  wp[0] = new worldPoint[w_rows*w_cols];
  for (int i=1; i<w_rows; i++)
    wp[i] = wp[i-1]+w_cols;
  return true;
}

void correlate::produceVRML (const char* filename)
{
  int rows = w_rows;
  int cols = w_cols;
  int r, c;
  int count;

  //void createPointIdIndex

  //first set size
  double **id;
  //allocate (rows, cols, id);
  
    id = new double*[rows];
    id[0] = new double[rows*cols];
    for (int i=1; i<rows; i++)
        id[i] = id[i-1]+cols;
    count = 0;
    for (r=0; r<rows; r++)
        for (c=0; c<cols; c++) {
            id[r][c] = count++;
    }
  
  //start producing VRML file
  ofstream outfile(filename);

  //create header
  outfile << "#VRML V2.0 utf8" << endl;
  outfile << endl;
  outfile << "# Produced by createVRMLfromGRID" << endl;
  outfile << endl;

  //develop beginning group information
  outfile << "Group { " << endl;
  outfile << "  children [" << endl;
  outfile << "    Shape {" << endl;
  outfile << "      appearance Appearance {" << endl;
  outfile << "        material Material {" << endl;
  outfile << "          diffuseColor 0 0.0498 1" << endl;
  outfile << "        }" << endl;
  outfile << "      }" << endl;
  outfile << "      geometry IndexedFaceSet {" << endl;
  outfile << "        ccw TRUE" << endl;
  outfile << "        solid TRUE" << endl;
  outfile << "        convex TRUE" << endl;

  //output points
  outfile << "        coord Coordinate{ " << endl;
  outfile << "          point [" << endl;
  for (r=0; r<rows; r++)
    for (c=0; c<cols; c++) {
      //      if (mask[r][c] == true) {
      outfile << "             ";
      outfile << wp[r][c].x << " ";
      outfile << wp[r][c].y << " ";
      outfile << wp[r][c].z << ", " << endl;
    }
  outfile << "          ] " << endl;
  outfile << "        } " << endl;
 

  //now indices
  outfile << "        coordIndex [ " << endl;
  for (r=0; r<rows-1; r++)
    for (c=0; c<cols-1; c++) {
      outfile << "          ";
      outfile << id[r][c] << ", ";
      outfile << id[r][c+1] << ", ";
      outfile << id[r+1][c+1] << ", ";
      outfile << id[r+1][c] << ", ";
      outfile << "-1, " << endl;
    }
  outfile << "        ]" << endl;

  //now close everything
  outfile << "      } " << endl;
  outfile << "    } " << endl;
  outfile << "  ]" << endl;
  outfile << "}" << endl;

  //the end

}

