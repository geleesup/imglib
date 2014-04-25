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

/*********************************************************************/
/*	Takes simple gradient					                         */
/*  inputImage:	Input Image											 */
/*********************************************************************/
Matrix gradEnergy(Image inputImage){

	int width = inputImage.getWidth();
	int height = inputImage.getHeight();
	int depth = inputImage.getDepth();
	

	Image paddedInput(inputImage.padImage(1));
	Matrix gradMatrix(width,height,depth);
	Image outputImage(width,height,depth);

	int r_up, c_right, pixel;
	int Gr, Gc;
	double GMag;

	for(int j=1;j<height+1;++j){
		for(int k=1;k<width+1;++k){
			pixel = int(paddedInput.getPixel(j,k,0));
			r_up = int(paddedInput.getPixel(j-1,k,0));			// "row-up"
			c_right = int(paddedInput.getPixel(j,k+1,0));		// "column-right"

			Gr = abs(pixel-r_up);								// Gradient row
			Gc = abs(pixel-c_right);							// Gradient columns

			GMag = Gr+Gc;
			//GMag = sqrt(double(Gr*Gr+Gc*Gc));
			
			gradMatrix.setElement(j-1,k-1,0,GMag);
		}
	}
	
	return gradMatrix;
}

Matrix gradEnergyH(Image inputImage){

	int width = inputImage.getWidth();
	int height = inputImage.getHeight();
	int depth = inputImage.getDepth();
	

	Image paddedInput(inputImage.padImage(1));
	Matrix gradMatrix(width,height,depth);
	Image outputImage(width,height,depth);

	int r_up, c_left, pixel;
	int Gr, Gc;
	double GMag;

	for(int k=1;k<width+1;++k){
	    for(int j=1;j<height+1;++j){
			pixel = int(paddedInput.getPixel(j,k,0));
			r_up = int(paddedInput.getPixel(j-1,k,0));			// "row-up"
			c_left = int(paddedInput.getPixel(j,k-1,0));		// "column-left"

			Gr = abs(pixel-r_up);								// Gradient row
			Gc = abs(pixel-c_left);							    // Gradient columns

			GMag = Gr+Gc;
			//GMag = sqrt(double(Gr*Gr+Gc*Gc));
			
			gradMatrix.setElement(j-1,k-1,0,GMag);
		}
	}
	
	return gradMatrix;
}


