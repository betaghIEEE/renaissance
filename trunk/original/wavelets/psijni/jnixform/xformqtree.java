
class xformqtreeinterface {
    private qtreearray mytree;
    
    public void getA(){ mytree.getA(); }
    
    
    public void getD(){ mytree.getD(); }
    public void getH(){ mytree.getH(); }
    public void getV(){ mytree.getV(); }
    public int getCurrentXUL () { return mytree.getXUL();}
    public int getCurrentYUL () { return mytree.getYUL();}
    public int getCurrentXUR () { return mytree.getXUR();}
    public int getCurrentYUR () { return mytree.getYUR();}
    public int getCurrentXLL () { return mytree.getXLL();}
    public int getCurrentYLL () { return mytree.getYLL();}
    public int getCurrentXLR () { return mytree.getXLR();}
    public int getCurrentYLR () { return mytree.getYLR();}
    public double getCurrentL1Energy () { return mytree.getL1Energy(); }
    public double getCurrentL2Energy () { return mytree.getL2Energy(); }
    public double getCurrentMax () { return mytree.getMax(); }
    public double getCurrentMin () { return mytree.getMin(); }
    public int getCurrentIsNull () { return mytree.isNull(); }
    public int getCurrentLength () { return mytree.getLength(); }
    public int getCurrentID () { return mytree.getID(); }
    public double getCurrentValues (int index) { return mytree.getValue (index); }
    public void gotoIndex (int i) {mytree.gotoNode(i) ;}
    public int getParent () {return mytree.getParent();}
    
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
        if ( java.lang.math.abs (x) > th ) return false;
        return true;
    }
    
    private void getSstats (quadnode r)
    {
        double sum = 0.0;
        double sum2 = 0.0;
        double max = 0.0;
        double min = java.lang.math.pow (10.0,37.0);
        int i, j;
        for ( i = 0; i < r.length; i++)
        {
            if ( max <= r.values[i] ) max = r.values[i];
            if ( min >= r.values[i] ) max = r.values[i];
            sum += r.values[i];
            sum2 += r.values[i] * r.values[i];
        }
        r.max = max;
        r.min = min;
        r.L1energy = sum;
        r.L2energy = sum2;
    }
    
    private quadnode setAnode (quadnode r)
    {
        quadnode a = new quadnode();
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
        } else a.isnotnull = 0;
        return a;
    }
    
    private quadnode setVnode (quadnode r)
    {
        quadnode a = new quadnode ();
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
        } else a.isnotnull = 0;
        return a;
    }
    
    private quadnode setHnode (quadnode r)
    {
        quadnode a = new quadnode ();
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
        } else a.isnotnull = 0;
        return a;
    }
    
    private quadnode setDnode (quadnode r)
    {
        quadnode a = new quadnode ();
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
        } else a.isnotnull = 0;
        return a;
    }
    
    private selfColXform ( quadnode r)
    {
        int j, k, n, l;
        int y , x, index;
        if ( (!r.isleaf) & (r.length != 0) & (r.values != NULL))
        {
            double average[] = new double [r.rows];
            double difference[] = new double [r.rows];
            for ( j = 0; j < r.cols; j++)
            {
                for ( k = 0; k < r.rows; k++)
                {
                    temp[k] = 0.0;
                }
                
                for ( k = 0; k < r.rows; k++)
                    for ( l = 0; l < ha.length; l++)
                    {
                        n = k - l; 
                        if ( (n >= 0) & (n < r.rows))
                        {
                            y = j;
                            x = n;
                            index = x * r.cols + y;
                            average[k] += r.values[index] * ha[l];
                        }
                    }
                for ( k = 0; k < r.rows; k++)
                    for ( l = 0; l < hd.length; l++)
                    {
                        n = k - l; 
                        if ( (n >= 0) & (n < r.rows))
                        {
                            y = j;
                            x = n;
                            index = x * r.cols + y;
                            difference[k] += r.values[index] * hd[l];
                        }
                    }
                for ( k = 0; k < r.rows/2; k++)
                {
                        x = k;
                        y = j;
                        index = x * r.cols + y;
                        r.values [index] = average[2 * k + 1];
                        x = k + r.rows/2;
                        y = j;
                        index = x * r.cols + y;
                        r.values [index] = difference[2 * k + 1];
                }
                        
                                
            }
            
        }
    }
        
        
        
    private selfRowXform ( quadnode r)
    {
        int j, k, n, l;
        int y , x, index;
        if ( (!r.isleaf) & (r.length != 0) & (r.values != NULL))
        {
            double average[] = new double [r.cols];
            double difference[] = new double [r.cols];
            for ( j = 0; j < r.rows; j++)
                {
                    for ( k = 0; k < r.cols; k++)
                    {
                        temp[k] = 0.0;
                    }
                    
                    for ( k = 0; k < r.cols; k++)
                        for ( l = 0; l < ha.length; l++)
                        {
                            n = k - l; 
                            if ( (n >= 0) & (n < r.cols))
                            {
                                y = n;
                                x = j;
                                index = x * r.cols + y;
                                average[k] += r.values[index] * ha[l];
                            }
                        }
                    for ( k = 0; k < r.cols; k++)
                        for ( l = 0; l < hd.length; l++)
                        {
                            n = k - l; 
                            if ( (n >= 0) & (n < r.rows))
                            {
                                y = n;
                                x = j;
                                index = x * r.cols + y;
                                difference[k] += r.values[index] * hd[l];
                            }
                        }
                    for ( k = 0; k < r.cols/2; k++)
                    {
                        x = j;
                        y = k;
                        index = x * r.cols + y;
                        r.values [index] = average[2 * k + 1];
                        x = j;
                        y = k + r.cols/2;
                        index = x * r.cols + y;
                        r.values [index] = difference[2 * k + 1];
                    }
                                    
                                    
                }
                    
            }
    }
/*    
    private void selfWXform (quadnode r)
    {
        quadnode playnode;
        getSstats (r);
        queuearray q = new queuearray ();
        q.put(r);
        int length = qtree.length;
        while (!q.empty())
        {
            playnode = q.get();
            if ( (qtree.getA(playnode.id) < length) & (qtree.getV(playnode.id) < length) )
                if ( (qtree.getH(playnode.id) < length) & (qtree.getD(playnode.id) < length))
                {
                    selfColWXform (r);
                    selfRowWXform (r);
                    qtree[qtree.getA(playnode.id)] = setAnode(r);
                    qtree[qtree.getV(playnode.id)] = setVnode(r);
                    qtree[qtree.getH(playnode.id)] = setHnode(r);
                    qtree[qtree.getD(playnode.id)] = setDnode(r);
                    getSstat (qtree[qtree.getA(playnode.id)]);
                    getSstat (qtree[qtree.getV(playnode.id)]);
                    getSstat (qtree[qtree.getH(playnode.id)]);
                    getSstat (qtree[qtree.getD(playnode.id)]);
                    if ( !epsilon( qtree[qtree.getA(playnode.id)].L2energy , qtree[qtree.getA(playnode.id)].epsilon))
                        q.put (qtree[qtree.getA(playnode.id)]);
                    if ( !epsilon( qtree[qtree.getV(playnode.id)].L2energy , qtree[qtree.getV(playnode.id)].epsilon))
                        q.put (qtree[qtree.getV(playnode.id)]);
                    if ( !epsilon( qtree[qtree.getH(playnode.id)].L2energy , qtree[qtree.getH(playnode.id)].epsilon))
                        q.put (qtree[qtree.getH(playnode.id)]);
                    if ( !epsilon( qtree[qtree.getD(playnode.id)].L2energy , qtree[qtree.getD(playnode.id)].epsilon))
                        q.put (qtree[qtree.getD(playnode.id)]);
                }
        }
    }
      */
	private double getChildValue (quadnode r, int x, int y)
	{
		int row = r.rows;
		int col = r.cols;
		int i;
		mytree.gotoNote (r.id);
		if ( x < row/2) 
		{
			if ( y > col/2)
			{
				i = x * mytree.getAnode().cols + y;
				return mytree.getAnode().values[i];
			} else 
			{
				i = x* mytree.getVnode().cols + (y - col /2);
				return mytree.getVnode().values[i];
			}
		}
		else
		{
			if ( y > col/2)
			{
				i = (x - row/2) * mytree.getHnode().cols + y;
				return mytree.getHnode().values[i];
			} else 
			{
				i = (x - row/2) * mytree.getDnode().cols + (y - col /2);
				return mytree.getDnode().values[i];
			}
		}
	}
	
	private quadnode selfColWIXform (quadnode r)
	{
		int i, j;
		int index, index1, indexr, indexl;
		quadnode temp = new quadnode(r);
		rrow2 = r.rows /2 ;
		
		for ( j = 0; j < r.cols; j++ )
			for (i = 0; i < rrow2 ; i++)
			{
				index = (2 * i) * r.rows + j;
				index1 = (2 * i +1) * r.rows + j;
				indexl = i * r.rows + j;
				indexr = (i + rrow2) * r.rows + j;
				temp.values[index] = (r.values[indexr] - r.values[indexl]) * sqhalf;
				temp.values[index1] = (r.values[indexr] + r.values[indexl]) * sqhalf;
			}
		return temp;
	}
	
	private quadnode selfRowWIXform (quadnode r)
	{
		int i, j;
		int index, index1, indexr, indexl;
		quadnode temp = new quadnode(r);
		rcol2 = r.cols/2;
		
		for ( i = 0; i < r.rows; i++ )
			for (j = 0; j < rcol2 ; j++)
			{
				index = i * r.rows + 2 * j;
				index1 = i * r.rows + 2 * j + 1;
				indexl = i * r.rows + j;
				indexr = i * r.rows + j + rcol2;
				temp.values[index] = (r.values[indexr] - r.values[indexl]) * sqhalf;
				temp.values[index1] = (r.values[indexr] + r.values[indexl]) * sqhalf;
			}
		return temp;
	}
	
	private double[] selfRowWIXform (double r[])
	{
	}
	
    private quadnode mergechild (quadnode r)
    {
        int i, j;
        int index, urindex, ulindex, lrindex, llindex;
        mytree.gotoNode (r.id);
        double A[] = mytree.getAnode().values;
        double V[] = mytree.getVnode().values;
        double H[] = mytree.getHnode().values;
        double D[] = mytree.getDnode().values;
        int row = mytree.getAnode().rows;
        int col = mytree.getAnode().rows;
        int rows = row + row;
        int cols = col + col;
        r.rows = rows;
        r.cols = cols;
        r.length = rows * cols;
        r.values = new double[r.length];
        
        for ( i = 0; i < row; i++)
            for ( j = 0 ; j < col; j++)
            {
                urindex = i * col * 2 + j;
                ulindex = i * col * 2 + j + col;
                lrindex = (i + row) * col * 2 + j ;
                llindex = (i + row) * col * 2 + j + col;
                index = i * row + j;
                r.values[urindex] = A[index];
                r.values[ulindex] = V[index];
                r.values[lrindex] = H[index];
                r.values[llindex] = D[index];
            }
                return r;
    }
        
        
    private void selfWIXform (quadnode r)
	{
		quadnode temp;
		quadnode tempchild;
		stackarray astack = new stackarray ();
		astack.push (r);
		while (!astack.empty())
		{
			tempchild = astack.pop();
			qtree.gotoNode (tempchild.id);
			if (qtree.isgrandparent() )
			{
				astack.push(tempchild);
				if ( !qtree.getAnode().isleaf() ) astack.push (qtree.getAnode());
				if ( !qtree.getVnode().isleaf() ) astack.push (qtree.getVnode());
				if ( !qtree.getHnode().isleaf() ) astack.push (qtree.getHnode());
				if ( !qtree.getDnode().isleaf() ) astack.push (qtree.getDnode());
			} else if (!qtree.isleaf())
			{
				temp = mergechild (tempchild);
				selfRowWIXform (selfColWIXform (temp));
				
				qtree->killchildren (temp);
			}
		}
		
	}
        
    private native int WIXnative ();
    
    private native int WXnative ();
    
    public qtreearray WX (qtreearray source) { 
        mytree = source; 
        WXnative (); 
        return mytree;
    }
    
    //public qtreearray mytree ();
    public qtreearray WX ( String filename)
    {
        quadnode t;
        qtreearray qt;
        pgmio pgm = new pgmio();
        pgm.getPGM(filename);
        pgm.qnode;
        int row = X2(pgm.qnode.rows);
        int col = X2(pgm.qnode.cols);
        int level;
        int size;
        if ( row > col ) level = col;
        else level = row;
        size = java.lang.math.pow (4, level);
        qt = new qtreearray (size);
        qt.setNode (0, t);
        WX(qt);
    }
    
    public qtreearray WIX ( qtreearray source) { 
        mytree = source; 
        WIXnative(); 
        return mytree;
    }
    
    public static void main (String args[])
    {
        xformqtreeinterface xq = new xformqtreeinterface();
        xq.WX("swalla10.pgm");
        
        
    }
    
}