#include "xformqtree.hpp"
#include "queuearray.hpp"
#include <math.h>

xformquadtree::xformquadtree ()
{
}


xformquadtree::xformquadtree (xformquadtree *axform)
{
}


void xformquadtree::setquadtree(quadnode *start)
{
    int i = 0;
    
}

void xformquadtree::getquadtree(quadnode *start)
{
}

void xformquadtree::setmatrix (matrixType &A)
{
    int rows = A.m_rows;
    int cols = A.m_cols;
    S.allocateStorage(rows, cols);
    T.allocateStorage(rows,cols);
    S.epsilon = A.epsilon;
    S.NUMBER_OF_GREY_LEVELS = A.NUMBER_OF_GREY_LEVELS;
    S.scaleX = A.scaleX;
    S.scaleY = A.scaleY;
    S.shiftX = A.shiftY;
    S.shiftY = A.shiftY;
    int i, j;
    for ( i = 0; i < rows; i++)
        for (j = 0; j < cols; j++)
        {
            S[i][j] = A[i][j];
        }
    
}
void xformquadtree::getmatrix (matrixType &A)
{
    int rows = S.m_rows;
    int cols = S.m_cols;
    A.allocateStorage(rows, cols);
    A.epsilon = S.epsilon;
    S.NUMBER_OF_GREY_LEVELS = A.NUMBER_OF_GREY_LEVELS;
    A.scaleX = S.scaleX;
    A.scaleY = S.scaleY;
    A.shiftX = S.shiftY;
    A.shiftY = S.shiftY;
    int i, j;
    for ( i = 0; i < rows; i++)
        for (j = 0; j < cols; j++)
        {
            A[i][j] = S[i][j];
        }
}


void xformquadtree::selfRowWXform (int i, int col)
{
}

void xformquadtree::selfRowWXform ()
{

 int k, l, n, i;
 myVector xTemp;
	
 int o = ha.Size();
 int p = hd.Size();
 int yur = current->getYUR();
 int yul = current->getYUL();
 int s2 = (yul - yur) /2;
 int s = (yul - yur);
 int rows = (current->getXUL() - current->getXLL());
	xTemp.allocate(s);
 //cout << "In Row Transform " << endl;
 for ( i = 0; i < rows; i++){
     for ( k = 0; k < s; k++)
         xTemp[k] = 0.0;
     //cout << " Initialized temporary vector " << i << endl;       
     for (k =0; k < s; k++)
         for ( l = 0; l < o; l++)
         {
             //n = s - (k -l ) -1;
             n = k -l;
             if ( (n >= 0) && ( n <  s))
                 xTemp[k] += S[i][n] * ha[l];
         }
             //cout << "Aquired average " ;
             for ( k = 0; k < s2; k++)
             {
                 T[i][k] = xTemp[2*k + 1];
             }
             
             for ( k = 0; k < s; k++)
                 xTemp[k] = 0.0;
    	//cout << "Getting Difference " ;
     for ( k = 0; k < s; k++)
     {
         for ( l = 0; l < p; l++)
         {
             //n = s - (k-l) - 1;
             n = k -l;
             if (( n >= 0) && ( n < s ))
                 xTemp[k] += S[i][n] * hd[l];
         }
     }
     //cout << "Acquired Difference " ;
     for ( k = 0; k < s2; k++)
     {
         T[i][k + s2] = xTemp[2*k + 1];
     }
     //cout << "Stored Difference " << endl;
 }
 
}


void xformquadtree::selfColWXform (int j, int row)
{
}

void xformquadtree::selfColWXform ()
{

 
    int k, l, n, j;
    myVector yTemp;
    int o = ha.Size();
    int p = hd.Size();
    int xul = current->getXUL();
    int xll = current->getXLL();
    int yul = current->getYUL();
    int yur = current->getYUR();
    int s2 = (xul - xll) /2;
    int s = (xul - xll);
    int cols = (yul - yur);
    yTemp.allocate(s);
    j = cols / 2;
 
    for ( j = 0; j < cols; j++) { 
        for ( k = 0; k < s; k++)
            yTemp[k] = 0.0;
        
        for ( k = 0; k < s; k++)
            for ( l = 0; l < o; l++)
            {
                n = k -l;
		if ( ( n >= 0) && ( n < s))
                    yTemp[k] += T[n][j] * ha[l];
            }
                
                for ( k = 0; k < s2; k++)
                    S[k][j] = yTemp[2*k + 1];
  
        for ( k = 0; k < s; k++)
            yTemp[k] = 0.0;
        for ( k = 0; k < s; k++)
            for ( l = 0; l < o; l++)
            {
                n = k - l;
		//n = s - (k -l ) -1;
                if ( ( n >= 0 ) && ( n < s ))
                    yTemp[k] += T[n][j] * hd[l];
            }
                for ( k = 0; k < s2; k++)
                    S[s2 + k][j] = yTemp[2*k + 1];
        
	//for ( k = 0; k < s; k++)
        //cout << "\t" << result[k][j];
    }
 
    
}


void xformquadtree::selfColWXform (int i, int col, myVector &ha, myVector &hd)
{
}
/*
void xformquadtree::selfWXform (matrixType &source, matrixType &result)
{
    head = new quadtree();
    setMatrix (source);
    selfRowXform();
    selfColXform();
    getMatrix (result);
}
*/

int xformquadtree::findX2 (int i)
{
    int r = i;
    int count = 0;
    int even = 1;
    while ( (r != 0) & ((r % 2) ==0) )
    {
        r = r /2;
        count++;
    }
    return count;
}

void xformquadtree::getSstats (quadnode &r)
{
    double sum = 0.0;
    double sum2 = 0.0
    double max = 0.0;
    double min = infinity;
    int i, j;
    for ( i = r.xul; i < r.xll; i++)
        for ( j = r.yul; j < r.yur; j++)
        {
            if ( max <= S[i][j]) max = S[i][j] ;
            if ( min >= S[i][j]) min = S[i][j];
            sum += S[i][j];
            sum2 += S[i][j] * S[i][j];
        }
            r.max = max;
    r.min = min;
    r.L1energy = sum;
    r.L2energy = sum2;
}

void xformquadtree::setAnode (quadnode &r, quadnode &a)
{
    if(((r.rows % 2) == 0) & ((r.cols % 2) ==0))
    {
        int rows = (r.rows)/2;
        int cols = (r.cols)/2;
        a.rows = rows;
        a.cols = cols;
        a.xul = r.xul;
        a.xll = r.xul + rows - 1;
        a.xur = r.xur;
        a.xlr = r.xur + rows - 1;
        a.yul = r.yul;
        a.yur = r.yul + cols - 1;
        a.yll = r.yll;
        a.ylr = r.yll + cols - 1;
        a.isnotnull = 1;
    } else a.isnotnull = 0;
    
}

void xformquadtree::setVnode (quadnode &r, quadnode &a)
{
    if(((r.rows % 2) == 0) & ((r.cols % 2) ==0))
    {
        int rows = (r.rows)/2;
        int cols = (r.cols)/2;
        a.rows = rows;
        a.cols = cols;
        a.xul = r.xul;
        a.xll = r.xul + rows - 1;
        a.xur = r.xur;
        a.xlr = r.xur + rows - 1;
        a.yul = r.yul + cols;
        a.yll = r.yul + cols;
        a.yur = r.yur;
        a.ylr = r.ylr;
        a.isnotnull = 1;
    } else a.isnotnull = 0;
}

void xformquadtree::setHnode (quadnode &r, quadnode &a)
{
    if(((r.rows % 2) == 0) & ((r.cols % 2) ==0))
    {
        int rows = (r.rows)/2;
        int cols = (r.cols)/2;
        a.rows = rows;
        a.cols = cols;
    
        a.xul = r.xul + rows;
        a.xll = r.xll;
        a.xur = r.xur + rows;
        a.xlr = r.xlr;
        a.yul = r.yul;
        a.yll = r.yll;
        a.yur = r.yul + cols - 1;
        a.ylr = r.yll + cols - 1;
        a.isnotnull = 1;
    } else a.isnotnull = 0;
}

void xformquadtree::setDnode (quadnode &r, quadnode &a)
{
    if(((r.rows % 2) == 0) & ((r.cols % 2) ==0))
    {
        int rows = (r.rows)/2;
        int cols = (r.cols)/2;
        a.rows = rows;
        a.cols = cols;
    
        a.xul = r.xul + rows;
        a.xll = r.xll;
        a.xur = r.xur + rows;
        a.xlr = r.xlr;
        a.yul = r.yul + cols;
        a.yll = r.yul + cols;
        a.yur = r.yur;
        a.ylr = r.ylr;
        a.isnotnull = 1;
    } else a.isnotnull = 0;
}



void xformquadtree::selfWXform ()
{
    int rows = S.m_rows;
    int cols = S.m_cols;
    int lr = findX2 (rows);
    int lc = findX2 (cols);
    quadnode r;
    int res;
    if ( lr > lc ) res = lc;
        else 
            res = lr;
        int vertices = (int) pow(4, res);
        qtree = new qtreearray(verticies);
        qarray = new queuearray(verticies);
        r.isnotnull = 1;
        r.rows = rows;
        r.cols = cols;
        r.xul = r.xur = 0;
        r.yul = r.yll = 0;
        r.xlr = xll = rows;
        r.ylr = r.yur = cols;
        r.energy = calEnergy(r.xul, r.yul, r.xlr, r.ylr);
        r.max = maxValue (r.xul, r.yul, r.xlr, r.ylr);
        r.max = minValue (r.xul, r.yul, r.xlr, r.ylr);
        r.length = 0;
        selfWXform (r);
    getMatrix (result);
}

void xformquadtree::selfWXform (quadnode &r)
/*
    Self Wavelet Transform
    Arguments:  Quad Node r
    
    This procedure uses the already set S and T matrices as source.  The 
        quad node r establishes the boundaries.
        This transform is a loop which uses a queue to feed itself.  
        Once the queue is empty, the procedure is finished.
    The first step is to load the queue with r.  Then start the loop.
        Get from the queue the FIFO item.  Perform the wavelet transform 
        on those parameters.  Once done, use the parameters to determine if
        a subsequent resolution is possible, for each branch.  If so, enter
        the it and the pointer to its place in the tree into the queue.  In
        any case, repeat the procedure.  
    Note that both the tree and queue structure are part of the class and are initialized
        by the calling function.
        
 */
{
    quadnode temp temp2, a,v,h,d;
    qarray->setNode (r);
    while (!qarray->empty())
    {
        qarray->get(temp);
        selfRowXform(temp);
        selfColXform(temp);
        setAnode (r,a);
        setVnode (r,v);
        sethnode (r,h);
        setDnode (r,d);
        qtree->gotoNode(r.id);
        if ( qtree->noleaving() ) {
            qtree->getNode(qtree->getA(), a);
            qtree->getNode(qtree->getV(), v);
            qtree->getNode(qtree->getH(), h);
            qtree->getNode(qtree->getD(), d);
            getSstats (a);
            getSstats (v);
            getSstats (h);
            getSstats (d);
            if ( !(epsilon (a.L1energy)) & !(epsilon(a.L2energy)) & a.isnotnull)
                qarray->setNode(a);
            if ( !(epsilon (v.L1energy)) & !(epsilon(v.L2energy)) & v.isnotnull)
                qarray->setNode(v);
            if ( !(epsilon (h.L1energy)) & !(epsilon(h.L2energy)) & h.isnotnull)
                qarray->setNode(h);
            if ( !(epsilon (d.L1energy)) & !(epsilon(d.L2energy)) & d.isnotnull)
                qarray->setNode(d);
            
        }
        
    }
}

void xformquadtree::selfRowWaveletXformInverse (matrixType &source, matrixType &result, int i)
{
}

void xformquadtree::selfColWaveletXformInverse (matrixType &source, matrixType &result, int j)
{
}

void xformquadtree::selfWaveletXformInverse (matrixType &source, matrixType &result)
{
}


void xformquadtree::selfRowWaveletXformInverse (matrixType &W, int col, int i)
{
}

void xformquadtree::selfColWaveletXformInverse (matrixType &W, int row, int j)
{
}

void xformquadtree::selfWaveletXformInverse (matrixType &source, matrixType &result, int res)
{
}

/*
bool xformquadtree::epsilon ( double x, int th)
{
}

bool xformquadtree::epsilon ( double x, double thresh)
{
}

*/
