#include "point2I.hpp"
#include "digits.hpp"

Point2I::Point2I() : valarray<int> (2)
{
}

Point2I::Point2I(int x, int y) : valarray<int> (2)
{
  (*this)[0] = x;
  (*this)[1] = y;
}

Point2I::Point2I(const Point2I& p) : valarray<int> (2)
{
  (*this)[0] = p[0];
  (*this)[1] = p[1];
}

const Point2I Point2I::operator=(const Point2I& p)
{
  (*this)[0] = p[0];
  (*this)[1] = p[1];
  
  return (*this);
}
const Point2I Point2I::operator=(const valarray<int>&v)
{
  (*this)[0] = v[0];
  (*this)[1] = v[1];
  
  return (*this);
}

const Point2I Point2I::operator=(const Point3D &p)
{
  (*this)[0] = int(p[0]/p[2]+0.5);
  (*this)[1] = int(p[1]/p[2]+0.5);
  
  return (*this);
}

const ColorImage drawPoints(const ColorImage &image, vector<Point2I> &points, int boxRadius,
			     unsigned char red, unsigned char green, unsigned char blue)
{
  ColorImage I = image;
  int r, c, k, l;
  int rows = I.getRowSize();
  int cols = I.getColSize();

  int p;
  for (p=0; p<points.size(); p++) {

    r = points[p][0];
    c = points[p][1];

    for (k=-boxRadius; k<=boxRadius; k++) 
      for (l=-boxRadius; l<=boxRadius; l+= 2*boxRadius) {
	if (r+k>=0 && r+k<rows && c+l>=0 && c+l<cols) {
	  I[r+k][c+l][0] = red;
	  I[r+k][c+l][1] = green;
	  I[r+k][c+l][2] = blue;
	}
	if (r+l>=0 && r+l<rows && c+k>=0 && c+k<cols) {
	  I[r+l][c+k][0] = red;
	  I[r+l][c+k][1] = green;
	  I[r+l][c+k][2] = blue;
	}
      }
  }
  return I;
}

const ColorImage drawPoints(const ColorImage &image, list<Point2I> &points, int boxRadius,
														unsigned char red, unsigned char green, unsigned char blue)
{
  ColorImage I = image;
  int r, c, k, l;
  int rows = I.getRowSize();
  int cols = I.getColSize();
  list<Point2I>::iterator p;
	
  for (p=points.begin(); p!=points.end(); p++) {
		
    r = (*p)[0];
    c = (*p)[1];
		
    for (k=-boxRadius; k<=boxRadius; k++) 
      for (l=-boxRadius; l<=boxRadius; l+= 2*boxRadius) {
				if (r+k>=0 && r+k<rows && c+l>=0 && c+l<cols) {
					I[r+k][c+l][0] = red;
					I[r+k][c+l][1] = green;
					I[r+k][c+l][2] = blue;
				}
				if (r+l>=0 && r+l<rows && c+k>=0 && c+k<cols) {
					I[r+l][c+k][0] = red;
					I[r+l][c+k][1] = green;
					I[r+l][c+k][2] = blue;
				}
      }
  }
		return I;
}

// Takes the corner id, marks an x at the spot of the corner
// and places a label to the left.
// The label can be changed by modifying the label images
// will crash is 0-9.pbm aren't in the same directory
const ColorImage labelPoints(const ColorImage &image, vector<Point2I> &points, 
			      unsigned char red, unsigned char green, unsigned char blue)
{
  char integerString[10];

  ColorImage I = image;

  int pr, pc, i, k, r, c;
  int id;

  int p;

  for (p=0; p<points.size(); p++) {

    id = p;
    pr = points[p][0];
    pc = points[p][1];

    //first draw the X
    for (k=-2; k<=2; k++) {
      if (pr+k>=0 && pr+k<I.getRowSize() && pc+k>=0 && pc+k<I.getColSize()) {
	I[pr+k][pc+k][0] = red;
	I[pr+k][pc+k][1] = green;
	I[pr+k][pc+k][2] = blue;
      }
      if (pr+k>=0 && pr+k<I.getRowSize() && pc-k>=0 && pc-k<I.getColSize()) {
	I[pr+k][pc-k][0] = red;
	I[pr+k][pc-k][1] = green;
	I[pr+k][pc-k][2] = blue;
       }
    }

    //now draw the digits

    int digit = 0;
    int dr = -int(digitRows[digit])/2;
    int dc = 4;
    int r2, c2;

    sprintf(integerString, "%d", id);
    for (i=0; i<strlen(integerString); i++) {
      digit = integerString[i]-'0';
      if (digit >=0 && digit <= 9) {
	for (r=0; r<digitRows[digit]; r++)
	  for (c=0; c<digitCols[digit]; c++) {

	    r2 = pr+dr+r;
	    c2 = pc+dc+c;

	    if (r2>=0 && r2<I.getRowSize() && c2>=0 && c2<I.getColSize()) {
	      switch (digit) {
	      case 0:
		I[r2][c2][0] = ((255-_digitI0[r][c])&red);//   | I[r2][c2][0];
		I[r2][c2][1] = ((255-_digitI0[r][c])&green);// | I[r2][c2][1];
		I[r2][c2][2] = ((255-_digitI0[r][c])&blue);//  | I[r2][c2][2];
		break;
	      case 1:
		I[r2][c2][0] = ((255-_digitI1[r][c])&red);//   | I[r2][c2][0];
		I[r2][c2][1] = ((255-_digitI1[r][c])&green);// | I[r2][c2][1];
		I[r2][c2][2] = ((255-_digitI1[r][c])&blue);//  | I[r2][c2][2];
		break;
	      case 2:
		I[r2][c2][0] = ((255-_digitI2[r][c])&red);//   | I[r2][c2][0];
		I[r2][c2][1] = ((255-_digitI2[r][c])&green);// | I[r2][c2][1];
		I[r2][c2][2] = ((255-_digitI2[r][c])&blue);//  | I[r2][c2][2];
		break;
	      case 3:
		I[r2][c2][0] = ((255-_digitI3[r][c])&red);//   | I[r2][c2][0];
		I[r2][c2][1] = ((255-_digitI3[r][c])&green);// | I[r2][c2][1];
		I[r2][c2][2] = ((255-_digitI3[r][c])&blue);//  | I[r2][c2][2];
		break;
	      case 4:
		I[r2][c2][0] = ((255-_digitI4[r][c])&red);//   | I[r2][c2][0];
		I[r2][c2][1] = ((255-_digitI4[r][c])&green);// | I[r2][c2][1];
		I[r2][c2][2] = ((255-_digitI4[r][c])&blue);//  | I[r2][c2][2];
		break;
	      case 5:
		I[r2][c2][0] = ((255-_digitI5[r][c])&red);//   | I[r2][c2][0];
		I[r2][c2][1] = ((255-_digitI5[r][c])&green);// | I[r2][c2][1];
		I[r2][c2][2] = ((255-_digitI5[r][c])&blue);//  | I[r2][c2][2];
		break;
	      case 6:
		I[r2][c2][0] = ((255-_digitI6[r][c])&red);//   | I[r2][c2][0];
		I[r2][c2][1] = ((255-_digitI6[r][c])&green);// | I[r2][c2][1];
		I[r2][c2][2] = ((255-_digitI6[r][c])&blue);//  | I[r2][c2][2];
		break;
	      case 7:
		I[r2][c2][0] = ((255-_digitI7[r][c])&red);//   | I[r2][c2][0];
		I[r2][c2][1] = ((255-_digitI7[r][c])&green);// | I[r2][c2][1];
		I[r2][c2][2] = ((255-_digitI7[r][c])&blue);//  | I[r2][c2][2];
		break;
	      case 8:
		I[r2][c2][0] = ((255-_digitI8[r][c])&red);//   | I[r2][c2][0];
		I[r2][c2][1] = ((255-_digitI8[r][c])&green);// | I[r2][c2][1];
		I[r2][c2][2] = ((255-_digitI8[r][c])&blue);//  | I[r2][c2][2];
		break;
	      case 9:
		I[r2][c2][0] = ((255-_digitI9[r][c])&red);//   | I[r2][c2][0];
		I[r2][c2][1] = ((255-_digitI9[r][c])&green);// | I[r2][c2][1];
		I[r2][c2][2] = ((255-_digitI9[r][c])&blue);//  | I[r2][c2][2];
		break;
	      default:
		break;
	      }
	    }

	  }
	}
      dc += digitCols[digit];
    }
  }
  return I;
}

// Takes the corner id, marks an x at the spot of the corner
// and places a label to the left.
// The label can be changed by modifying the label images
// will crash is 0-9.pbm aren't in the same directory
const ColorImage labelPoints(const ColorImage &image, vector<Point2I> &points, vector<int> &ids,
			      unsigned char red, unsigned char green, unsigned char blue)
{
  char integerString[10];

  ColorImage I = image;

  int pr, pc, i, k, r, c;
  int id;

  int p;

  for (p=0; p<points.size(); p++) {

    id = ids[p];
    pr = points[p][0];
    pc = points[p][1];

    //first draw the X
    for (k=-2; k<=2; k++) {
      if (pr+k>=0 && pr+k<I.getRowSize() && pc+k>=0 && pc+k<I.getColSize()) {
	I[pr+k][pc+k][0] = red;
	I[pr+k][pc+k][1] = green;
	I[pr+k][pc+k][2] = blue;
      }
      if (pr+k>=0 && pr+k<I.getRowSize() && pc-k>=0 && pc-k<I.getColSize()) {
	I[pr+k][pc-k][0] = red;
	I[pr+k][pc-k][1] = green;
	I[pr+k][pc-k][2] = blue;
       }
    }

    //now draw the digits

    int digit = 0;
    int dr = -int(digitRows[digit])/2;
    int dc = 4;
    int r2, c2;

    sprintf(integerString, "%d", id);
    for (i=0; i<strlen(integerString); i++) {
      digit = integerString[i]-'0';
      if (digit >=0 && digit <= 9) {
	for (r=0; r<digitRows[digit]; r++)
	  for (c=0; c<digitCols[digit]; c++) {

	    r2 = pr+dr+r;
	    c2 = pc+dc+c;

	    if (r2>=0 && r2<I.getRowSize() && c2>=0 && c2<I.getColSize()) {
	      switch (digit) {
	      case 0:
		I[r2][c2][0] = ((255-_digitI0[r][c])&red);//   | I[r2][c2][0];
		I[r2][c2][1] = ((255-_digitI0[r][c])&green);// | I[r2][c2][1];
		I[r2][c2][2] = ((255-_digitI0[r][c])&blue);//  | I[r2][c2][2];
		break;
	      case 1:
		I[r2][c2][0] = ((255-_digitI1[r][c])&red);//   | I[r2][c2][0];
		I[r2][c2][1] = ((255-_digitI1[r][c])&green);// | I[r2][c2][1];
		I[r2][c2][2] = ((255-_digitI1[r][c])&blue);//  | I[r2][c2][2];
		break;
	      case 2:
		I[r2][c2][0] = ((255-_digitI2[r][c])&red);//   | I[r2][c2][0];
		I[r2][c2][1] = ((255-_digitI2[r][c])&green);// | I[r2][c2][1];
		I[r2][c2][2] = ((255-_digitI2[r][c])&blue);//  | I[r2][c2][2];
		break;
	      case 3:
		I[r2][c2][0] = ((255-_digitI3[r][c])&red);//   | I[r2][c2][0];
		I[r2][c2][1] = ((255-_digitI3[r][c])&green);// | I[r2][c2][1];
		I[r2][c2][2] = ((255-_digitI3[r][c])&blue);//  | I[r2][c2][2];
		break;
	      case 4:
		I[r2][c2][0] = ((255-_digitI4[r][c])&red);//   | I[r2][c2][0];
		I[r2][c2][1] = ((255-_digitI4[r][c])&green);// | I[r2][c2][1];
		I[r2][c2][2] = ((255-_digitI4[r][c])&blue);//  | I[r2][c2][2];
		break;
	      case 5:
		I[r2][c2][0] = ((255-_digitI5[r][c])&red);//   | I[r2][c2][0];
		I[r2][c2][1] = ((255-_digitI5[r][c])&green);// | I[r2][c2][1];
		I[r2][c2][2] = ((255-_digitI5[r][c])&blue);//  | I[r2][c2][2];
		break;
	      case 6:
		I[r2][c2][0] = ((255-_digitI6[r][c])&red);//   | I[r2][c2][0];
		I[r2][c2][1] = ((255-_digitI6[r][c])&green);// | I[r2][c2][1];
		I[r2][c2][2] = ((255-_digitI6[r][c])&blue);//  | I[r2][c2][2];
		break;
	      case 7:
		I[r2][c2][0] = ((255-_digitI7[r][c])&red);//   | I[r2][c2][0];
		I[r2][c2][1] = ((255-_digitI7[r][c])&green);// | I[r2][c2][1];
		I[r2][c2][2] = ((255-_digitI7[r][c])&blue);//  | I[r2][c2][2];
		break;
	      case 8:
		I[r2][c2][0] = ((255-_digitI8[r][c])&red);//   | I[r2][c2][0];
		I[r2][c2][1] = ((255-_digitI8[r][c])&green);// | I[r2][c2][1];
		I[r2][c2][2] = ((255-_digitI8[r][c])&blue);//  | I[r2][c2][2];
		break;
	      case 9:
		I[r2][c2][0] = ((255-_digitI9[r][c])&red);//   | I[r2][c2][0];
		I[r2][c2][1] = ((255-_digitI9[r][c])&green);// | I[r2][c2][1];
		I[r2][c2][2] = ((255-_digitI9[r][c])&blue);//  | I[r2][c2][2];
		break;
	      default:
		break;
	      }
	    }

	  }
	}
      dc += digitCols[digit];
    }
  }
  return I;
}

void markPoints(char *outfile, const ColorImage &image, vector<Point2I> &points,
		unsigned char red, unsigned char green, unsigned char blue)
{
  ColorImage I = image;

  int pr, pc, i, k, r, c;
  int p;

  for (p=0; p<points.size(); p++) {
    
    pr = points[p][0];
    pc = points[p][1];

    //first draw the X
    for (k=-2; k<=2; k++) {
      if (pr+k>=0 && pr+k<I.getRowSize() && pc+k>=0 && pc+k<I.getColSize()) {
	I[pr+k][pc+k][0] = red;
	I[pr+k][pc+k][1] = green;
	I[pr+k][pc+k][2] = blue;
      }
      if (pr+k>=0 && pr+k<I.getRowSize() && pc-k>=0 && pc-k<I.getColSize()) {
	I[pr+k][pc-k][0] = red;
	I[pr+k][pc-k][1] = green;
	I[pr+k][pc-k][2] = blue;
       }
    }
  }
    
  I.save(outfile);
}

void save_Point2I_vector(vector<Point2I> &pts, const char *filename)
{
  ofstream outfile(filename);
  outfile << pts.size() << endl;

  for (int i=0; i<pts.size(); i++)
    outfile << pts[i][0] << "\t" << pts[i][1] << endl;

  outfile.close();
}

vector<Point2I> load_Point2I_vector(const char *filename)
{
  int num;
  int x, y;
  ifstream infile(filename);

  //first get number of pts;
  infile >> num;
  vector<Point2I> pts(num);

  for (int i=0; i<num; i++) {
    infile >> pts[i][0];
    infile >> pts[i][1];
  }

  return pts;
}


