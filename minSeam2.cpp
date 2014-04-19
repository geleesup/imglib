// Name:	Geun Lee
// ID:		2005-8997-10
// Date:	12/3/2010 

#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <fstream>
#include "image.h"
#include <math.h>
#include "Matrix.h"

using namespace std;
extern double min3(double a, double b, double c);
extern int minIndex(double a, double b, double c);
extern Image mat2im(Matrix toConvert);

Matrix minSeamVegy(Matrix Energy, float minEnergy){
	int width = Energy.getWidth();
	int height = Energy.getHeight();
	
	Matrix M(width+2,height+2,1);		// zero padded of width 1
	M.fill(10000000000000);
	Matrix seamVector(1,height+1,1);
	
	// Create M - cumulant minimum energy

	// Initalize first row
	for(int j=0;j<width;++j){		
		M.setElement(1,j+1,0,Energy.getElement(0,j,0));
	}
	
	// Calculate cumulant minumum energy
	
	int iM,jM;		// M Indexers

	for(int i=1;i<height;++i){		// start on second row
		for(int j=0;j<width;++j){

			iM = i+1;	jM = j+1;

			double e = Energy.getElement(i,j,0);
			double min = min3(M.getElement(iM-1,jM-1,0),M.getElement(iM-1,jM,0),M.getElement(iM-1,jM+1,0));
			M.setElement(iM,jM,0,e+min);
		}
	}

	// Find seam indices
	

	// Find first min
	double minVal= 10000000000000;
	int minColIndex;

	for(int j=1;j<width-1;++j){		
		double x = M.getElement(height-2,j+1,0);
		//cout << " " << x << " ";

		if(x < minVal && x >= minEnergy){
			minVal = x;
			
			minColIndex = j;
		}
	}
	//cout << endl << "minVal: " << minVal << "  minEnergy: " << minEnergy << endl;

	seamVector.setElement(height-1,0,0,minColIndex);
	//M.setElement(height-2,minColIndex,0,10000000000);

	// Continue seam index search

	int col = minColIndex;		// Initlalize current column

	for (int i=height-2;i>0;--i){		// second row
		iM = i+1;
				
		col = col+minIndex(M.getElement(iM-1,col-1,0),M.getElement(iM-1,col,0),M.getElement(iM-1,col+1,0));

		seamVector.setElement(i,0,0,col);
		//M.setElement(iM-1,col,0,10000000000);


	}

	seamVector.setElement(height,0,0,minVal);
	//Image MImage(mat2im(M));
	//MImage.writeImage("Mseamed.raw");
	return seamVector;
}