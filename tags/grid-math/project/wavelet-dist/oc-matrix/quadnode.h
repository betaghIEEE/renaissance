
struct quadnode
{
	int xul, yul;
    int xll, yll;
    int xur, yur;
    int xlr, ylr;
    int rows, cols;
    double L1energy;
    double L2energy;
    double max;
    double min;
    double epsilon;
    int length;
    int isnotnull;
    int myid;
    int NUMBER_OF_GREY_LEVELS;
} quadnode;