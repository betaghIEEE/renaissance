#include "pgmio.h"
#include <iostream.h>
#include <fstream.h>
#include <math.h>

void getImagePGM2 (const char* filename, JNIEnv *env, jobject obj);

JNIEXPORT void JNICALL Java_pgmio_getPGM
  (JNIEnv *env, jobject obj, jstring mystring)
  {
    jboolean test = JNI_TRUE;
    cout << "Inside Test \n";
    char best[128];
    const jchar *foobar = env->GetStringChars (mystring,&test);
    int i;
    for ( i = 0; i < 128 & foobar != '\0'; i++){
        best[i] = *foobar; foobar++;}
        
        jclass cls = env->GetObjectClass(obj);
        jmethodID setValue = env->GetMethodID (cls, "setvalue", "(I)V");
        if (setValue == NULL ) return;
        jmethodID allocate = env->GetMethodID (cls, "allocate", "(I)V");
        if (allocate == NULL ) return;
        jmethodID setGrey = env->GetMethodID (cls, "setGrey", "(I)V");
        if (setGrey == NULL ) return;
    getImagePGM2 ( best,env, obj);
  }

void getImagePGM2 (const char* filename, JNIEnv *env, jobject obj)
{
	ifstream infile(filename);
        char temp;
        int tempInt;
        int rows;
        int cols;
        int pos;
        int i, j;
        int length = 0;
        char buf[100];
        double greyLevel;
        double *pic;
        jdouble value;
        
        
	if (infile.bad()) {
		cout <<"readPGM: Unable to open file! " ;
                cout << filename;
                    exit (1);
                }
	infile.getline ( buf, 100);
	if ((buf[0] != 'P') && (buf[1] !='2') ){
		cout <<"readPGM: magic number is not P2!";
                exit (1);
                }
	
        //infile >> temp;
        cout << "\n Columns";
        tempInt = 0;
        //while ((temp >= '0') && (temp <='9')){
           /// tempInt = tempInt * 10 + charToInt(temp);
            infile >> tempInt;
            cout << tempInt;
            //}
        cols = tempInt;
        
        cout << "\n Rows ";
	//infile >> temp;
        tempInt = 0;
        //while ((temp >= '0') && (temp <='9')){
          //  tempInt = tempInt * 10 + charToInt(temp);
            infile >> tempInt;
            cout << tempInt;
            //}
        rows = tempInt;
        length = rows * cols;
	//infile >> temp;
        tempInt = 0;
        //while ((temp >= '0') && (temp <='9')){
          //  tempInt = tempInt * 10 + charToInt(temp);
            infile >> tempInt;
            //cout << temp;
            //}
        greyLevel = tempInt;
        
        /*
            Actual point of reading in data
            
        */
        cout << "\n  Setting Transfer \n";
        jclass cls = env->GetObjectClass(obj);
        jmethodID setValue = env->GetMethodID (cls, "setvalue", "(I)V");
        if (setValue == NULL ) return;
        jmethodID settemp = env->GetMethodID (cls, "settemp", "(D)V");
        if (settemp == NULL ) return;
        jmethodID allocate = env->GetMethodID (cls, "allocate", "(I)V");
        if (allocate == NULL ) return;
        jmethodID setGrey = env->GetMethodID (cls, "setGrey", "(I)V");
        if (setGrey == NULL ) return;
        jmethodID setRow = env->GetMethodID (cls, "setRow", "(I)V");
        if (setRow == NULL ) return;
        jmethodID setCol = env->GetMethodID (cls, "setCol", "(I)V");
        if (setCol == NULL ) return;
        env->CallVoidMethod(obj, allocate, (jint)length);
        env->CallVoidMethod(obj, setGrey, (jint)greyLevel);
        env->CallVoidMethod(obj, setRow, (jint)rows);
        env->CallVoidMethod(obj, setCol, (jint)cols);
        //output.NUMBER_OF_GREY_LEVELS = tempInt;
        //output.m_rows=rows;
        //output.m_cols= cols;
        //output.allocateStorage(rows, cols);
        //cout << "Row ";cout << rows << "Cols " << cols << " Grey Level " << greyLevel << "\n"; 
        //cout << "\n Rows "; 
        //cout << rows ; 
        //cout << " Columns " ; 
        //cout << cols << " \n";
        //cout << " Grey Level ";
        //cout << greyLevel;
        //cout << "\n"; 
        pic = new double[length];
        for (  i = 0; i < rows; i++){ 
        	//cout << endl;
	    for ( j = 0; j < cols; j++) {
                pos = i * cols + j;
                infile >> tempInt;
                value = tempInt/greyLevel;
                //cout << "\n " << pos;
                pos = i * cols + j;
                pic[pos] = value;
                //output[i][j] =value ;
                env->CallVoidMethod (obj, settemp, value);
                env->CallVoidMethod (obj, setValue, pos );
		//cout << value << '\t';
         }
	}
	//infile.read(m_data[0],m_rows*m_cols);
        
	if (infile.bad()){
		cout << "Unable to read entire file!";
                exit (1);
                }
	infile.close();
        //this = tempImage;
        cout << "Read the file";

}
/*
void saveImagePGM2 (const char* filename)
{
	int i;
	FileOutputStream outfile(filename);
	outfile << "P2" << endl;
	outfile << input.m_cols << " " << input.m_rows << endl;
	outfile << 255 << endl; //this is 255
	
	for (int r=0; r< input.m_rows; r++) {
		for (int c=0; c< input.m_cols; c++)
                {
                    i = int (ceil (input[r][c] * 255));
                    if ( i > 255) i = 255;
                    if (i < 0 ) i = 0;
                    outfile << i;
                    outfile << " ";
                }
                outfile << endl;
            }

	outfile.close();
        return 1;
}*/
