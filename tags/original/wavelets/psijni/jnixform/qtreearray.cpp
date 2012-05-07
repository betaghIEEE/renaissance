#include "qtreearray.hpp"
#include <stddef.h>
#include <iostream.h>

qtreearray::qtreearray ()
{
    current = 0;
    tree = NULL;
    length = 0;
}

qtreearray::qtreearray (int length)
{
    allocate (length);
    current = 0;
}

int qtreearray::findX2 (int i)
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


bool qtreearray::initialize (int i)
{
	tree[i].xul = 0;
	tree[i].yul = 0;
	tree[i].xur = 0;
	tree[i].yur = 0;
	tree[i].xll = 0;
	tree[i].yll = 0;
	tree[i].xlr = 0;
	tree[i].ylr = 0;
	tree[i].rows = 0;
	tree[i].cols = 0;
	tree[i].L1energy = 0.0;
	tree[i].L2energy = 0.0;
	tree[i].max = 0.0;
	tree[i].min = 0.0;
	tree[i].epsilon = 0.0;
	tree[i].length = 0;
	tree[i].isnotnull = 0;
	tree[i].id = i;
	tree[i].NUMBER_OF_GREY_LEVELS = 0;
	
}

bool qtreearray::initialize (matrixType &S)
{
    int row = S.m_rows;
    int col = S.m_cols;
    int i, test;
    int lc = findX2(col);
    int lr = findX2 (row);
    int lp;
    if ( lc < lr ) lp = lc; else lp = lr;
    length = (int) pow (4, lp);
    if ( tree != NULL) delete []tree;
    tree = new quadnode [length];
    
    tree[0].xul = 0 ;
    tree[0].yul = 0;
    tree[0].xur = 0;
    tree[0].yll = 0;
    
    
    tree[0].xll = S.m_rows -1;
    tree[0].xlr = S.m_rows -1;
    tree[0].yur = S.m_cols -1;
    tree[0].ylr = S.m_cols -1;
    
    tree[0].rows = row;
    tree[0].cols = col;
    tree[0].epsilon = 0.0;
    
    for ( i = 1; i < length; i++)
    {
        test = (i -1) % 4;
        switch ( test) {
            case 0: 
                initAnode (i);
                break;
            case 1: 
                initVnode (i);
                break;
            case 2: 
                initHnode (i);
                break;
            case 3: 
                initDnode(i);
                break;
        }
    }
}

void qtreearray::setIsNotNull (int value)
{
    tree[current].isnotnull = value;
}

void qtreearray::setIsNotNull (int value, int index)
{
    tree[index].isnotnull = value;
}

int qtreearray::getIsNotNull ()
{
    return tree[current].isnotnull;
}

int qtreearray::getIsNotNull (int index)
{
    return tree[index].isnotnull;
}

void qtreearray::initAnode ( int i)
{
    int root = (i -1 ) / 4;
    tree[i].rows = tree[root].rows/2;
    tree[i].cols = tree[root].cols/2;
    tree[i].xul = tree[root].xul;
    tree[i].xll = tree[root].xul + tree[i].rows - 1;
    tree[i].xur = tree[root].xur;
    tree[i].xlr = tree[root].xur + tree[i].rows - 1;
    tree[i].yul = tree[root].yul;
    tree[i].yur = tree[root].yul + tree[i].cols - 1;
    tree[i].yll = tree[root].yll;
    tree[i].ylr = tree[root].yll + tree[i].cols - 1;
    tree[i].isnotnull = 0;
    tree[i].id = i;
    tree[i].epsilon = tree[root].epsilon;
    
}

void qtreearray::initVnode ( int i)
{
    int root = (i -1 ) / 4;
    tree[i].rows = tree[root].rows/2;
    tree[i].cols = tree[root].cols/2;
    tree[i].xul = tree[root].xul;
    tree[i].xur = tree[root].xur;
    tree[i].yur = tree[root].yur;
    tree[i].ylr = tree[root].ylr;
    
    tree[i].xll = tree[root].xul + tree[i].rows - 1;
    tree[i].xlr = tree[root].xur + tree[i].rows - 1;
    tree[i].yul = tree[root].yul + tree[i].cols;
    tree[i].yll = tree[root].yul + tree[i].cols;
    tree[i].isnotnull = 0;
    tree[i].id = i;
    tree[i].epsilon = tree[root].epsilon;
}

void qtreearray::initHnode ( int i)
{
    
    int root = (i -1 ) / 4;
    tree[i].rows = tree[root].rows/2;
    tree[i].cols = tree[root].cols/2;
    tree[i].xll = tree[root].xll;
    tree[i].xlr = tree[root].xlr;
    tree[i].yul = tree[root].yul;
    tree[i].yll = tree[root].yll;
    
    tree[i].xul = tree[root].xul + tree[i].rows;
    tree[i].xur = tree[root].xur + tree[i].rows;
    tree[i].yur = tree[root].yul + tree[i].cols - 1;
    tree[i].ylr = tree[root].yll + tree[i].cols - 1;
    tree[i].isnotnull = 0;
    tree[i].id = i;
    tree[i].epsilon = tree[root].epsilon;
}

void qtreearray::initDnode ( int i)
{
    
    int root = (i -1 ) / 4;
    tree[i].rows = tree[root].rows/2;
    tree[i].cols = tree[root].cols/2;
    tree[i].xul = tree[root].xul + tree[i].rows;
    tree[i].xll = tree[root].xll;
    tree[i].xur = tree[root].xur + tree[i].rows;
    tree[i].xlr = tree[root].xlr;
    tree[i].yul = tree[root].yul + tree[i].cols;
    tree[i].yll = tree[root].yul + tree[i].cols;
    tree[i].yur = tree[root].yur;
    tree[i].ylr = tree[root].ylr;
    tree[i].isnotnull = 0;
    tree[i].id = i;
    tree[i].epsilon = tree[root].epsilon;
}

void qtreearray::getStats ( matrixType &S)
{
    double sum = 0.0;
    double sum2 = 0.0;
    double max = 0.0;
    double min = pow (10.0,37.0);
    int i, j;
    int index = current;
    for ( i = tree[index].xul; i < tree[index].xll; i++)
        for ( j = tree[index].yul; j < tree[index].yur; j++)
        {
            if ( max <= S[i][j]) max = S[i][j] ;
            if ( min >= S[i][j]) min = S[i][j];
            sum += S[i][j];
            sum2 += S[i][j] * S[i][j];
        }
            tree[index].max = max;
    tree[index].min = min;
    tree[index].L1energy = sum;
    tree[index].L2energy = sum2;
}

void qtreearray::getStats ( matrixType &S, int index)
{
    double sum = 0.0;
    double sum2 = 0.0;
    double max = 0.0;
    double min = pow (10.0,37.0);
    int i, j;
    for ( i = tree[index].xul; i < tree[index].xll; i++)
        for ( j = tree[index].yul; j < tree[index].yur; j++)
        {
            if ( max <= S[i][j]) max = S[i][j] ;
            if ( min >= S[i][j]) min = S[i][j];
            sum += S[i][j];
            sum2 += S[i][j] * S[i][j];
        }
            tree[index].max = max;
    tree[index].min = min;
    tree[index].L1energy = sum;
    tree[index].L2energy = sum2;
    tree[index].isnotnull = 1;
}

bool qtreearray::allocate (int slength)
{
	length = slength;
	tree = new quadnode [slength];
	int i;
	for ( i = 0; i < slength; i ++)
	{
		initialize(i);
	}
}

bool qtreearray::deallocate ()
{
	if ( tree != NULL)
	{
		delete tree;
		tree = NULL;
	}
}

bool qtreearray::getNode (int i, quadnode &value )
{
    cout << " " << i << " is being copied \n";
    value.id = i;
    if ( i < length ) 
	value.copy (tree[i]); 
        /*
    {
        tree[i].xll = value.xll;
        tree[i].xul = value.xul;
        tree[i].xur = value.xur;
        tree[i].xlr = value.xlr;
        tree[i].yll = value.yll;
        tree[i].yul = value.yul;
        tree[i].yur = value.yur;
        tree[i].ylr = value.ylr;
        tree[i].rows = value.rows;
        tree[i].cols = value.cols;
        tree[i].epsilon = value.epsilon;
        tree[i].L2energy = value.L2energy;
        tree[i].L1energy = value.L1energy;
        tree[i].max = value.max;
        tree[i].min = value.min;        
        tree[i].isnotnull = value.isnotnull;
        tree[i].NUMBER_OF_GREY_LEVELS = value.NUMBER_OF_GREY_LEVELS;
        
    }*/
    else return false;
    return true;
}

bool qtreearray::setNode (int i, quadnode &value )
{
    cout << " Setting " << i << "\n";
    if ( i < length) {
        cout << " Copying qnode \n";
        value.id = i;
	tree[i].copy(value);
        cout << " Copied qnode \n";
	tree[i].isnotnull = 1;
        cout << " Setting complete \n "; 
        return true;
    }
    cout << "Setting false \n";
    return false;
}

bool qtreearray::div4 (int i, int base)
{
    int temp = i;
    while ( temp > base )
    {
        temp = (temp - 1) /4;
    }
    if ( temp == base) return true;
    return false;
}

void qtreearray::killNode ()
{
    int count = current;
    int i;
    int a, v,h, d;
        if (!isleaf())
        {
            for ( i = count; i < length; i++)
            {
                if ( div4 (i, count)) killNode (i);
            }
        }
	if (current > 0 ) current = (current - 1) / 4;
}

void qtreearray::killNode (int i)
{
    cout << " Killing node " << i << "\n";
    if ( i < length) tree[i].isnotnull = 0;
}

void qtreearray::killNode (quadnode &value)
{
    int id = value.id;
    killNode (id);
    killNode ( id * 4 +1);
    killNode ( id * 4 +2);
    killNode ( id * 4 +3);
    killNode ( id * 4 +4);
    
    
}

void qtreearray::getNode (quadnode &value)
{
	value.copy (tree[current]);
}

void qtreearray::setNode (quadnode &value)
{
	tree[current].copy (value);
}




bool qtreearray::gotoNode (int i)
{
    if ( i < length)
	current = i;
    else return false;
    return true;
}

int qtreearray::getA()
{
	int a = current * 4 + 1;
	return a;
}

int qtreearray::getH()
{
	int a = current * 4 + 3;
	return a;
}

int qtreearray::getD()
{
	int a = current * 4 + 4;
	return a;
}

int qtreearray::getV()
{
	int a = current * 4 + 2;
	return a;
}

int qtreearray::getA(int pos)
{
	int a = pos * 4 + 1;
	return a;
}

int qtreearray::getH(int pos)
{
	int a = pos * 4 + 3;
	return a;
}

int qtreearray::getD(int pos)
{
	int a = pos * 4 + 4;
	return a;
}

int qtreearray::getV(int pos)
{
	int a = pos * 4 + 2;
	return a;
}


int qtreearray::getLength() { return length;}

int qtreearray::isleaf ()
{
    int i = current;
    if ( !tree[i * 4 + 1].isnotnull) return 0;
    if ( !tree[i * 4 + 2].isnotnull) return 0;
    if ( !tree[i * 4 + 3].isnotnull) return 0;
    if ( !tree[i * 4 + 4].isnotnull) return 0;
    return 1;
	
}

int qtreearray::isleaf (int i)
{
    if ( !tree[i * 4 + 1].isnotnull) return 0;
    if ( !tree[i * 4 + 2].isnotnull) return 0;
    if ( !tree[i * 4 + 3].isnotnull) return 0;
    if ( !tree[i * 4 + 4].isnotnull) return 0;
	return 1;
	
}

int qtreearray::isgrandparent ()
{
	if (!isleaf(getA())) return 1;
	if (!isleaf(getV())) return 1;
	if (!isleaf(getH())) return 1;
	if (!isleaf(getD())) return 1;
	return 0;
}

int qtreearray::isgrandparent (int i )
{
    if (!isleaf(getA(i))) return 1;
    if (!isleaf(getV(i))) return 1;
    if (!isleaf(getH(i))) return 1;
    if (!isleaf(getD(i))) return 1;
    return 0;
}

