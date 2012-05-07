/*
 *  calType.cpp
 *  math02
 *
 *  Created by Daniel Beatty on Sun Aug 04 2002.
 *  Copyright (c) 2002 __MyCompanyName__. All rights reserved.
 *
 */

#include "calType.h"
#include <math.h>
//#include <mathlink.h>

bool calType::openFile (const char * filename)
{
    input.open (filename);
    //buffer = new char[128];
    return (open = input.good());
}

bool calType::closeFile ()
{
    if (open) {
        input.close();
        return true;
    } 
    return false;
}

bool calType::readLine()
/*
 Input: Filename of points file
        points are of the form
        Xw Yw Zw Xi Yi
    Output:  Project matrix filled with points
    Purpose: Read one line of points.  Call parseline, and return its value.
*/
{
    bool success;
    buflen = 128;
    buffer = new char[buflen];
    if (input.eof())
        return false;
    input.getline (buffer, buflen, '\n');
    
    cout << buffer;
   // cout << sizeof(buffer);
    success = parseLine ();
    
   // cout << " :";
   // cout << " Xw = " << Xw ;
   // cout << " Yw = " << Yw ;
   // cout << " Zw = " << Zw ;
   // cout << " Xi = " << Xi ;
   // cout << " Yi = " << Yi ;
   // cout << endl;
    //ifstream infile();
    delete []buffer;
    return success;
}

bool calType::parseLine ()
/*
    Input: Filename of points file
        points are of the form
        Xw Yw Zw Xi Yi
    Output:  Project matrix filled with points
    Purpose: Parse on line of points.  If success report true, otherwise report false.
*/

{
    i = 0;
    if ( getdouble (Xw) )
        if ( getdouble (Yw))
            if (getdouble (Zw))
                if (getdouble (Xi))
                    if ( getdouble(Yi))
                        return true;
    return false;
}

bool calType::parseVLine ()
/*
    Input: Text file line
    Output: Status of Parsing
    Purpose: Read in a V Line
*/
{
    buffer = new char[buflen];
    if (input.eof())
        return false;
    input.getline (buffer, buflen, '\n');
    
    cout << buffer;
    
    v = new double[12];
    double temp;
    int index;
    cout << "\n Regurgitate\n";
    for (index = 0; index < 12; index++)
    {
        if (getdouble(temp))
        {
            v[index] = temp;
            cout << temp;
            cout << " ";
        } else
            return false;
    }
    return true;
}


bool calType::getdouble (double &value)
/*
    Input: Pointer to the real number value
    External Dependence:  buffer
                            buffer length;
    Output:  The real value 
    Purpose: Read in a real number of limited precision.  
*/
{
    if ( i >= buflen) return false;
    char ch;
    double temp=0;
    double exp = 0.0;
    bool decimal = false;
    double plus;
    double sign = 1;
    bool number = false;
    //int i = 0;
    bool stateRepeat = true;
    do {
        ch = buffer[i++];
        
        switch (ch)
        {
            case '0': 
                        plus = 0;
                        number = true;
                        stateRepeat = true;
                        temp = temp * 10 + plus;
                        if (decimal)
                            exp--;
                        break;
            case '1': 
                        plus = 1;
                        number = true;
                        stateRepeat = true;
                        temp = temp * 10 + plus;
                        if (decimal)
                            exp--;
                        break;
            case '2': 
                        plus = 2;
                        number = true;
                        stateRepeat = true;
                        temp = temp * 10 + plus;
                        if (decimal)
                            exp--;
                        break;
            case '3': 
                        plus = 3;
                        number = true;
                        stateRepeat = true;
                        temp = temp * 10 + plus;
                        if (decimal)
                            exp--;
                        break;
            case '4': 
                        plus = 4;
                        number = true;
                        stateRepeat = true;
                        temp = temp * 10 + plus;
                        if (decimal)
                            exp--;
                        break;
            case '5': 
                        plus = 5;
                        number = true;
                        stateRepeat = true;
                        temp = temp * 10 + plus;
                        if (decimal)
                            exp--;
                        break;
            case '6': 
                        plus = 6;
                        number = true;
                        stateRepeat = true;
                        temp = temp * 10 + plus;
                        if (decimal)
                            exp--;
                        break;
            case '7': 
                        plus = 7;
                        number = true;
                        stateRepeat = true;
                        temp = temp * 10 + plus;
                        if (decimal)
                            exp--;
                        break;
            case '8': 
                        plus = 8;
                        number = true;
                        stateRepeat = true;
                        temp = temp * 10 + plus;
                        if (decimal)
                            exp--;
                        break;
            case '9': 
                        plus = 9;
                        number = true;
                        stateRepeat = true;
                        temp = temp * 10 + plus;
                        if (decimal)
                            exp--;
                        break;
            case '.' :
                decimal = true;
                exp = 0.0;
                break;
            case '\0':
                        stateRepeat = false;
                        break;
            case '-' :	
                    if (number) 
                        {
                            number = false;
                            stateRepeat = false;
                        } else
                        {
                            number = false;
                            stateRepeat = false;
                            sign = -1;
                        }
            case ' ' :
            case '\t': 
                        if ( number)
                            stateRepeat = false;
                            else 
                            stateRepeat = true;
                        break;
            default:
                        number = false;
                        stateRepeat = false;
        }
        if ( i >= buflen ) stateRepeat = false;
    }while (stateRepeat);
    if (number)
    {
        value = sign *temp;
        if (decimal)
            value *= pow(10,exp);
        return true;
    } 
        value = 0;
        return false;
}

void calType::setBuffer (const char* test)
/*
    
*/
{
    int i, imax = strlen (test);
    delete []buffer;
    buffer = new char [128];
    for ( i = 0; i < imax; i++)
        buffer[i] = test[i];
}


void calType::getPointsFromFile (const char* filename)
/*
    Input: Filename of points file
        points are of the form
        Xw Yw Zw Xi Yi
    Output:  Project matrix filled with points
*/
{
    
    double X,Y,Z,x,y;
    //char ch[10];
    ifstream infile(filename);
    projection = new matrixType (40,12);
    //Amatrix = temp;
    //bool empty = true;
    int i = 0;
    cout << "Obtaining data from file " << filename << " " << endl;
    openFile (filename);
    while (readLine() && (i < 40))
    {
        //X = ngetXw();
        //Y = num->getYw();
        //Z = num->getZw();
        //x = num->getXi ();
        //y = num->getYi ();
        setWorldImage(Xw,Yw,Zw,Xi,Yi,i);
    }
}


void calType::setWorldImage (double X, double Y, double Z, double x, double y, int &i) 
/*
    Purpose is to insert a set of values into a projection matrix.
*/
{
    int iplus = i +1;
    projection->m_data[i][0] = projection->m_data[iplus][4] = X;
    projection->m_data[i][1] = projection->m_data[iplus][5]= Y;
    projection->m_data[i][2] = projection->m_data[i][6] = Z;
    projection->m_data[i][3] = projection->m_data[i][7]= 1;
    projection->m_data[i][4] = projection->m_data[iplus][0]= 0;
    projection->m_data[i][5] = projection->m_data[iplus][1] = 0;
    projection->m_data[i][6] = projection->m_data[iplus][2] = 0;
    projection->m_data[i][7] = projection->m_data[iplus][0] = 0;
    projection->m_data[i][8] = -X*x;
    projection->m_data[i][9] = -Y*x;
    projection->m_data[i][10] = -Z*x;
    projection->m_data[i][11] = -x;
    projection->m_data[iplus][8] = -X*y;
    projection->m_data[iplus][9] = -Y*y;
    projection->m_data[iplus][10] = -Z*y;
    projection->m_data[iplus][11] = -y;
    i += 2;
}


void calType::computeParameter ()
/*
    Input from class:  V matrix computed from an SVD operation
    Output: none
    Purpose: Calcuate intrinsic and extrinsic parameters from the null
        matrix of a SVD calcuation on the Projection matrix.
*/
{
    double gamma;
    double sigma = 1;
    double temp;
    double tz;
   // double *tempMatrix;
    //m.getRow(11,tempMatrix);
    double tempMatrix[12];
    int i;
    int row = VT.m_rows - 1;
    int col = VT.m_cols;
    /*if ( T != NULL)
        delete T;
    if ( R != NULL)
        delete R;
        */
    T.allocateStorage(3,1);
    R.allocateStorage(3,3);
    
    for (i =0; i< col; i++)	//tempMatrix[i] =
        tempMatrix[i] = VT.m_data[row][i];
    temp = tempMatrix[8] * tempMatrix[8];
    temp += tempMatrix[9] * tempMatrix[9];
    temp += tempMatrix[10] * tempMatrix[10];
    gamma = sqrt (temp);
    temp = tempMatrix[11] / gamma;
    if (temp < 0 ) 
            {
                temp = -temp;
                gamma = -gamma;
                sigma = -1;
            }
    tz = temp;
    
    
    ox = tempMatrix[0] * tempMatrix[8] + tempMatrix[1] * tempMatrix[9] + tempMatrix[2] * tempMatrix[10];
    oy = tempMatrix[4] * tempMatrix[8]+ tempMatrix[5] * tempMatrix[9] + tempMatrix[6] * tempMatrix[10];
    
    temp = tempMatrix[0] * tempMatrix[0];
    temp += tempMatrix[1] * tempMatrix[1];
    temp += tempMatrix[2] * tempMatrix[2];
    temp += ox * ox;
    if (temp < 0) {
        temp = - temp;
    }
    fx = sqrt (temp);
    temp = tempMatrix[4] * tempMatrix[4];
    temp += tempMatrix[5] * tempMatrix[5];
    temp += tempMatrix[6] * tempMatrix[6];
    temp += oy * oy;
    if (temp < 0) {
        temp = - temp;
    }
    fy = sqrt (temp);
    for ( int i = 0; i < 3; i++)
        {
            temp = sigma * (ox*tempMatrix[8+i] - tempMatrix[i])/fx;
            R.m_data[0][i] = temp;
            temp = sigma * (oy*tempMatrix[8+i] - tempMatrix[4+ i])/fy;
            R.m_data[1][i] = temp;
            temp = tempMatrix[8+i] / gamma;
            R.m_data[2][i] = temp;
        }
    temp = sigma * ( ox * tz - tempMatrix[3]); 
    
    T.m_data[0][0] = temp;
    temp = sigma * (oy * tz - tempMatrix[7]);
    T.m_data[1][0] = temp;
    T.m_data[2][0] = tz;
    setUpIntrinsicMatrix ();
    setUpExtrinsicMatrix ();
    
    cout << "Parameters computed " << endl;
    
    
}

void calType::setUpIntrinsicMatrix ()
/*
    Input: None external 
        from class: fx,fy,ox,oy
    Output: The intrinsic matrix
    Purpose:  Insert the intrinsic parameters into the intrinsic matrix
*/
{
    
    Mi.allocateStorage(3,3);
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            Mi.m_data[i][j] = 0;
        }
        
    }
    Mi.m_data[2][2] = 1.0;
        
    Mi.m_data[0][0] = fx;
    Mi.m_data[1][1] = fy;
    Mi.m_data[0][2] = ox;
    Mi.m_data[1][2] = oy;
    cout << "Intrinsic Parameters set " << endl;
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
            cout << Mi.m_data[i][j] << " ";
        cout << endl;
    }
    
}

void calType::setUpExtrinsicMatrix ()
/*
    Input: none external
        from class:  R matrix, T vector
    Output:  Extrinsic Parameters
    Purpose: Insert R matrix and Dot(Rx,T) values into the
        extrinsic parameters
*/
{
    //matrixType *temp = new matrixType (1,3);
    Mx.allocateStorage(3,4);
    double temp;
    double temp2;
    
    int i,j;
    
    for (i=0; i < 3; i++)
    {
        temp2 = 0;
        temp = 0;
        
        for (j=0; j < 3; j++)
        {
            temp = R.m_data[i][j];
            Mx.m_data [i][j] = temp;
            temp2 += T.m_data[j][0] * temp;
            
        }
        Mx.m_data[i][3] = temp2;
    }
    cout << "Extrinsic Parameters set " << endl;
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 4; j++)
            cout << Mx.m_data[i][j] << " ";
        cout << endl;
    }
}

double calType::computeFocalLength ()
/*
    Input: No outside input.  
    Output: The value of the focal length
    Purpose:  Computes the focal length of camera system based on already
        computed rotation matrix, translation vectors and intrinsic parameters.
*/
{
    int i;
    double Zc;
    double Zw;
    double Zi;
    double TTrans, TTransR;
    double tiny = pow ((double) 10, -20);
    double tempT;
    double tempT2;
    matrixType tempM(3,1) ; //= new matrixType(3,1);
    matrixType tempM2;
    matrixType worldOriginPrime (4,1) ; //= new matrixType(4,1);
    matrixType worldOrigin (3,1); //= new matrixType(3,1);
    matrixType cameraOrigin (3,1); //= new matrixType(3,1);
    matrixType imageOrigin(3,1); // = new matrixType(3,1);
    
    for (i=0; i < 3; i++)
    {
        worldOrigin.m_data[i][0] = 50.0;
        worldOriginPrime.m_data[i][0] = 50.0;
    }
    worldOriginPrime.m_data[3][0] = 1.0;
    
    tempM = (Mi * Mx);
    imageOrigin = tempM * worldOriginPrime;
    Zi = imageOrigin.m_data[2][0];
    //Zc= cameraOrigin.getValue (2,0);
    cout << "\nZi = " << Zi << "\n";
    imageOrigin = imageOrigin * (1/ Zi);
    //Zw = -1 * T.getValue(0,2);
    
    
    //tempM2 = T.transpose();
    //tempM = T * tempM2;
    tempM= worldOrigin - T;
    cameraOrigin = R * tempM;
    tempT = imageOrigin.m_data[0][0]/cameraOrigin.m_data[0][0];
    tempT2 =  imageOrigin.m_data[1][0]/cameraOrigin.m_data[1][0];
    Zc = cameraOrigin.m_data[2][0];
    f = Zc * (tempT + tempT2)/2;
    cout << "Focal Length is " << f << endl;
    
}
/*
void calType::computeSVD (int argc, const char * argv[])
{
    MLENV env;
    MLINK link;
    long n;
    int i,j, c;
    double *results;
    c = 0;
    int rows = 4; //projection->m_rows;
    int cols = 3; //projection->m_cols;
    long n1, n2, n3;
    long dims[] = {rows, cols}; 
    long *dimR;
    double *a = new double [rows*cols];
    //a = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
    for ( i = 0; i < rows; i++)
        for (j = 0; j < cols; j++)
            a[c++] = c;  //projection->m_data[i][j];
    long errno;
    char *param = "-linkprotocol local -linkconnect /usr/bin/MathKernel -mathlink";
    env = MLInitialize(0);
    link = MLOpenString( env, param, &errno);
    
    MLActivate(link);
    
    MLPutFunction (link, "SingluarValues", 1);
    MLPutFunction (link, "N",1);
    MLPutFunction (link, "List", rows);
    for ( i = 0; i < rows; i++)
        MLPutRealList(link, a + (i*cols), cols);
   // MLPutRealArray(link, a, dims, NULL,
    MLCheckFunction (link, "List", &n);
    MLGetRealArray (link, &results, &dimR, NULL, &n);
    cout << endl;
    cout << n << "\t";
    cout << &n << "\t";
    //cout << *n << "\t";
    for ( i = 0; i < 3 ; i++)
        cout << dimR[i] << "\t";
    cout << endl;
    
}
*/
/*
extern "C" void dgetrf_(
        const int &m,                   // (input)
        const int &n,                   // (input)
        double *a,                      // a[n][lda] (input/output)
        const int &lda,                 // (input)
        int *ipiv,                      // ipiv[min(m,n)] (output)
        int &info                       // (output)
        );

extern "C" void dgetri_(
        const int &n,                   // (input)
        double *a,                      // a[n][lda] (input/output)
        const int &lda,                 // (input)
        const int *ipiv,                // ipiv[n] (input)
        double *work,                   // work[lwork] (workspace/output)
        const int &lwork,               // (input)
        int &info                       // (output)
        );

extern "C" void dgesvd_(
        const char &jobu,               // (input)
        const char &jobvt,              // (input)
        const int &m,                   // (input)
        const int &n,                   // (input)
        double *a,                      // a[n][lda] (input/output)
        const int &lda,                 // (input)
        double *s,                      // s[min(m,n)] (output)
        double *u,                      // u[ucol][ldu] (output)
        const int &ldu,                 // (input)
        double *vt,                     // vt[n][ldvt] (output)
        const int &ldvt,                // (input)
        double *work,                   // work[lwork] (workspace/output)
        const int &lwork,               // (input)
        int &info                       // (output)
        );

void calType::computeSVD ()
{
    int rows = projection.m_rows;
    int cols = projection.m_cols;
    int mirc, mxrc, lwork, lworka, lworkb ;
    if ( rows > cols) 
        {
            mirc = cols;
            mxrc = rows;
        }
        else
        {
            mirc = rows;
            mxrc = cols;
        }
    
    lworka = 3*mirc+mxrc;
    lworkb = 5 * mirc;
    if ( lworka > lworkb) 
                lwork = lworka;
                else
                lwork = lworkb;
    int i,j, info;
    int k = 0;
    double *A = new double [rows * cols];
    double *S = new double [rows * cols];
    double *U2 = new double [rows * mirc];
    double *VT2 = new double [mirc * cols];
    double *WORK = new double [lwork];
    for (i = 0; i< rows; i++)
        for (j=0; i<cols; j++)
            A[k++] = projection.m_data[i][j];
    dgesvd_( 'S', 'S', m_rows, m_cols, A, m_rows, S, U2, m_rows, VT2, ldvt, WORK, lwork, info );
    
    VT = matrixType (ldvt, cols);
    k = 0;
    for ( i = 0; i < ldvt; i++)
        for (j = 0; j < cols; j++)
            VT.m_data[i][j] = VT2[k++];
    
    delete []A;
    delete []U2;
    delete []S;
    delete []VT2;
    delete []WORK;  
    
}
*/
