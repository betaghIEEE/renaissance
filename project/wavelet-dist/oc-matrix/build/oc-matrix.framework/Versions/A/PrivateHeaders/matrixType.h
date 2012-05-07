//
//  matrixType.h
//  oc-matrix
//
//  Created by Daniel D. Beatty on 4/1/05.
//  Copyright 2005 __MyCompanyName__. All rights reserved.
//

#import <Cocoa/Cocoa.h>


@interface matrixType : NSObject {
	int m_rows;
    int m_cols;
    double epsilon;
	double max;
	double min;
    int NUMBER_OF_GREY_LEVELS;
	double **m_data;
	
	
    double scaleX;
    double scaleY;
    double shiftX;
    double shiftY;
	
}
-(id) initWithRows: (int) rows columns:(int) cols;
-(id) initWithMatrixType:(matrixType *) rhs;
-(void) allocateStorage;
-(void) allocateRows: (int) rows columns:(int)cols;
-(void) copyElements: (matrixType *) source;
-(void) deleteStorage;
-(void) zeroize;
-(void) defineEpsilon;
-(double) shiftX;
-(double) shiftY;
-(double) scaleX;
-(double) scaleY;
-(int) m_cols;
-(int) m_rows; 


-(double**) matrix;
-(BOOL) checkRows: (int) rows columns:(int)cols;
-(void) addMatrixEqual: (matrixType *) rhs;
-(void) subMatrixEqual: (matrixType *) rhs;
-(void) multMatrixEqual: (matrixType *) rhs;
-(void) sparseMatrixMultiplyEqual: (matrixType *) rhs;
-(void) multEqual: (double) rhs;
-(void) divEqual: (double) rhs;
-(void) addEqual: (double) rhs;
-(void) subEqual: (double) rhs;
-(double) getEnergy ;
-(double) getL1Energy ;
-(double) getMax;
-(double) getMin;
-(double) getMaxWithIn: (int) Urows lRows:(int) lRows lCols:(int) lCols rCols:(int) rCols;
-(double) getMinWithIn: (int) Urows lRows:(int) lRows lCols:(int) lCols rCols:(int) rCols;

-(double) getEnergyWithIn: (int) Urows lRows:(int) lRows lCols:(int) lCols rCols:(int) rCols;
-(double) getL1EnergyWithIn: (int) Urows lRows:(int) lRows lCols:(int) lCols rCols:(int) rCols;

//-(void) generateHistogram;
@end
