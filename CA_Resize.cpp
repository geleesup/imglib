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

extern Matrix gradEnergy(Image inputImage);
extern Image mat2im(Matrix toConvert);
extern Matrix minSeamV(Matrix Energy);
extern Image shrink(Image inputImage, Matrix mask, int numPixels);
extern Image stretch(Image inputImage, Matrix mask, int numPixels);
extern Image stretchV(Image inputImage, Matrix mask, int numPixels);


int main(int argc, char* argv[]){

	int width, height, depth, val;
	width = 256; height = 256; depth = 1;

	// Check for proper syntax

	if (argc < 2){
        printf( "Syntax Error - Incorrect Parameter usage:\r\n\r\n" );
        printf( "%s input.raw mask.raw <width> <height> <depth> <shrink/stretch>"
                "\r\n\r\n", argv[0] );
        printf( "Default: <width = 256> <height = 256> <depth = 1>\r\n");

		return 0;
	}

	// Process the arguments

	if (argc > 6){
		val = atoi(argv[6]);				// Make sure image width and height are included
		depth = atoi(argv[5]);
		height = atoi(argv[4]);         
		width = atoi(argv[3]);
	}
	
	bool shrinkIm = false;		// stretch if true
	if (val < 0){shrinkIm = true;}
	

	// Begin processing code

	Image inputImage(width, height, depth);
	Image maskInput(width,height,depth);
	Matrix maskProtect(width,height,1);
	Image outputImage(width,height,depth);			// For padded image

	inputImage.readImage(argv[1]);
	
	// Process Mask
	
	if(atoi(argv[2]) != 1){
		maskInput.readImage(argv[2]);
	}
		
	for(int i=0;i<height;++i){
		for(int j=0;j<width;++j){
			maskProtect.setElement(i,j,0,double(maskInput.getPixel(i, j, 0)));
		}
	}
	
	Image output(inputImage);

	if(shrinkIm){
		val *= -1;
		output.copy(shrink(output,maskProtect,val));
		Image final(output.getBlock(0,0,height-1,width-1-val));
		cout << endl;
		final.writeImage("resized.raw");
	}
	else{
		Image stretchIm(stretch(inputImage,maskProtect,val));
		cout << endl;
		stretchIm.writeImage("resized.raw");
	}
	

	return 0;	
}
