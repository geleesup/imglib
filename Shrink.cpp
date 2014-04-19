/*
 *  Shrink-Sketch.cpp
 *  dao
 *
 *  Created by Geun Lee on 11/16/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <fstream>
#include "image.h"
#include <math.h>
#include "Matrix.h"

extern Matrix gradEnergy(Image inputImage);
extern Image mat2im(Matrix toConvert);
extern Matrix minSeamV(Matrix Energy);

using namespace std;

Image shrink(Image inputImage, Matrix maskProtect, int numPixels){
	
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

	for(int k=0;k<numPixels;++k){
		cout << "\r\r" << "Shrink: " << numPixels-k << "    " << flush;
		
		Matrix Energy(gradEnergy(gray.getBlock(0, 0, height-1, width-1-k)));			// Calculate gradient energy
		
		Energy.copy(Energy.add(maskProtect.getBlock(0, 0, height-1, width-1-k)));					// Add protect mask

		Matrix seam(minSeamV(Energy));				// Get seam vector

		// Seam removal
	
		for(int i=0;i<height;++i){
			int j = seam.getElement(i,0,0);
		
			while(j<width-1){
				inputImage.setPixel(i,j,0,inputImage.getPixel(i,j+1,0));
				inputImage.setPixel(i,j,1,inputImage.getPixel(i,j+1,1));
				inputImage.setPixel(i,j,2,inputImage.getPixel(i,j+1,2));
			
				gray.setPixel(i,j,0,gray.getPixel(i,j+1,0));
				gray.setPixel(i,j,1,gray.getPixel(i,j+1,1));
				gray.setPixel(i,j,2,gray.getPixel(i,j+1,2));
				
				maskProtect.setElement(i,j,0,maskProtect.getElement(i,j+1,0));
				//maskProtect.setElement(i,j,1,maskProtect.getElement(i,j+1,1));
				//maskProtect.setElement(i,j,2,maskProtect.getElement(i,j+1,2));
				
			
				++j;
			}

		}
	}

	
	return inputImage.getBlock(0,0,height-1,width-1);
}