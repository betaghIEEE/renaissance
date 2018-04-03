#include "xformqtree.hpp"
#include "queuearray.hpp"
#include "haarGen.hpp"
#include <iostream.h>
#include <stddef.h>
#include <math.h>


bool xformquadtree::epsilon ( double x, int th)
{
    return epsilon (x, (double) th);
}

bool xformquadtree::epsilon ( double x, double thresh)
{
    cout << " In epsilon \n";
    pm.totaldatacompares++;
    pm.totalinstructions ++;
    pm.totaldatatransfers ++;
    if ( fabs(x) > thresh) return false;
    return true;
}


int xformquadtree::findX2 (int i)
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

void xformquadtree::getSstats (quadnode &r)
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
            sum += S[i][j];
            sum2 += S[i][j] * S[i][j];
            pm.totalinstructions += 4;
            pm.totaldatatransfers += 4;
        }
            r.max = max;
    r.min = min;
    r.L1energy = sum;
    r.L2energy = sum2;
    
    pm.totalinstructions += 8;
    pm.totaldatatransfers += 8;
}


 void xformquadtree::getNode (quadnode &r)
{
     r.xul = qtree->getXUL();
     r.xll = qtree->getXLL();
     r.yul = qtree->getYUL();
     r.yll = qtree->getYLL();
     r.xur = qtree->getXUR();
     r.xlr = qtree->getXLR();
     r.yur = qtree->getYUR();
     r.ylr = qtree->getYLR();
     r.rows = qtree->getRows();
     r.cols = qtree->getCols();
     pm.totalinstructions += 15;
     pm.totaldatatransfers += 15;
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
        a.id = r.id * 4 + 1;
        a.epsilon = r.epsilon;
        pm.totalinstructions += 15;
        pm.totaldatatransfers += 15;
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
        a.id = r.id * 4 + 2;
        a.epsilon = r.epsilon;
        pm.totalinstructions += 15;
        pm.totaldatatransfers += 15;
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
        a.id = r.id * 4 + 3;
        a.epsilon = r.epsilon;
        pm.totalinstructions += 15;
        pm.totaldatatransfers += 15;
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
        a.isnotnull = 3;
        a.id = r.id * 4 + 4;
        a.epsilon = r.epsilon;
        pm.totalinstructions += 15;
        pm.totaldatatransfers += 15;
    } else a.isnotnull = 0;
}


void xformquadtree::selfRowWXform (quadnode &r)
{
    
    int k, l, n, i;
    //cout << " In Row X form \n" ;
    int o = ha.Size();
    int p = hd.Size();
    //cout << " Past o and p allocation" << o << " " << p << "\n";
    int yur = r.yur;
    int yul = r.yul;
    int xul = r.xul;
    int xll = r.xll;
    int s2 = r.cols /2;
    int s = r.cols;
    int rows = r.rows;
    pm.totalinstructions += 9;
    pm.totaldatatransfers += 9;
    //cout << " The value of s in Row X form " << s << "\n";
    //xTemp.allocate(s);
    //cout << " Past vector allocation \n" ;
    //cout << "In Row Transform " << endl;
    for ( i = 0; i < rows; i++){
        for ( k = 0; k < s; k++)
        {
            xTemp[k] = 0.0;
            xTemp2[k]= 0.0;
            pm.totalinstructions += 6;
            pm.totaldatatransfers += 2;
        }
        //cout << " Initialized temporary vector " << i << endl;       
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
                //cout << "Aquired average " ;
                for ( k = 0; k < s2; k++)
                {
                    S[i+xul][k+yul] = xTemp[2*k + 1];
                    S[i+xul][k+s2+yul] = xTemp2[2*k+1];
                    pm.totalinstructions += 6;
                    pm.totaldatatransfers += 2;
                }        //cout << "Stored Difference " << endl;
    }
    cout << "\n Completed a Row X form \n";
}


void xformquadtree::selfColWXform (quadnode &r)
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
        //cout << "\t" << result[k][j];
    }
    cout << "\n Completed a Column X form \n";
    
    
}

void xformquadtree::haarRowWXform (quadnode &r)
{
    
    
    
    int k, l, n, j, i;
    int x, y, x2, x1;
    int o = ha.Size();
    int p = hd.Size();
    int xul = r.xul;
    int xll = r.xll;
    int yul = r.yul;
    int yur = r.yur;
    int s2 = r.rows /2;
    int s = r.rows;
    int cols = r.cols;
    double left , right;
    pm.totalinstructions += 8;
    pm.totaldatatransfers += 8;
    cout << "Inside Haar Row Xform \n";
    cout << " XUL = " << xul << " YUL = " << yul << "\n";
    cout << " rows = " << s << " columns = " << cols << " sqhalf "<< sqhalf << "\n";
    
    
    for ( j = 0; j < cols; j++)
    {
        for ( i = 0; i < s2; i++)
        {
            xTemp[i] = 0.0;
            xTemp2[i] = 0.0;
            pm.totalinstructions += 2;
            pm.totaldatatransfers += 2;
        }
        cout << " Initialized xTemp \n";
        
            for ( i = 0; i < s2; i++)
            {
                x = i * 2 + xul;
                x1 = i *2 + xul +1;
                y = j + yul;
                cout << " x = " << x << " y = " << y << " x1 " << x1 << "\n";
                left = S[x][y];
                cout << left << "  ";
                right = S[x1][y];
                cout << right << " \n"; 
                xTemp[i] = (left + right) * sqhalf;
                xTemp2[i] = (left - right) * sqhalf;
                pm.totalinstructions += 7;
                pm.totaldatatransfers += 7;
                
            }
            
            cout << " Row X form for column " << j << " complete \n";
            for ( i = 0; i < s2; i++)
            {
                x = i + xul;
                x2 = i + s2 + xul;
                y = j + yul;
                cout << " i = " << i << " x " << x << " x2 " << x2 << " y " << y << " j " << j << " \n";
                S[x][y] = xTemp[i];
                S[x2][y] = xTemp[i];
                pm.totalinstructions += 5;
                pm.totaldatatransfers += 5;
                    
            }
        }
    
    
    
}


void xformquadtree::haarColWXform (quadnode &r)
{
    
    
    
    int k, l, n, j, i;
    int x, y, x2, y2;
    int o = ha.Size();
    int p = hd.Size();
    int xul = r.xul;
    int xll = r.xll;
    int yul = r.yul;
    int yur = r.yur;
    int s2 = r.cols /2;
    int s = r.cols;
    int rows = r.rows;
    
    pm.totalinstructions += 9;
    pm.totaldatatransfers += 9;
    
    for ( i = 0; i < rows; i++)
    {
        for ( j = 0; i < s2; j++)
        {
            xTemp[j] = 0.0;
            xTemp2[j] = 0.0;
            pm.totalinstructions += 2;
            pm.totaldatatransfers += 2;
        }
        for ( j = 0; j < s2; j++)
        {
            x = i + xul;
            y = j * 2 + yul;
            xTemp[j] = (S[x][y] + S[x][y + 1]) * sqhalf;
            xTemp2[j] = (S[x][y] - S[x][y + 1]) * sqhalf;
            pm.totalinstructions += 10;
            pm.totaldatatransfers += 4;
        }
        for ( i = 0; i < s2; i++)
        {
            x = i + xul;
            y2 = i + s2 + yul;
            y = j + yul;
            S[x][y] = xTemp[i];
            S[x][y2] = xTemp[i];
            pm.totalinstructions += 5;
            pm.totaldatatransfers += 5;
            
        }
    }
    
    
    
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
    cout << " \n The resolution in the self WX form is " << res << " \n";
    //res = res / 2 -1;
    res = 6;
    int vertices = (int) pow(4, res);
    resolution = vertices;
    
    cout << " \n The vertices in the self WX form is " << vertices << " \n";
    qtree = new qtreearray();
    queue = new queuearray(vertices + 1);
    qtree->initialize (S);
    cout << " Quad Tree Initalized \n";
    
    pm.totalinstructions += 9;
    /*
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
    qtree->setNode(0, r); */
    
    qtree->getStats (S, 0);
    cout << " Stats taken \n";
    qtree->getNode (0, r);
    xTemp.allocate(cols);
    yTemp.allocate(rows);
    xTemp2.allocate(cols);
    yTemp2.allocate(rows);
    
    cout << " X and Y vectors allocated \n";
    selfWXform (r);
}



// void xformquadtree::selfWXform (quadnode &r)
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
/*
{
    quadnode temp, temp2, a,v,h,d;
    int ai, vi, hi, di;
    queue->put (r);
    //int length = queue->getLength();
    int length = resolution;
    int length4 = resolution / 4;
    cout << "In Self WX form r "  << length << " " << r.id << "\n";
    while (!queue->empty())
    {
        queue->get(temp);
        
        //cout << a.id << " " << v.id << " " <<  h.id << " " << d.id <<  "\n";
        if (qtree->gotoNode(temp.id) & (( temp.id ) < length4 ))
        {
            cout << " Starting Process \n";
            selfRowWXform(temp);
            selfColWXform(temp);
            cout << " Getting stats \n";
            /*
            getSstats (a);
            getSstats (v);
            getSstats (h);
            getSstats (d);
             */ /*
            //qtree->gotoNode(temp.id);
            ai = qtree->getA();
            vi = qtree->getV();
            hi = qtree->getH();
            di = qtree->getD();
            //qtree->getNode (
            cout << " Have stats \n";
            //if (  !(epsilon(a.L2energy, a.epsilon)) & qtree->setNode(qtree->getA(), a))
            if ( ai < length4) {
                qtree->getStats(S, ai);
                qtree->getNode (ai, a);
                queue->put(a);
            }
            cout << " Past A \n";
            //if ( !(epsilon(v.L2energy, v.epsilon)) & qtree->setNode(qtree->getV(), v))
            if ( vi < length4) {
                qtree->getStats(S, vi );
                qtree->getNode (vi, v);
                queue->put(v);
            }
            cout << " Past V \n";
            //if (  !(epsilon(h.L2energy, h.epsilon)) & qtree->setNode(qtree->getH(), h))
            if ( hi < length4) {
                qtree->getStats(S, hi );
                qtree->getNode (hi, h);
                queue->put(h);
            }
            cout << " Past H \n";
            //if (  !(epsilon(d.L2energy, d.epsilon)) & qtree->setNode(qtree->getD(), d))
            if ( di <= length4) {
                qtree->getStats(S, di );
                qtree->getNode (di, d);
                queue->put(d);
            }
        }
        cout << " Next loop \n " ;
    }
}

*/

void xformquadtree::selfWXform (quadnode &r)
 {
    quadnode temp, temp2, a,v,h,d;
    int ai, vi, hi, di;
    int index;
    //int length = queue->getLength();
    int length = resolution;
    int length4 = resolution/4 + 1;
    cout << "In Self WX form r "  << length << " " << r.id << "\n";
    int i,j;            
    pm.totalinstructions += 4;
   // for (i = 0; i < r.rows; i++)
     //   for ( j = 0; j < r.cols; j++)
       //     T[i][j] = S[i][j];
    for ( index = r.id; index < length4; index++)
    {
        qtree->gotoNode(index);
        qtree->getNode (temp);
        selfRowWXform(temp);
        selfColWXform(temp);
        getSstats(temp);
        cout << " Next loop  " << index << " \n" ;
    }
    ///for (i = 0; i < r.rows; i++)
       // for ( j = 0; j < r.cols; j++)
         //   S[i][j] = T[i][j];
    
}

void xformquadtree::selfPWXform (quadnode &r)
{
    quadnode temp, temp2, a,v,h,d;
    int ai, vi, hi, di;
    int index;
    //int length = queue->getLength();
    int length = resolution;
    int length4 = resolution/4 ;
    cout << "In Self WX form r "  << length << " " << r.id << "\n";
    int i,j;            
    pm.totalinstructions += 4;
    
    // for (i = 0; i < r.rows; i++)
    //   for ( j = 0; j < r.cols; j++)
    //     T[i][j] = S[i][j];
    for ( index = r.id; index < length4; index = index * 4 +1)
    {
        qtree->gotoNode(index);
        qtree->getNode (temp);
        selfRowWXform(temp);
        selfColWXform(temp);
        getSstats(temp);
        cout << " Next loop  " << index << " \n" ;
    }
    ///for (i = 0; i < r.rows; i++)
    // for ( j = 0; j < r.cols; j++)
    //   S[i][j] = T[i][j];
    
}

void xformquadtree::haarWXform (quadnode &r)
{
    quadnode temp, temp2, a,v,h,d;
    int ai, vi, hi, di;
    int index;
    //int length = queue->getLength();
    int length = resolution;
    int length4 = resolution/4 ;
    pm.totalinstructions += 4;

    cout << "In Self WX form r "  << length << " " << r.id << "\n";
    int i,j;
    // for (i = 0; i < r.rows; i++)
    //   for ( j = 0; j < r.cols; j++)
    //     T[i][j] = S[i][j];
    for ( index = r.id; index < length4; index++)
    {
        qtree->gotoNode(index);
        qtree->getNode (temp);
        haarRowWXform(temp);
        haarColWXform(temp);
        cout << " Next loop  " << index << " \n" ;
    }
    ///for (i = 0; i < r.rows; i++)
    // for ( j = 0; j < r.cols; j++)
    //   S[i][j] = T[i][j];
    
}



void xformquadtree::selfRowWIXform (quadnode &r)
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
    
    cout << " In Row WIX " << startx << " by " << starty << " to " << endx << " by " << endy <<  " \n";
    for ( i = 0; i < lenx; i++)
        for (j = 0; j < leny; j++)
        {
            S[i + r.xul][2 * j + r.yul] = (T[i + r.xul][j + r.yul] - T[i + r.xul][j + r.yul + leny] ) * sq2;
            S[i + r.xul][2 * j + 1 + r.yul] = (T[i + r.xul][j + r.yul] + T[i + r.xul][j + r.yul + leny] ) * sq2;
            pm.totalinstructions += 14;
            pm.totaldatatransfers += 2;
        }
    
}

void xformquadtree::selfColWIXform (quadnode &r)
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
    
    cout << " In Col WIX "  << startx << " by " << starty << " to " <<   endx<< " by " << endy <<  " \n";
    for ( j = 0; j < leny; j++)
        for (i = 0; i < lenx; i++)
        {
            T[i * 2 + r.xul +0][ j + r.yul] = (S[i + r.xul][j + r.yul] - S[i + lenx + r.xul][j  + r.yul] ) * sq2;
            T[i *2  + r.xul+ 1][j + r.yul] = (S[i + r.xul][j + r.yul] + S[i + lenx + r.xul][j  + r.yul] ) * sq2;
            pm.totalinstructions += 14;
            pm.totaldatatransfers += 2;
        }
}


bool xformquadtree::selfWIXform (quadnode &r)
{
    quadnode temp;
    quadnode tempchild;
    astack->push(r);
    int index;
    cout << " Inside the WIX xform \n";
    int length = resolution / 4;
    int i, j;
    pm.totalinstructions += 4;
    pm.totaldatatransfers ++;
    //for (i = 0; i < r.rows; i++)
    //    for ( j = 0; j < r.cols; j++)
          //  T[i][j] = S[i][j];
    for ( index = length - 1; index >= 0; index--)
    {
        qtree->gotoNode(index);
        qtree->getNode(temp);
        selfColWIXform(temp);
        selfRowWIXform(temp);
        
    }
    // for (i = 0; i < r.rows; i++)
    //    for ( j = 0; j < r.cols; j++)
     //       S[i][j] = T[i][j]; 
    return true;
}

bool xformquadtree::selfPWIXform (quadnode &r)
{
    quadnode temp;
    quadnode tempchild;
    astack->push(r);
    int index,start;
    cout << " Inside the WIX xform \n";
    int length = resolution / 4;
    int i, j;
    //for (i = 0; i < r.rows; i++)
    //    for ( j = 0; j < r.cols; j++)
    //  T[i][j] = S[i][j];
    pm.totalinstructions += 4;
    pm.totaldatatransfers ++;
    for ( index = 0; index < length; index = index * 4 +1){ start = index; pm.totalinstructions++;}
    for ( index = start; index >= 0;  )
    {
        qtree->gotoNode(index);
        qtree->getNode(temp);
        selfColWIXform(temp);
        selfRowWIXform(temp);
        if ( index != 0 ) index = (index)/4;
        else index = -1;
        pm.totalinstructions += 3;
        
    }
    // for (i = 0; i < r.rows; i++)
    //    for ( j = 0; j < r.cols; j++)
    //       S[i][j] = T[i][j]; 
    return true;
}


xformquadtree::xformquadtree ()
{
    pm.totalinstructions = 0;
    pm.totaldatacompares = 0;
    pm.totaldatatransfers = 0;
    sqhalf = pow (0.5 , 0.5);
    queue = NULL;
    astack = NULL;
    ha.allocate(2);
    hd.allocate(2);
    ha[0] = hd[0] = ha[1] = sqhalf;
    hd[1] = -sqhalf;
}


xformquadtree::xformquadtree (xformquadtree *axform)
{
    pm.totalinstructions = 0;
    pm.totaldatacompares = 0;
    pm.totaldatatransfers = 0;
    sqhalf = pow (0.5 , 0.5);
    axform->getmatrix (S);
    int rows = S.m_rows;
    int cols = S.m_rows;
    T.allocateStorage(rows, cols);
    axform->getquadtree (*qtree);
    queue = NULL;
    astack = NULL;
    ha.allocate(2);
    hd.allocate(2);
    ha[0] = hd[0] = ha[1] = sqhalf;
    hd[1] = -sqhalf;
    
}

xformquadtree::xformquadtree (qtreearray &sample)
{
    pm.totalinstructions = 0;
    pm.totaldatacompares = 0;
    pm.totaldatatransfers = 0;
    sqhalf = pow (0.5 , 0.5);
    int rows;
    int cols;
    queue = NULL;
    astack = NULL;
    ha.allocate(2);
    hd.allocate(2);
    ha[0] = hd[0] = ha[1] = sqhalf;
    hd[1] = -sqhalf;
    sample.gotoNode(0);
    quadnode temp;
    sample.getNode(temp);
    S.allocateStorage(temp.rows, temp.cols);
    T.allocateStorage(temp.rows, temp.cols);
    getquadtree (sample);
}




void xformquadtree::setquadtree(qtreearray &start)
{
    int i = 0;
    int length = start.getLength();
    qtree = new qtreearray (length);
    quadnode temp;
    for ( i = 0; i < length; i++)
    {
        start.getNode (i, temp);
        if (qtree->isleaf(temp.id))
            setmatrix (temp);
        
        qtree->setNode (i, temp);
        
    }
    
}

void xformquadtree::getquadtree(qtreearray &start)
{    
    int i = 0;
    int length = qtree->getLength();
    start.allocate(length);
    quadnode temp;
    for ( i = 0; i < length; i++)
    {
        qtree->getNode (i, temp);
        if (qtree->isleaf(temp.id))
            getmatrix(temp);
        start.setNode (i, temp);
    }
}

void xformquadtree::setmatrix (matrixType &A)
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
void xformquadtree::getmatrix (matrixType &A)
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

void xformquadtree::setmatrix (quadnode &A)
{
    int rows = A.rows;
    int cols = A.cols;
    pm.totalinstructions += 2;
    S.allocateStorage (rows, cols);
    pm.totalinstructions += rows * cols;
    
    int i,j;
    int ieq;
    for (i = 0; i < rows; i++)
        for ( j = 0; j < cols; j++)
        {
            ieq = cols * i + j;
            //S[i][j] = A.values[ieq];
            pm.totalinstructions ++;
            pm.totaldatatransfers++;
            
        }
            
}



void xformquadtree::getmatrix (quadnode &A)
{
    int rows = S.m_rows;
    int cols = S.m_cols;
    A.rows = rows;
    A.cols = cols;
    pm.totalinstructions += 4;
    pm.totaldatatransfers += 4;
    int length = rows * cols;
    A.length = length;
    //A.values = new double[length];
    
    int i,j;
    int ieq;
    for (i = 0; i < rows; i++)
        for ( j = 0; j < cols; j++)
        {
            ieq = cols * i + j;
            pm.totalinstructions ++;
             //A.values[ieq] = S[i][j];
        }
            
}

void xformquadtree::selfWXform (matrixType &source, matrixType &result)
{

    cout << "Allocated quadnode and set values \n";
    setmatrix (source);
    cout << " Matrix set \n";
    //selfRowWXform(r);
    cout << " Row X form complete \n";
    //selfColWXform(r);
    selfWXform();
    getmatrix (result);
}

void xformquadtree::haarWXform (matrixType &source, matrixType &result)
{
    cout << "Allocated quadnode and set values \n";
    setmatrix (source);
    cout << " Matrix set \n";
    
    
    int rows = S.m_rows;
    int cols = S.m_cols;
    int lr = findX2 (rows);
    int lc = findX2 (cols);
    quadnode r;
    int res;
    if ( lr > lc ) res = lc;
    else 
        res = lr;
    cout << " \n The resolution in the self WX form is " << res << " \n";
    res = res / 2 -1;
    res = 1;
    int vertices = (int) pow(4, res);
    resolution = vertices;
    
    pm.totalinstructions += 7;
    cout << " \n The vertices in the self WX form is " << vertices << " \n";
    qtree = new qtreearray();
    queue = new queuearray(vertices + 1);
    qtree->initialize (S);
    cout << " Quad Tree Initalized \n";
    
    pm.totalinstructions += (int)pow(4,7);
    
    qtree->getStats (S, 0);
    cout << " Stats taken \n";
    qtree->getNode (0, r);
    xTemp.allocate(cols);
    yTemp.allocate(rows);
    xTemp2.allocate(cols);
    yTemp2.allocate(rows);
    
    pm.totalinstructions += 2 * (rows + cols);
    
    cout << " X and Y vectors allocated \n";
    haarRowWXform (r);
    haarColWXform (r);
    getmatrix(result);
}

void xformquadtree::selfWXform (matrixType &source, qtreearray &result)
{
    setmatrix (source);
    selfWXform ();
    getquadtree (result);
}

void xformquadtree::selfWXform (matrixType &source, matrixType &result, int resol)
{
    setmatrix (source);int rows = S.m_rows;
    int cols = S.m_cols;
    int lr = findX2 (rows);
    int lc = findX2 (cols);
    quadnode r;
    int res;
    if ( lr > lc ) res = lc;
    else 
        res = lr;
    cout << " \n The resolution in the self WX form is " << res << " \n";
    //res = res / 2 -1;
    res = resol;
    int vertices = (int) pow(4, res) ;
    resolution = vertices;
    
    cout << " \n The vertices in the self WX form is " << vertices << " \n";
    qtree = new qtreearray();
    queue = new queuearray(vertices + 1);
    qtree->initialize (S);
    cout << " Quad Tree Initalized \n";
    /*
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
     qtree->setNode(0, r); */
    
    qtree->getStats (S, 0);
    cout << " Stats taken \n";
    qtree->getNode (0, r);
    xTemp.allocate(cols);
    yTemp.allocate(rows);
    xTemp2.allocate(cols);
    yTemp2.allocate(rows);
    
    cout << " X and Y vectors allocated \n";
    selfWXform (r);
    getmatrix (result);
}

void xformquadtree::selfPWXform (matrixType &source, matrixType &result, int resol)
{
    setmatrix (source);int rows = S.m_rows;
    int cols = S.m_cols;
    int lr = findX2 (rows);
    int lc = findX2 (cols);
    pm.totalinstructions += 2;
    quadnode r;
    int res;
    if ( lr > lc ) res = lc;
    else 
        res = lr;
    cout << " \n The resolution in the self WX form is " << res << " \n";
    //res = res / 2 -1;
    res = resol;
    int vertices = (int) pow(4, res);
    resolution = vertices;
    pm.totalinstructions += 4;
    
    cout << " \n The vertices in the self WX form is " << vertices << " \n";
    qtree = new qtreearray();
    queue = new queuearray(vertices + 1);
    qtree->initialize (S);
    cout << " Quad Tree Initalized \n";
    
    
    qtree->getStats (S, 0);
    cout << " Stats taken \n";
    qtree->getNode (0, r);
    xTemp.allocate(cols);
    yTemp.allocate(rows);
    xTemp2.allocate(cols);
    yTemp2.allocate(rows);
    pm.totalinstructions += 2 * (rows + cols);
    
    
    cout << " X and Y vectors allocated \n";
    selfPWXform (r);
    getmatrix (result);
}

void xformquadtree::selfRepWXform (matrixType &source, matrixType &result, int resol)
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
    cout << " \n The resolution in the self WX form is " << res << " \n";
    //res = res / 2 -1;
    res = resol;
    int vertices = (int) pow(4, res);
    resolution = vertices;
    
    cout << " \n The vertices in the self WX form is " << vertices << " \n";
    qtree = new qtreearray();
    queue = new queuearray(vertices + 1);
    qtree->initialize (S);
    cout << " Quad Tree Initalized \n";
    /*
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
     qtree->setNode(0, r); */
    
    qtree->getStats (S, 0);
    cout << " Stats taken \n";
    qtree->getNode (0, r);
    xTemp.allocate(cols);
    yTemp.allocate(rows);
    xTemp2.allocate(cols);
    yTemp2.allocate(rows);
    pm.totalinstructions += 2 * (rows + cols);
    
    cout << " X and Y vectors allocated \n";
    for ( i = 0; i < res; i++)
    {
        selfRowWXform(r);
        selfColWXform(r);
    }
    getmatrix (result);
}


void xformquadtree::selfWIXform (matrixType &source, matrixType &result)
{/*
    quadnode r;
    r.xul = 0 ;
    r.yul = 0;
    r.xur = 0;
    r.yll = 0;
    r.xll = source.m_rows ;
    r.xlr = source.m_rows;
    r.yur = source.m_cols;
    r.ylr = source.m_cols; */
    quadnode r;
    int lr, lc, res;
    int rows, cols;
    int index;
    cout << "Allocated quadnode and set values \n";
    setmatrix(source);
    delete qtree;
    qtree = new qtreearray ();
    qtree->initialize (S);
    qtree->getNode(0,r);
    rows = S.m_rows;
    cols = S.m_cols;
    lr = findX2 (rows);
    lc = findX2 (cols);
    if ( lr > lc ) res = lc;
    else 
        res = lr;
    cout << " \n The resolution in the self WX form is " << res << " \n";
    res = res / 2 - 1;
    res = 6;
    int vertices = (int) pow(4, res);
    resolution = vertices;
    astack = new stackarray (vertices);
    int qtlength = qtree->getLength();
    pm.totalinstructions += 12;
    //for ( index = vertices; index < qtlength; index++) qtree->killNode(index);
    selfWIXform (r);
    getmatrix(result);
}

void xformquadtree::selfWIXform (matrixType &source, matrixType &result, int resol)
{/*
 quadnode r;
 r.xul = 0 ;
 r.yul = 0;
 r.xur = 0;
 r.yll = 0;
 r.xll = source.m_rows ;
 r.xlr = source.m_rows;
 r.yur = source.m_cols;
 r.ylr = source.m_cols; */
    quadnode r;
    int lr, lc, res;
    int rows, cols;
    int index;
    cout << "Allocated quadnode and set values \n";
    setmatrix(source);
    delete qtree;
    qtree = new qtreearray ();
    qtree->initialize (S);
    qtree->getNode(0,r);
    rows = S.m_rows;
    cols = S.m_cols;
    lr = findX2 (rows);
    lc = findX2 (cols);
    if ( lr > lc ) res = lc;
    else 
        res = lr;
    cout << " \n The resolution in the self WX form is " << res << " \n";
    res = res / 2 - 1;
    res = resol;
    int vertices = (int) pow(4, res);
    resolution = vertices;
    astack = new stackarray (vertices);
    int qtlength = qtree->getLength();
    pm.totalinstructions += 12;
    //for ( index = vertices; index < qtlength; index++) qtree->killNode(index);
    selfWIXform (r);
    getmatrix(result);
}

void xformquadtree::selfPWIXform (matrixType &source, matrixType &result, int resol)
{/*
 quadnode r;
 r.xul = 0 ;
 r.yul = 0;
 r.xur = 0;
 r.yll = 0;
 r.xll = source.m_rows ;
 r.xlr = source.m_rows;
 r.yur = source.m_cols;
 r.ylr = source.m_cols; */
    quadnode r;
    int lr, lc, res;
    int rows, cols;
    int index;
    cout << "Allocated quadnode and set values \n";
    setmatrix(source);
    delete qtree;
    qtree = new qtreearray ();
    qtree->initialize (S);
    qtree->getNode(0,r);
    rows = S.m_rows;
    cols = S.m_cols;
    lr = findX2 (rows);
    lc = findX2 (cols);
    if ( lr > lc ) res = lc;
    else 
        res = lr;
    cout << " \n The resolution in the self WX form is " << res << " \n";
    res = res / 2 - 1;
    res = resol;
    int vertices = (int) pow(4, res);
    resolution = vertices;
    astack = new stackarray (vertices);
    int qtlength = qtree->getLength();
    pm.totalinstructions += 12;
    //for ( index = vertices; index < qtlength; index++) qtree->killNode(index);
    selfPWIXform (r);
    getmatrix(result);
}


void xformquadtree::selfRepWIXform (matrixType &source, matrixType &result, int resol)
{/*
 quadnode r;
 r.xul = 0 ;
 r.yul = 0;
 r.xur = 0;
 r.yll = 0;
 r.xll = source.m_rows ;
 r.xlr = source.m_rows;
 r.yur = source.m_cols;
 r.ylr = source.m_cols; */
    quadnode r;
    int lr, lc, res;
    int rows, cols;
    int index;
    int i;
    cout << "Allocated quadnode and set values \n";
    setmatrix(source);
    delete qtree;
    qtree = new qtreearray ();
    qtree->initialize (S);
    qtree->getNode(0,r);
    rows = S.m_rows;
    cols = S.m_cols;
    lr = findX2 (rows);
    lc = findX2 (cols);
    if ( lr > lc ) res = lc;
    else 
        res = lr;
    cout << " \n The resolution in the self WX form is " << res << " \n";
    res = res / 2 - 1;
    res = resol;
    int vertices = (int) pow(4, res);
    resolution = vertices;
    astack = new stackarray (vertices);
    int qtlength = qtree->getLength();
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


void xformquadtree::selfWIXform (qtreearray &source, matrixType &result)
{
    setquadtree(source);
    quadnode r;
    qtree->getNode (0,r);
    selfWIXform (r);
    getmatrix(result);
}



