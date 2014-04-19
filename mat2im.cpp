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
/*	Converts Image object to matrix object		                     */
/*  toConvert:	Image object to be converted						 */
/*********************************************************************/

double linScale(double min, double max, double x);

Image mat2im(Matrix toConvert){
	
	int width = toConvert.getWidth();
	int height = toConvert.getHeight();
	int depth = toConvert.getDepth();

	Image output(width,height,depth);

	double max = 0;
	double min = 1000000000000000;
	
	for(int i=0;i<height;++i){
		for(int j=0;j<width;++j){
			for(int k=0;k<depth;++k){
				double val = toConvert.getElement(i,j,k);
				if(val > max)
					max = val;
				if(val < min)
					min = val;
			}
		}
	}
	
	for(int i=0;i<height;++i){
		for(int j=0;j<width;++j){
			for(int k=0;k<depth;++k){
				unsigned char pixel = (unsigned char)(linScale(min,max,toConvert.getElement(i,j,k)));
				output.setPixel(i,j,k,pixel);
			}
		}
	}
	
	return output;
}
double linScale(double min, double max, double x){
	double out=0;

	out = (255/(max-min))*(x-min);

	return out;
}