#include "xformpsin.hpp"
#include "queuearray.hpp"
#include "haarGen.hpp"
#include <iostream.h>
#include <stddef.h>
#include <math.h>
#include <vector>
#include <algorithm>
using namespace std;


bool xformpsin::epsilon ( double x, int th)
{
    return epsilon (x, (double) th);
}

bool xformpsin::epsilon ( double x, double thresh)
{
    //cerr << " In epsilon \n";
    pm.totaldatacompares++;
    pm.totalinstructions ++;
    pm.totaldatatransfers ++;
    if ( fabs(x) > thresh) return false;
    return true;
}


int xformpsin::findX2 (int i)
{
    int r = i;
    int count = 0;
    int even = 1;
    while ( (r != 0) & ((r % 2) ==0) )
    {
        r = r /2;
        count++;
        pm.totaldatacompares++;
        pm.totalinstructions +=2;
        pm.totaldatatransfers ++;
    }
    
    pm.totalinstructions +=3;
    return count;
}

void xformpsin::getSstats (quadnode &r)
{
    double sum = 0.0;
    double sum2 = 0.0;
    double max = 0.0;
    double min = pow (10.0,37.0);
    int i, j;
    for ( i = r.xul; i < r.xll; i++)
        for ( j = r.yul; j < r.yur; j++)
        {
            if ( max <= S[i][j]) max = S[i][j] ;
            if ( min >= S[i][j]) min = S[i][j];
            //sum += S[i][j];
            sum2 += S[i][j] * S[i][j];
            pm.totalinstructions += 4;
            pm.totaldatatransfers += 4;
        }
            r.max = max;
    r.min = min;
    //r.L1energy = sum;
    r.L2energy = sum2;
    
    pm.totalinstructions += 8;
    pm.totaldatatransfers += 8;
}


void xformpsin::getNode (quadnode &r)
{
    r.xul = qtree[current].xul;
    r.xll = qtree[current].xll;
    r.yul = qtree[current].yul;
    r.yll = qtree[current].yll;
    r.xur = qtree[current].xur;
    r.xlr = qtree[current].xlr;
    r.yur = qtree[current].yur;
    r.ylr = qtree[current].ylr;
    r.rows = qtree[current].rows;
    r.cols = qtree[current].cols;
    pm.totalinstructions += 15;
    pm.totaldatatransfers += 15;
}

void xformpsin::generateTree ()
{
    int row = S.m_rows;
    int col = S.m_cols;
    int res = (int) (log((double)resolution)/log(4.0));
    int beginingrow;
    int i, j;
    int hspot, dspot, aspot, vspot;
    //treesize = (int) pow (4,resolution) + 1;
    treesize = resolution + 1;
    int base = (int) pow ((double)2, (double)res);
    int baserow = row / (int) pow ((double)2, (double)res);
    int basecol = col / (int) pow ((double)2, (double)res);
    qtree = new quadnode[treesize];
    int ares = 1;
    qtree[0].id = -1;
    qtree[1].id = aval = 0;
    qtree[2].id = vval = 1;
    qtree[resolution + 1].id = hval = 2;
    qtree[resolution + 2].id = dval = 3;
    int rc, cc;
    rc = cc = 2;
    //cerr << "In the process of generating a tree. Size " << treesize << "\n";
    for ( ares = 2; ares < res; ares++)
    {
        for ( i = 0; i < rc; i++)
            for ( j = 0; j < cc; j++)
            {
                aspot = resolution * (j ) + i +1;
                vspot = resolution * j + i + rc +1;
                hspot = resolution * (j + cc) + i +1;
                dspot = resolution * (j + cc) + rc + i +1;
                //qtree[aspot].id += aval * pow ((double) 4, (double) ares);
                qtree[vspot].id += qtree[aspot].id * (int)pow ((double)4,(double) ares);
                qtree[hspot].id += qtree[aspot].id * (int)pow ((double)4, (double)ares);
                qtree[dspot].id += qtree[aspot].id * (int)pow ((double)4, (double)ares);
                
            }
                
        for ( i = 0; i < rc; i++)
            for ( j = 0; j < cc; j++)
            {
                aspot = resolution * (j ) + i +1;
                vspot = resolution * j + i + rc +1;
                hspot = resolution * (j + cc) + i +1;
                dspot = resolution * (j + cc) + rc + i +1;
                qtree[aspot].id += aval * (int)pow ((double) 4, (double) ares);
                qtree[vspot].id += vval * (int)pow ((double) 4, (double) ares);
                qtree[hspot].id += hval * (int)pow ((double) 4, (double) ares);
                qtree[dspot].id += dval * (int)pow ((double) 4, (double) ares);
            }
        cc = cc * 2;
        rc = rc * 2;
    }
   // cerr << " First numbers done \n";
    for ( i = 0; i < treesize; i++)
        qtree[i].id ++;
    qtree[0].xul = qtree[0].xur = 0;
    qtree[0].yul = qtree[0].yll = 0;
    qtree[0].xll = qtree[0].xlr = row;
    qtree[0].yur = qtree[0].ylr = col;
    
    //cerr << "With our bases " << base << " base col " << basecol << " base row " << baserow  << "\n";
    for ( i = 0; i < res ; i++)
        for ( j = 0 ; j < res; j++)
        {
            aspot = i * (j * resolution) + 1;
            qtree[aspot].xul = qtree[aspot].xur = i * baserow;
            qtree[aspot].yul = qtree[aspot].yll = j * basecol;
            qtree[aspot].xll = qtree[aspot].xlr = (i + 1) * baserow -1;
            qtree[aspot].yur = qtree[aspot].ylr = (j + 1) * basecol -1;
        }
    //cerr << "Nearly done \n";
    
}

void xformpsin::generateTree (matrixType &A)
{
    int row = A.m_rows;
    int col = A.m_cols;
    int rres = findX2 (row);
    int cres = findX2 (col);
    if ( rres  < cres) resolution = rres;
    else resolution = cres;
    setmatrix (A);
    int beginingrow;
    int i, j;
    int hspot, dspot, aspot, vspot;
    treesize = (int) pow ((double)4,(double)resolution) + 1;
    int base = (int) pow ((double)2,(double) resolution);
    int baserow = row / (int) pow ((double)2, (double)resolution);
    int basecol = col / (int) pow ((double)2, (double)resolution);
    qtree = new quadnode[treesize];
    int ares = 1;
    qtree[0].id = -1;
    qtree[1].id = aval = 0;
    qtree[2].id = vval = 1;
    qtree[resolution + 1].id = hval = 2;
    qtree[resolution + 2].id = dval = 3;
    int rc, cc;
    rc = cc = 2;
    for ( ares = 2; ares < resolution; ares++)
    {
        for ( i = 0; i < rc; i++)
            for ( j = 0; j < cc; j++)
            {
                aspot = resolution * (j ) + i +1;
                vspot = resolution * j + i + rc +1;
                hspot = resolution * (j + cc) + i +1;
                dspot = resolution * (j + cc) + rc + i +1;
                //qtree[aspot].id += aval * pow ((double) 4, (double) ares);
                qtree[vspot].id += qtree[aspot].id * (int)pow ((double)4, (double)ares);
                qtree[hspot].id += qtree[aspot].id * (int)pow ((double) 4, (double) ares);
                qtree[dspot].id += qtree[aspot].id * (int)pow ((double) 4, (double) ares);
                
            }
                
                for ( i = 0; i < rc; i++)
                    for ( j = 0; j < cc; j++)
                    {
                        aspot = resolution * (j ) + i +1;
                        vspot = resolution * j + i + rc +1;
                        hspot = resolution * (j + cc) + i +1;
                        dspot = resolution * (j + cc) + rc + i +1;
                        qtree[aspot].id += aval * (int)pow ((double) 4, (double) ares);
                        qtree[vspot].id += vval * (int)pow ((double) 4, (double) ares);
                        qtree[hspot].id += hval * (int)pow ((double) 4, (double) ares);
                        qtree[dspot].id += dval * (int)pow ((double) 4, (double) ares);
                        
                    }
        
        cc = cc * 2;
        rc = rc * 2;
        
    }
    for ( i = 0; i < treesize; i++)
        qtree[i].id ++;
    qtree[0].xul = qtree[0].xur = 0;
    qtree[0].yul = qtree[0].yll = 0;
    qtree[0].xll = qtree[0].xlr = row;
    qtree[0].yur = qtree[0].ylr = col;
    
    for ( i = 0; i < base; i++)
        for ( j = 0 ; j < base; j++)
        {
            aspot = i * (j * resolution) + 1;
            qtree[aspot].xul = qtree[aspot].xur = i * baserow;
            qtree[aspot].yul = qtree[aspot].yll = j * basecol;
            qtree[aspot].xll = qtree[aspot].xlr = (i + 1) * baserow -1;
            qtree[aspot].yur = qtree[aspot].ylr = (j + 1) * basecol -1;
        }
}

void xformpsin::patchEnergy ()
{
    int i;
    for ( i = 0; i < treesize; i++)
        getSstats (qtree[i]);
}

void xformpsin::zeroize (quadnode &r)
{
    int i,j;
    for ( i = r.xul; i < r.xll; i++)
        for ( j = r.yul ; j < r.yur; j++)
            S[i][j] = 0.0;
    
}


void xformpsin::DarwinPatchFilter (double epsilon)  // Note epsilon is the percentage
{
    double L2epsilon, sum;
    int i;
    int threshlimit;
    patchEnergy();
    L2epsilon = qtree[0].L2energy * epsilon;
    sorttreeL2();
    i = treesize - 1; 
    while ( (i >= 0) && (sum < L2epsilon))
    {
        sum += qtree[i--].L2energy;
    }
    
    threshlimit = i - 1;
    for ( i = 0 ; i < threshlimit ; i++) zeroize (qtree[i]);
    
}



void xformpsin::DarwinFilter (double epsilon)
{
    int i, j, k;
    double sum;
    generateHistogram();
    while ( ( sum < epsilon) & ( k < histosize))
    {
        sum += histogram[k++];
    }
    
    for ( i = 0; i < S.m_rows; i++)
        for ( j = 0; j < S.m_cols; j++)
            if  (pow(S[i][j],2) < sum ) S[i][j] = 0.0;
    
}

void xformpsin::DarwinPatchFilter (matrixType &A, double epsilon)  // Note epsilon is the percentage
{
    setmatrix(A);
    double L2epsilon, sum;
    int i;
    int threshlimit;
    //cerr << "Generating Tree \n"; 
    generateTree();
    //cerr << "Getting patch energies \n";
    patchEnergy();
    L2epsilon = qtree[0].L2energy * epsilon;
    //cerr << " Sorting the patches\n ";
    sorttreeL2();
    i = treesize - 1; 
    while ( (i >= 0) && (sum < L2epsilon))
    {
        sum += qtree[i--].L2energy;
    }
    
    threshlimit = i - 1;
    for ( i = 0 ; i < threshlimit ; i++) zeroize (qtree[i]);
    getmatrix(A);
}



void xformpsin::DarwinFilter (matrixType &A, double epsilon)
{
    setmatrix(A);
    T.allocateStorage (S.m_rows, S.m_cols);
    matrixType integrity(S.m_rows, S.m_cols);
    int i, j, k;
    for ( i = 0; i < S.m_rows; i++)
        for ( j = 0; j < S.m_cols; j++)
            T[i][j] = S[i][j];
    double sum;
    //cerr << " Generate Histogram \n"; 
    double energy = S.getEnergy() * histosize;
    double epenergy = energy * epsilon ;//* S.m_rows * S.m_cols;
    double magic;
    generateHistogram();
    //cerr << "Generating magic number \n";
    //k = histosize -1 ;
    k = 0;
    sum = 0.0;
    //cerr << "\n";
    
    //while ( ( k < histosize) && ( sum < epenergy)  )
    //{
        //cerr << histogram[k] << "\t";
    //    sum += histogram[k++];
    //}
        k = (int) (epsilon * histosize);
    cerr << "Got magic number at " << k << " of " << histosize << "\n";
    //magic = fabs(histogram [k]);
    //magic = epenergy;
    magic = histogram[k];
    
    cerr << "Sum " << sum << " epenergy " << epenergy << " Magic number " << magic << " and the total energy " << energy << "\n";
    for ( i = 0; i < S.m_rows; i++)
        for ( j = 0; j < S.m_cols; j++)
            if  (pow(S[i][j], 2.0) < magic ) S[i][j] = 0.0;
    getmatrix(A);
    integrity = S - T;
    cerr << " The integrity of the new matrix is " << integrity.getEnergy() << "\n";
    
}



void xformpsin::generateHistogram ()
{
    
    int i, j, pos;
    histosize = S.m_rows * S.m_cols;
    histogram = new double [histosize];
    vector <double>elements (S.m_rows * S.m_cols);
    cerr << " Histogram allocated "<< histosize << " = " << S.m_rows << " by" << S.m_cols << "\n";
    pos = 0;
    for ( i = 0; i < S.m_rows; i++)
        for ( j = 0; j < S.m_cols; j++)
        {
            //pos = j + i * S.m_cols;
            elements[pos++] = pow(S[i][j], 2);
        }
            cerr << " Histogram arranged \n" ;
    // merge sort doubles:  
            //sorthistogram ();
    sort (elements.begin(), elements.end());
    for ( i = 0; i < histosize; i++)
        histogram[i] = elements[i];
    cerr << " Histogram allocated \n"; 
}

void xformpsin::swap ( quadnode &l, quadnode &r)
{
    quadnode temp;
    temp.copy (r);
    r.copy (l);
    l.copy (temp);
}


void xformpsin::swap ( double &l, double &r)
{
    double temp;
    temp = r;
    r = l;
    l = temp;
}

void xformpsin::sorttreewp ()
{
    // Selection sort on node.id
    int i, j, l, r, min;
    l = 0;
    r = treesize - 1;
    for ( i = l ; i < r; i++)
    {
        min = i;
        for ( j = i +1; j <= r; j++)
            if (qtree[j].id < qtree[min].id ) min = j; 
        swap (qtree[i], qtree[min]);
    }
}


void xformpsin::sorttreepsin ()
{
}


void xformpsin::sorttreeL2 ()
{
    // Selection sort on node.id
    int i, j, l, r, min;
    l = 0;
    r = treesize - 1;
    for ( i = l ; i < r; i++)
    {
        min = i;
        for ( j = i +1; j <= r; j++)
            if (qtree[j].L2energy < qtree[min].L2energy ) min = j; 
        swap (qtree[i], qtree[min]);
    }
    
}

void xformpsin::sorthistogram ()
{
    // Selection sort on node.id
    int i, j, l, r, min;
    l = 0;
    r = histosize - 1;
    cerr << " Sorting Histogram \n ";
    for ( i = l ; i < r; i++)
    {
        min = i;
        for ( j = i + 1; j <= r; j++)
            if (histogram[j] < histogram[min] ) min = j; 
        swap (histogram[i], histogram[min]);
    }
    
}


/*
void xformpsin::setAnode (quadnode &r, quadnode &a);
void xformpsin::setVnode (quadnode &r, quadnode &a);
void xformpsin::setHnode (quadnode &r, quadnode &a);
void xformpsin::setDnode (quadnode &r, quadnode &a);
*/


void xformpsin::selfWXform (matrixType &source, matrixType &result, int resol)
{
    
    setmatrix (source);int rows = S.m_rows;
    int cols = S.m_cols;
    int lr = findX2 (rows);
    int lc = findX2 (cols);
    int i;
    quadnode r;
    int res;
    if ( lr > lc ) res = lc;
    else 
        res = lr;
    cerr << " \n The resolution in the self WX form is " << res << " \n";
    //res = res / 2 -1;
    res = resol;
    int vertices = (int) pow ((double) 4, (double) res);
    resolution = vertices;
    
    cerr << " \n The vertices in the self WX form is " << vertices << " \n";
    qtree = new quadnode;
    //queue = new queuearray(vertices + 1);
    //qtree->initialize (S);
    //generateTree();
    //cerr << " Quad Tree Initalized \n";
    
     r.isnotnull = 1;
     r.rows = rows;
     r.cols = cols;
     r.xul = r.xur = 0;
     r.yul = r.yll = 0;
     r.xlr = r.xll = rows;
     r.ylr = r.yur = cols;
     r.id = 0;
     getSstats(r);
     r.length = 0;
    
    //getStats (qtree[0]);
    cerr << " Stats taken \n";
    //qtree->getNode (0, r);
    qtree[0] = r;
    xTemp.allocate(cols);
    yTemp.allocate(rows);
    xTemp2.allocate(cols);
    yTemp2.allocate(rows);
    pm.totalinstructions += 2 * (rows + cols);
    
    cerr << " X and Y vectors allocated \n";
    for ( i = 0; i < res; i++)
    {
        selfRowWXform(r);
        selfColWXform(r);
    }
    getmatrix (result);
    
}




void xformpsin::selfRowWXform (quadnode &r)
{
    
    int k, l, n, i;
    //cerr << " In Row X form \n" ;
    int o = ha.Size();
    int p = hd.Size();
    //cerr << " Past o and p allocation" << o << " " << p << "\n";
    int yur = r.yur;
    int yul = r.yul;
    int xul = r.xul;
    int xll = r.xll;
    int s2 = r.cols /2;
    int s = r.cols;
    int rows = r.rows;
    pm.totalinstructions += 9;
    pm.totaldatatransfers += 9;
    //cerr << " The value of s in Row X form " << s << "\n";
    //xTemp.allocate(s);
    //cerr << " Past vector allocation \n" ;
    //cerr << "In Row Transform " << endl;
    for ( i = 0; i < rows; i++){
        for ( k = 0; k < s; k++)
        {
            xTemp[k] = 0.0;
            xTemp2[k]= 0.0;
            pm.totalinstructions += 6;
            pm.totaldatatransfers += 2;
        }
        //cerr << " Initialized temporary vector " << i << endl;       
        for (k =0; k < s; k++)
            for ( l = 0; l < o; l++)
            {
                //n = s - (k -l ) -1;
                n = k -l;
                if ( (n >= 0) && ( n <  s)) {
                    xTemp[k] += S[i+xul][n+yul] * ha[l];
                    xTemp2[k] += S[i+xul][n+yul] * hd[l];
                    pm.totalinstructions += 6;
                    pm.totaldatatransfers += 2;
                }
                pm.totaldatacompares++;
                pm.totalinstructions +=2;
                pm.totaldatatransfers ++;
                
            }
                //cerr << "Aquired average " ;
                for ( k = 0; k < s2; k++)
                {
                    S[i+xul][k+yul] = xTemp[2*k + 1];
                    S[i+xul][k+s2+yul] = xTemp2[2*k+1];
                    pm.totalinstructions += 6;
                    pm.totaldatatransfers += 2;
                }        //cerr << "Stored Difference " << endl;
    }
        cerr << "\n Completed a Row X form \n";
}


void xformpsin::selfColWXform (quadnode &r)
{
    
    
    int k, l, n, j;
    int o = ha.Size();
    int p = hd.Size();
    int xul = r.xul;
    int xll = r.xll;
    int yul = r.yul;
    int yur = r.yur;
    int s2 = r.rows /2;
    int s = r.rows;
    int cols = r.cols;
    //yTemp.allocate(s);
    j = cols / 2;
    
    pm.totalinstructions += 9;
    pm.totaldatatransfers += 9;
    for ( j = 0; j < cols; j++) { 
        for ( k = 0; k < s; k++)
        {
            yTemp[k] = 0.0;
            yTemp2[k] = 0.0;
            pm.totalinstructions += 2;
            pm.totaldatatransfers += 2;
        }
        for ( k = 0; k < s; k++)
            for ( l = 0; l < o; l++)
            {
                n = k -l;
		if ( ( n >= 0) && ( n < s)) {
                    yTemp[k] += S[n+xul][j+yul] * ha[l];
                    yTemp2[k] += S[n+xul][j+yul] * hd[l];
                    
                    pm.totalinstructions += 6;
                    pm.totaldatatransfers += 2;
                }
                pm.totaldatacompares++;
                pm.totalinstructions +=2;
                pm.totaldatatransfers ++;
            }
                
                for ( k = 0; k < s2; k++) {
                    S[k+xul][j+yul] = yTemp[2*k + 1];
                    S[k+xul +s2][j + yul] = yTemp2[2*k+1];
                    pm.totalinstructions += 6;
                    pm.totaldatatransfers += 2;
                }
                
                //for ( k = 0; k < s; k++)
                //cerr << "\t" << result[k][j];
    }
        cerr << "\n Completed a Column X form \n";
        
        
}




void xformpsin::selfRowWIXform (quadnode &r)
{
    int startx = r.xul;
    int starty = r.yul;
    int endx = r.xlr;
    int endy = r.yur;
    int i,j;
    int lenx = r.rows;
    int leny = r.cols/2;
    double sq2 = sqhalf;
    pm.totalinstructions += 7;
    
    cerr << " In Row WIX " << startx << " by " << starty << " to " << endx << " by " << endy <<  " \n";
    for ( i = 0; i < lenx; i++)
        for (j = 0; j < leny; j++)
        {
            S[i + r.xul][2 * j + r.yul] = (T[i + r.xul][j + r.yul] - T[i + r.xul][j + r.yul + leny] ) * sq2;
            S[i + r.xul][2 * j + 1 + r.yul] = (T[i + r.xul][j + r.yul] + T[i + r.xul][j + r.yul + leny] ) * sq2;
            pm.totalinstructions += 14;
            pm.totaldatatransfers += 2;
        }
            
}

void xformpsin::selfColWIXform (quadnode &r)
{
    int startx = r.xul;
    int starty = r.yul;
    int endx = r.xlr;
    int endy = r.yur;
    int i,j;
    int lenx = r.rows /2;
    int leny = r.cols;
    double sq2 = sqhalf;
    pm.totalinstructions += 7;
    
    cerr << " In Col WIX "  << startx << " by " << starty << " to " <<   endx<< " by " << endy <<  " \n";
    for ( j = 0; j < leny; j++)
        for (i = 0; i < lenx; i++)
        {
            T[i * 2 + r.xul +0][ j + r.yul] = (S[i + r.xul][j + r.yul] - S[i + lenx + r.xul][j  + r.yul] ) * sq2;
            T[i *2  + r.xul+ 1][j + r.yul] = (S[i + r.xul][j + r.yul] + S[i + lenx + r.xul][j  + r.yul] ) * sq2;
            pm.totalinstructions += 14;
            pm.totaldatatransfers += 2;
        }
}


void xformpsin::selfWIXform (matrixType &source, matrixType &result, int resol)
{
    
 quadnode r;
 r.xul = 0 ;
 r.yul = 0;
 r.xur = 0;
 r.yll = 0;
 r.xll = source.m_rows -1 ;
 r.xlr = source.m_rows - 1;
 r.yur = source.m_cols -1;
 r.ylr = source.m_cols -1; 
 r.rows = source.m_rows;
 r.cols = source.m_cols;
 r.epsilon = 0.0;
 
    int lr, lc, res;
    int rows, cols;
    int index;
    int i;
    cerr << "Allocated quadnode and set values \n";
    setmatrix(source);
    //delete qtree;
    //qtree = new qtreearray ();
    //qtree->initialize (S);
    //qtree->getNode(0,r);
    rows = S.m_rows;
    cols = S.m_cols;
    lr = findX2 (rows);
    lc = findX2 (cols);
    //if ( lr > lc ) res = lc;
    //else 
    //    res = lr;
    //cerr << " \n The resolution in the self WX form is " << res << " \n";
    //res = res / 2 - 1;
    res = resol;
    int vertices = (int) pow ((double) 4, (double) res);
    resolution = vertices;
    //astack = new stackarray (vertices);
    //int qtlength = qtree->getLength();
    pm.totalinstructions += 12;
    //for ( index = vertices; index < qtlength; index++) qtree->killNode(index);
    //selfWIXform (r);
    for ( i = 0; i < res; i++)
    {
        selfColWIXform(r);
        selfRowWIXform(r);
    }
    getmatrix(result);
}


void xformpsin::setmatrix (matrixType &A)
{
    int rows = A.m_rows;
    int cols = A.m_cols;
    pm.totalinstructions += 2;
    S.allocateStorage(rows, cols);
    T.allocateStorage(rows,cols);
    pm.totalinstructions += 2* rows * cols;
    S.epsilon = A.epsilon;
    S.NUMBER_OF_GREY_LEVELS = A.NUMBER_OF_GREY_LEVELS;
    S.scaleX = A.scaleX;
    S.scaleY = A.scaleY;
    S.shiftX = A.shiftY;
    S.shiftY = A.shiftY;
    pm.totalinstructions += 6;
    int i, j;
    for ( i = 0; i < rows; i++)
        for (j = 0; j < cols; j++)
        {
            S[i][j] = A[i][j];
            pm.totalinstructions ++;
            pm.totaldatatransfers++;
        }
            
}
void xformpsin::getmatrix (matrixType &A)
{
    int rows = S.m_rows;
    int cols = S.m_cols;
    pm.totalinstructions += 2;
    A.allocateStorage(rows, cols);
    pm.totalinstructions +=  rows * cols;
    A.epsilon = S.epsilon;
    S.NUMBER_OF_GREY_LEVELS = A.NUMBER_OF_GREY_LEVELS;
    A.scaleX = S.scaleX;
    A.scaleY = S.scaleY;
    A.shiftX = S.shiftY;
    A.shiftY = S.shiftY;
    pm.totalinstructions += 6;
    int i, j;
    for ( i = 0; i < rows; i++)
        for (j = 0; j < cols; j++)
        {
            A[i][j] = S[i][j];
            pm.totalinstructions ++;
            pm.totaldatatransfers++;
        }
}

xformpsin::xformpsin ()
{
    pm.totalinstructions = 0;
    pm.totaldatacompares = 0;
    pm.totaldatatransfers = 0;
    sqhalf = pow (0.5 , 0.5);
    //queue = NULL;
    //astack = NULL;
    ha.allocate(2);
    hd.allocate(2);
    ha[0] = hd[0] = ha[1] = sqhalf;
    hd[1] = -sqhalf;
    aval = 0;
    vval = 1;
    hval = 2;
    dval = 3;
    
}


xformpsin::xformpsin (xformpsin *axform)
{
    pm.totalinstructions = 0;
    pm.totaldatacompares = 0;
    pm.totaldatatransfers = 0;
    sqhalf = pow (0.5 , 0.5);
    axform->getmatrix (S);
    int rows = S.m_rows;
    int cols = S.m_rows;
    T.allocateStorage(rows, cols);
    //axform->getquadtree (*qtree);
    //queue = NULL;
    //astack = NULL;
    ha.allocate(2);
    hd.allocate(2);
    ha[0] = hd[0] = ha[1] = sqhalf;
    hd[1] = -sqhalf;
    aval = 0;
    vval = 1;
    hval = 2;
    dval = 3;
    
}
