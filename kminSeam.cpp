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
Matrix sort(Matrix vector);

Matrix KminSeamV(Matrix Energy, double k){
	int width = Energy.getWidth();
	int height = Energy.getHeight();
	
	Matrix M(width+2,height+2,1);		// zero padded of width 1
	M.fill(1000000000);
	Matrix seamVector(1,height,1);
	Matrix seamMatrix((int)k,height,1);
	
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
	
	Matrix sortedEnergy(sort(M.getBlock(height-2,0,height-2,width+1)));

		
	for(int j=0;j<k;++j){

		seamMatrix.setElement(height-1,j,0,sortedEnergy.getElement(0,j,0)-1);
		M.setElement(height,int(sortedEnergy.getElement(0,j,0)),0,100000000);
		// Continue seam index search

		//int col = minColIndex;		// Initlalize current column
		int col = int(sortedEnergy.getElement(0,j,0));

		for (int i=height-2;i>=0;--i){		// second row
			iM = i+1;
				
			col = col+minIndex(M.getElement(iM,col-1,0),M.getElement(iM,col,0),M.getElement(iM,col+1,0));

			//seamVector.setElement(i,0,0,col);
			seamMatrix.setElement(i,j,0,col-1);
			M.setElement(iM,col,0,100000000);
		}
	}

	// Shift seams
	for(int i=20;i<k;++i){
		for(int j=0;j<k;++j){
			if(seamMatrix.getElement(0,j,0) > seamMatrix.getElement(0,i,0)){
				seamMatrix.setElement(0,j,0,seamMatrix.getElement(0,j,0)+1);
			}
		}
	}

	return seamMatrix;
}
Matrix sort(Matrix vector){

	int minIndexVal;
	double minValue;
	double temp;
	int width = vector.getWidth();

	Matrix index(width,1,1);

	for(int j=0;j<width;++j){ index.setElement(0,j,0,j); }

	int tempN;

	for(int i=0;i<width;++i){
		minIndexVal = i;
	
		minValue = vector.getElement(0,i,0);

		for (int j=i+1;j<width;++j){
		
			if (vector.getElement(0,j,0)<minValue){			
				minIndexVal = j;
				
				
				minValue = vector.getElement(0,j,0);
			}
		}
	
		temp = vector.getElement(0,i,0);
		tempN = (int)index.getElement(0,i,0);

		vector.setElement(0,i,0,vector.getElement(0,minIndexVal,0));

		index.setElement(0,i,0,index.getElement(0,minIndexVal,0));

		vector.setElement(0,minIndexVal,0,temp);

		index.setElement(0,minIndexVal,0,tempN);
	}
	
	
	Matrix vectorS(width,1,1);
	Matrix indexS(width,1,1);

	int x = int(vector.getElement(0,0,0));

	int ix = int(index.getElement(0,0,0));
	int iS = 1;

	vectorS.setElement(0,0,0,x);	indexS.setElement(0,0,0,ix);

	
	for(int i=0;i<width;++i){
		if(vector.getElement(0,i,0)>=x){
			x = int(vector.getElement(0,i,0));
			vectorS.setElement(0,iS,0,x);	indexS.setElement(0,iS,0,index.getElement(0,i,0));
			x+=10;
			++iS;
		}
	}

	return indexS;

	//return vector[2];
}