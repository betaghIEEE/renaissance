
class quadnode
{
    public int xul, yul;
    public int xll, yll;
    public int xur, yur;
    public int xlr, ylr;
    public int rows, cols;
    public double L1energy;
    public double L2energy;
    public double max;
    public double min;
    public double values[];
    public double epsilon;
    public int length;
    public int isnotnull;
    public int id;
    public int NUMBER_OF_GREY_LEVELS;
    public int levels;
    /*
    public void copy(quadnode source)
    {
        xul = source.xul;
        yul = source.yul;
        xur = source.xur;
        yur = source.yur;
        xll = source.xll;
        yll = source.yll;
        xlr = source.xlr;
        ylr = source.ylr;
        rows = source.rows;
        cols = source.cols;
        L1energy = source.L1energy;
        L2energy = source.L2energy;
        max = source.max;
        min = source.min;
        epsilon = source.epsilon;
        length = source.length;
        isnotnull = source.isnotnull;
        id = source.id;
        NUMBER_OF_GREY_LEVELS = source.NUMBER_OF_GREY_LEVELS;
        int i;
        for ( i = 0; i < length; i++)
            values[i] = source.values[i];
    }*/
}