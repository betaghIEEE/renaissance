

class queuearray
{
    private quadnode q[];
    private int N, head, tail;
    public queuearray (int size) 
    { 
        N = size + 1; 
        q = new quadnode[size + 1];  
        head = N; 
        tail = 0;
    }
    
    public boolean empty ()
    {
        if ( head % N == tail ) return true;
        return false;
    }
    
    public void put (quadnode myItem)
    {
        q[tail] = myItem;
        tail++;
        tail = tail % N;
    }
    
    public quadnode pop ()
    {
        head = head % N;
        return q[head++];
    }
    
    public quadnode get () { return pop (); }
    
    
}