

class stackarray 
{
    private quadnode s[];
    private int N, size;
    public stackarray (int maxsize)
    {
        size = maxsize;
        s = new quadnode [maxsize];
        N = 0;
    }

    boolean empty () 
    {
        if ( N == 0) return true;
        return false;
    }   
    void push (quadnode item)
    {
        s[N++] = item;
    }
    quadnode pop ()
    {
        return s[--N];
    }
    
}