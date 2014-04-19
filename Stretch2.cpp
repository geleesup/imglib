#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <fstream>
#include "image.h"
#include <math.h>
#include "Matrix.h"

extern Matrix gradEnergy(Image inputImage);
extern Image mat2im(Matrix toConvert);
extern Matrix minSeamVegy(Matrix Energy, float minEnergy);

using namespace std;

Image stretch(Image inputImage, Matrix maskProtect, int numPixels){
	
	// Grayscale Image
	int width = inputImage.getWidth();
	int height = inputImage.getHeight();
	int depth = inputImage.getDepth();
	
	Image gray(width,height,1);
	
	for(int i=0;i<height;++i){			
		for(int j=0;j<width;++j){
			double R = 0.299*int(inputImage.getPixel(i,j,0));
			double G = 0.587*int(inputImage.getPixel(i,j,1));
			double B = 0.114*int(inputImage.getPixel(i,j,2));
			
			gray.setPixel(i,j,0,(unsigned char)(int(R+G+B+0.5)));		// Round to integer and make to unsigned char
		}
	}

																		
	Image output(width+numPixels,height,depth);					// Declare stretched output image
	Image grayStretch(width+numPixels,height,1);	
	Matrix maskStretch(width+numPixels,height,1);

	// Copy image contents
	for(int i=0;i<height;++i){ for(int j=0;j<width;++j){ for(int k=0;k<depth;++k){				
		output.setPixel(i,j,k,inputImage.getPixel(i,j,k));		
	}}}

	for(int i=0;i<height;++i){ for(int j=0;j<width;++j){
		grayStretch.setPixel(i,j,0,gray.getPixel(i,j,0));
		maskStretch.setElement(i,j,0,maskProtect.getElement(i,j,0));
	}}
	
	// Seam Duplication
	
	float energyCount=0; 

	for(int k=0;k<numPixels;++k){

		Matrix Energy(gradEnergy(grayStretch.getBlock(0,0,height-1,width-1+k)));
		Energy.copy(Energy.add(maskStretch.getBlock(0,0,height-1,width-1+k).xScalar(10000000/255)));		
		Matrix seamVector(minSeamVegy(Energy,energyCount));				// Get seamMatrix
	
		energyCount = 0;

		for(int i=0;i<height;++i){
			int seamJ = (int)seamVector.getElement(i,0,0);
			//cout << " " << seamJ << " ";
			for(int j=width+numPixels-1;j>seamJ;--j){
				//cout << " " << j << " ";
				output.setPixel(i,j,0,output.getPixel(i,j-1,0));
				output.setPixel(i,j,1,output.getPixel(i,j-1,1));
				output.setPixel(i,j,2,output.getPixel(i,j-1,2));
								
				maskStretch.setElement(i,j,0,maskStretch.getElement(i,j-1,0));

				grayStretch.setPixel(i,j,0,grayStretch.getPixel(i,j-1,0));
				
			}
					
			output.setPixel(i,seamJ,0,int(255*(1-(double(k)/double(numPixels)))));
			output.setPixel(i,seamJ,1,20);
			output.setPixel(i,seamJ,2,int(255*((double(k)/double(numPixels)))));
			
			maskStretch.setElement(i,seamJ,0,255);
						
			//energyCount += Energy.getElement(i,seamJ,0);
			//energyCount++;
		}

		energyCount = seamVector.getElement(height,0,0);
		//energyCount += energyCount*(0.5);
		//cout << endl << energyCount << endl;
		//energyCount += Energy.getElement(height-1,int(seamVector.getElement(height-1,0,0)),0)+50;
		cout << "\r\r" << "Stretch: " << k;
	}
	

	
	return output;
}