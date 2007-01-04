
class xformqtree
{

    private qtreearray mytree;
    private double A[];
    private double ha[];
    private double hd[];
    private double sqhalf;
    
    private int findX2 (int i) {
        
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
    
    public boolean epsilon (double x, int th)
    {
        return epsilon (x, (double)th);
    }
    
    public boolean epsilon (double x, double th)
    {
        if ( java.lang.Math.abs (x) > th ) return false;
        return true;
    }
    
    private void getSstats (quadnode r)
    {
        double sum = 0.0;
        double sum2 = 0.0;
        double max = 0.0;
        double min = java.lang.Math.pow (10.0,37.0);
        int i, j, index;
        int x, y;
        
        for ( i = 0; i < r.rows; i++)
            for ( j = 0 ; j < r.cols; j++)
                  {
                        x = i + r.xul;
                        y = i + r.yul;
                        index = x * r.cols +j;
                      if ( max <= A[index] ) max = A[index];
                      if ( min >= A[index] ) max = A[index];
                      sum += A[i];
                      sum2 += A[i] * A[i];
                  }
        r.max = max;
        r.min = min;
        r.L1energy = sum;
        r.L2energy = sum2;
    }

    private void matrixRowWXform ( quadnode r)
    {
        int j, k, n, l;
        int y , x, index;
        if ( (r != null ) )
        {
            double average[] = new double [r.cols];
            double difference[] = new double [r.cols];
            for ( j = 0; j < r.rows; j++)
            {
                for ( k = 0; k < r.cols; k++)
                {
                    average[k] = 0.0;
                    difference[k] = 0.0;
                }
                
                for ( k = 0; k < r.cols; k++)
                    for ( l = 0; l < ha.length; l++)
                    {
                        n = k - l; 
                        if ( (n >= 0) & (n < r.cols))
                        {
                            y = n + r.yul;
                            x = j + r.xul;
                            index = x * r.cols + y;
                            average[k] += A[index] * ha[l];
                        }
                    }
                for ( k = 0; k < r.cols; k++)
                    for ( l = 0; l < hd.length; l++)
                    {
                        n = k - l; 
                        if ( (n >= 0) & (n < r.cols))
                        {
                            y = n + r.yul;
                            x = j + r.xul;
                            index = x * r.cols + y;
                            difference[k] += A[index] * hd[l];
                        }
                    }
                for ( k = 0; k < r.cols/2; k++)
                {
                        x = j + r.yul;
                        y = k + r.xul;
                        index = x * r.cols + y;
                        A [index] = average[2 * k + 1];
                        x = k + r.rows/2;
                        y = j;
                        index = x * r.cols + y;
                        A [index] = difference[2 * k + 1];
                }
                        
                                
            }
            
        }
           // return r;
    }
    
    private void matrixColWXform ( quadnode r)
    {
        int j, k, n, l;
        int y , x, index;
        if ( r != null)
        {
            double average[] = new double [r.rows];
            double difference[] = new double [r.rows];
            for ( j = 0; j < r.cols; j++)
            {
                for ( k = 0; k < r.rows; k++)
                {
                    average[k] = 0.0; difference[k] = 0.0;
                }
                
                for ( k = 0; k < r.rows; k++)
                    for ( l = 0; l < ha.length; l++)
                    {
                        n = k - l; 
                        if ( (n >= 0) & (n < r.rows))
                        {
                            y = j + r.yul;
                            x = n + r.xul;
                            index = x * r.cols + y;
                            average[k] += A[index] * ha[l];
                        }
                    }
                for ( k = 0; k < r.rows; k++)
                    for ( l = 0; l < hd.length; l++)
                    {
                        n = k - l; 
                        if ( (n >= 0) & (n < r.rows))
                        {
                            y = j + r.yul;
                            x = n + r.xul;
                            index = x * r.cols + y;
                            difference[k] += A[index] * hd[l];
                        }
                    }
                for ( k = 0; k < r.rows/2; k++)
                {
                        x = k + r.xul;
                        y = j + r.yul;
                        index = x * r.cols + y;
                        A [index] = average[2 * k + 1];
                        x = k + r.rows/2;
                        y = j;
                        index = x * r.cols + y;
                        A [index] = difference[2 * k + 1];
                }
                        
                                
            }
            
        }
           // return r;
    }
    /* Note these libraries need to be completed.
    */
    public quadnode matrixWXform ( quadnode r)
    {
        quadnode playnode;
        getSstats (r);
        
        int length = mytree.getLength();
        queuearray q = new queuearray (length);
        q.put(r);
        while (!q.empty())
        {
            playnode = q.get();
            mytree.gotoNode(playnode.id);
            if ( mytree.setChildren(r))
                {
                    matrixColWXform (r);
                    matrixRowWXform (r);
                    getSstats (mytree.getAnode());
                    getSstats (mytree.getVnode());
                    getSstats (mytree.getHnode());
                    getSstats (mytree.getDnode());
                    if ( !epsilon( mytree.getAnode().L2energy , mytree.getAnode().epsilon))
                        q.put (mytree.getAnode());
                    if ( !epsilon( mytree.getVnode().L2energy , mytree.getVnode().epsilon))
                        q.put (mytree.getVnode());
                    if ( !epsilon( mytree.getHnode().L2energy , mytree.getHnode().epsilon))
                        q.put (mytree.getHnode());
                    if ( !epsilon( mytree.getDnode().L2energy , mytree.getDnode().epsilon))
                        q.put (mytree.getDnode());
                }
        }    return r;
    }
        
        private double[] matrixColWIXform (quadnode r)
	{
            int i, j, x, y , x2, y2;
            int index, index1, indexr, indexl;
            int length = A.length;
            double temp[] = new double[length];
            int rrow2 = r.rows /2 ;
            mytree.gotoNode (r.id);
            
            for ( j = 0; j < r.cols; j++ )
                for (i = 0; i < rrow2 ; i++)
                {
                    x = i + r.xul;
                    y= j + r.yul;
                    x2 = i * 2 + r.xul;
                    y2 = j * 2 + r.yul;
                    index = (x2) * mytree.getRoot().cols + y;
                    index1 = (x2 +1) * mytree.getRoot().cols + y;
                    indexl = x * mytree.getRoot().cols + y;
                    indexr = (x + rrow2) * mytree.getRoot().cols + y;
                    temp[index] = (A[indexr] - A[indexl]) * sqhalf;
                    temp[index1] = (A[indexr] + A[indexl]) * sqhalf;
                }
                    return temp;
	}
    
        private double[] matrixRowWIXform (quadnode r)
	{
            int i, j, x,y, x2, y2;
            int index, index1, indexr, indexl;
            int length = r.rows * r.cols;
            double temp[] = new double [length];
            int rcol2 = r.cols/2;
            mytree.gotoNode (r.id);
            
            for ( i = 0; i < r.rows; i++ )
                for (j = 0; j < rcol2 ; j++)
                {
                    x = i + r.xul;
                    y= j + r.yul;
                    x2 = i * 2 + r.xul;
                    y2 = j * 2 + r.yul;
                    index = x * mytree.getRoot().cols + y2;
                    index1 = x  * mytree.getRoot().cols+ (y2 + 1);
                    indexl = x * mytree.getRoot().rows + y;
                    indexr = x * mytree.getRoot().rows + (y + rcol2);
                    temp[index] = (A[indexr] - A[indexl]) * sqhalf;
                    temp[index1] = (A[indexr] + A[indexl]) * sqhalf;
                }
                    return temp;
	}    
        
        
        private void matrixColWIXform (double temp[])
	{
            int i, j, x, y , x2, y2;
            int index, index1, indexr, indexl;
            quadnode r = mytree.getNode();
            int rrow2 = r.rows /2 ;
            
            for ( j = 0; j < r.cols; j++ )
                for (i = 0; i < rrow2 ; i++)
                {
                    x = i + r.xul;
                    y= j + r.yul;
                    x2 = i * 2 + r.xul;
                    y2 = j * 2 + r.yul;
                    index = (x2) * mytree.getRoot().cols + y;
                    index1 = (x2 +1) * mytree.getRoot().cols + y;
                    indexl = x * mytree.getRoot().cols + y;
                    indexr = (x + rrow2) * mytree.getRoot().cols + y;
                    A[index] = (temp[indexr] - temp[indexl]) * sqhalf;
                    A[index1] = (temp[indexr] + temp[indexl]) * sqhalf;
                }
                    
	}
        
        private void matrixRowWIXform (double temp[])
	{
            int i, j, x,y, x2, y2;
            int index, index1, indexr, indexl;
            quadnode r = mytree.getNode();
            int rcol2 = r.cols/2;
            
            for ( i = 0; i < r.rows; i++ )
                for (j = 0; j < rcol2 ; j++)
                {
                    x = i + r.xul;
                    y= j + r.yul;
                    x2 = i * 2 + r.xul;
                    y2 = j * 2 + r.yul;
                    index = x * mytree.getRoot().cols + y2;
                    index1 = x  * mytree.getRoot().cols+ (y2 + 1);
                    indexl = x * mytree.getRoot().rows + y;
                    indexr = x * mytree.getRoot().rows + (y + rcol2);
                    A[index] = (temp[indexr] - temp[indexl]) * sqhalf;
                    A[index1] = (temp[indexr] + temp[indexl]) * sqhalf;
                }
                   
	}          
        /*
            Add matrixRowWIXform and matrixColWIXform with the argument of a double[]
         */ 
            public void selfWIXform (quadnode r)
            {
		quadnode temp;
		quadnode tempchild;
		stackarray astack = new stackarray (mytree.getLength());
		astack.push (r);
                int index;
		while (!astack.empty())
		{
                    tempchild = astack.pop();
                    mytree.gotoNode (tempchild.id); 
                    if (mytree.isgrandparent() )
                    {
                        index = tempchild.id;
                        astack.push(tempchild);
                        if ( !mytree.isleaf(index) ) astack.push (mytree.getAnode());
                        if ( !mytree.isleaf(index) ) astack.push (mytree.getVnode());
                        if ( !mytree.isleaf(index) ) astack.push (mytree.getHnode());
                        if ( !mytree.isleaf(index) ) astack.push (mytree.getDnode());
                    } else if (!mytree.isleaf())
                    {
                        //temp = mergechild (tempchild);
                        matrixRowWIXform (matrixColWIXform (tempchild));
                        
                        //mytree->killchildren (temp);
                    } 
		}
		
            }   
            
            public quadnode selfWXform ( quadnode r)
            {
                A = r.values;
                int nodes, row, col;
                row = findX2(r.rows);
                col = findX2(r.cols);
                if ( row < col) mytree = new qtreearray (row);
                else mytree = new qtreearray (col);
                mytree.setRootNode (r);
                matrixWXform (r);
                mytree.getRoot().values = A;
                return mytree.getRoot();
            }
            
            public matrixType selfWXform (matrixType temp)
            {
                matrixType work = temp;
                quadnode r = new quadnode();
                r.length = temp.length;
                r.values = temp.A;
                r.rows = temp.rows;
                r.cols = temp.cols;
                int nodes, row, col;
                row = findX2(temp.rows);
                col = findX2(temp.cols);
                if ( row < col) mytree = new qtreearray (row);
                else mytree = new qtreearray (col);
                return work;
            }
            
}
