// Name:	Geun Lee
// ID:		2005-8997-10
// Date:	9/17/2010 

#include "Image.h"
#include <fstream>
#include <iostream>
#include <stdio.h>

using namespace std;

// Constructor
Image::Image(int w, int  h, int d)
{
	width = w;
	height = h;
	depth = d;

	for(int i=0;i<256;++i){		// Initialize all elements of histArray
		histArray[i]=0;		
	}
	
	// Dynamic memory allocation
	
	// ImageArray was defined in Image.h as Pointer --> Pointer --> Pointer --> char.
	// The dynamic array is defined as an array of pointers. 

	ImageArray = new unsigned char**[height];	// Defines 2nd dimension.  Pointer --> Pointer --> char 
													
	for(int i = 0;i<height;++i){
		ImageArray[i] = new unsigned char*[width];			// Defines 3rd Dimension. Pointer --> char. These array
															// elements point to the actual location in memory where
		for(int j = 0;j<width;++j){							// the char will be stored.
			ImageArray[i][j] = new unsigned char[depth];	// <-- These are the locations of type unsigned char.
		}
	}

	for (int i=0;i<height;++i){								// Set each memory values in the array to NULL;
		for (int j = 0;j<width;++j){						
			for (int k = 0;k<depth;++k)
				ImageArray[i][j][k] = NULL;    // CR issues.
		}
	}
}

// Copy constructor

// We cannot use the default copy constructor because it will only copy over the address to the dynamic array.
// We need to define a new image array and copy with a element-by-element method.

Image::Image(Image const & preImage)		// preImage is the image array that is to be copied
{
	width = preImage.width; height = preImage.height; depth = preImage.depth;

	ImageArray = new unsigned char**[height];			// Same as default constructor to create image array

	for(int i = 0;i<height;++i){						
		ImageArray[i] = new unsigned char*[width];

		for(int j = 0;j<width;++j){
			ImageArray[i][j] = new unsigned char[depth];
		}
	}

	for (int i=0;i<height;++i){
		for (int j = 0;j<width;++j){
			for (int k = 0;k<depth;++k)
				ImageArray[i][j][k] = preImage.ImageArray[i][j][k];   // Copies each element in pre-image to ImageArray
		}
	}
}

// Deconstructor

// Using the new[] operator, we have to de-allocate memory once we are finished. This works backwards from the constructor
// deleting each sub array.

Image::~Image(void)
{
	for (int i=0;i<height;++i){
		for (int j=0;j<width;++j){
			delete [] ImageArray[i][j];			// deleting 3D pointers
		}
		delete [] ImageArray[i];				// deleting 2D pointers
	}

	delete [] ImageArray;						// delete the final pointer array
}


// Read file to image function

// Method to open up a image file and read contents into our defined array
void Image::readImage (char* fileName){
	ifstream readFile;						

    //File* readFile;

    //readFile = fopen( fileName, "rb" );

	readFile.open(fileName, ios::binary);	// Opens the file with binary option. Binary option is important, otherwise
											// unwanted carriage returns are included.
    if( !readFile ) {
        printf( "Cannot open file: %s\r\n", fileName );
		exit(1);
	}
	
	for (int i=0;i<height;++i){				// Element-by-elment read from file into image array.
		for (int j = 0;j<width;++j){		// We can't do pointer arithmetic to write to memory 
			for (int k = 0;k<depth;++k)		// because memory is non-contiguously allocated.
				ImageArray[i][j][k] = readFile.get();    
		}
	}

	readFile.close();
}


// Write contents of the image array to a file
void Image::writeImage (char* fileName){
	ofstream writeFile;
	
	writeFile.open(fileName, ios::out | ios::ate | ios::binary);	//ios::out set for output
													//ios::ate  moves stream pointer to end of file
	if(!writeFile.is_open()){						//ios::binary binary mode. Important* see readImage.
        printf( "Cannot open file: %s\r\n", fileName );
		exit(1);
	}

	for (int i=0;i<height;++i){
		for (int j = 0;j<width;++j){
			for (int k = 0;k<depth;++k)
				writeFile << ImageArray[i][j][k];	// Element-by-element write
		}
	}
	
	writeFile.close();
	
    printf( "%s    Size: %f KB %d x %d x %d\r\n", fileName,
        (width*height*depth)/float(1024), width, height, depth );
}

// Returns the pixel value given an image coordinate
unsigned char Image::getPixel(int row, int column, int plane){
	
	return ImageArray[row][column][plane];
}

// write a pixel value to image array given an image coordinate and value
void Image::setPixel(int row, int column, int plane, unsigned char value){
	ImageArray[row][column][plane] = value;
}


// Creates a sub-image of current image. Dimension and location of sub-image defined by corner coordinates.
// Returns the sub-image. Useful for getting image blocks, search windows.

Image Image::getBlock(int row_1, int col_1, int row_2, int col_2){		// two coordinates equiv to y1,x1, y2,x2.
	int row_min,  row_max, col_min, col_max;							// which correspond to two opposite corners of
																		// sub image.
	
	// First, find minimum and maximum bounds for the block, then use for loops to output values element-by-element 
	// to a new image array.
	
	if (row_1 <= row_2){		// Compare the y values of the coordinates		
		row_min = row_1;		// Assign the correspond min,max values
		row_max = row_2;
	}
	else if (row_1 > row_2){
		row_min = row_2;
		row_max = row_1;
	}

	if (col_1 <= col_2){		// Compare the x values of the coordinates
		col_min = col_1;
		col_max = col_2;
	}
	else if (col_1 > col_2){
		col_min = col_2;
		col_max = col_1;
	}

	Image imageBlock((col_max-col_min)+1, (row_max-row_min)+1, depth);  // Allocate memory with the sub-image dimensions
																		// +1 because going from image coordinates to
	int to_col = 0;		// Index values of sub-image array				// array coordinates
	int to_row = 0;
	

	for (int i = row_min; i <= row_max; ++i){ 			// Row indexer
		to_col = 0;											// Set sub-image col index to 0 each time a new row starts
		for (int j = col_min; j <= col_max; ++j){ 		// Column indexer
			for (int k = 0;k<depth;++k){					// Depth indexer
				imageBlock.setPixel(to_row,to_col,k,ImageArray[i][j][k]);	// Set values of sub-image array to values from
			}																// the original image.
			to_col++;		// Increment sub-image column index
		}
		to_row++;			// Increment sub-image row index
	}

	return imageBlock;		// returns the sub-image array
}

// Multiplies each element in the image by a scalar
Image Image::xScalar(float scalar){
	float temp=0;
	int toChar=0;
	
	Image xArray(width,height,depth);	

	for (int i=0;i<height;++i){
		for (int j = 0;j<width;++j){
			for (int k = 0;k<depth;++k){
				temp = float(ImageArray[i][j][k]);		// Cast unsigned char to float
				temp *= scalar;							// multiply by scalar and assign
				toChar = int(temp+0.5);					// Rounding
				xArray.setPixel(i,j,k,(unsigned char)(toChar));	// write pixel
			}
		}
	}

	return xArray;
}

Image Image::addImage(Image const & toAddImage){
	int firstValue=0;
	int secondValue=0;
	int sum = 0;

	Image sumArray(width,height,depth);
	
	// Check if images are same size
	if(!(toAddImage.width == width && toAddImage.height == height))
		exit(1);
	else
	{
		for (int i=0;i<height;++i){
				for (int j = 0;j<width;++j){
					for (int k = 0;k<depth;++k){
						firstValue = int(ImageArray[i][j][k]);			// Cast unsigned char to int
						secondValue = int(toAddImage.ImageArray[i][j][k]);
						sum = firstValue+secondValue;					// multiply by scalar and assign
																
						sumArray.setPixel(i,j,k,(unsigned char)(sum));	// write pixel
					}
				}
		}
	}

	return sumArray;
}

void Image::fillImage(int value){
	for (int i=0;i<height;++i){
		for (int j = 0;j<width;++j){
			for (int k = 0;k<depth;++k){
				ImageArray[i][j][k]=(unsigned char)(value);
			}
		}
	}
}

void Image::makeHistogram(){
	for(int i=0;i<256;++i){		// Initialize all elements of histArray
		histArray[i]=0;		
	}

	for (int i=0;i<height;++i){
		for (int j = 0;j<width;++j){
			for (int k = 0;k<depth;++k){
				histArray[int(ImageArray[i][j][k])]++;
			}
		}
	}
}

void Image::writeHistogram(char* fileName){
	ofstream writeFile;
	
	writeFile.open(fileName, ios::out | ios::ate | ios::binary);	//ios::out set for output
													//ios::ate  moves stream pointer to end of file
	if(!writeFile.is_open()){						//ios::binary binary mode. Important* see readImage.
        printf( "Cannot open file: %s\r\n", fileName );
		exit(1);
	}

	for (int i=0;i<256;++i){
		writeFile << i << "," << histArray[i] << "\r\n";	// Element-by-element write , \r\n MS  Text Mode
	}
	
	writeFile.close();
				
    printf( "%s\r\n", fileName );
}

int * Image::getHistogram(){
	return histArray;
}
Image Image::padImage(int pW){
	// Pad Image

	// pW padding width. 3x3 - padding is 1 on all sides 5x5 - padding is 2

	int pImWidth = width+(2*pW);
	int pImHeight = height+(2*pW);

	Image paddedImage(pImWidth, pImHeight, depth); // Padding extra row to both top and bottom

	for(int i=0;i<height;++i){						// Copy original image to center of blank padded image
		for(int j = 0;j<width;++j){
			for(int k = 0;k<depth;++k){
				paddedImage.setPixel(i+pW,j+pW,k,ImageArray[i][j][k]);
			}
		}
	}

	for(int i=0;i<pW;++i){							// Top + Bottom Border Padding
		for(int j=0;j<pImWidth;++j){
			for(int k = 0;k<depth;++k){
				paddedImage.setPixel(i,j,k,paddedImage.getPixel(2*pW-i,j,k));
				int rI = (pImHeight-1)-i;			// "Reverse i" - reversed row Index
				paddedImage.setPixel(rI,j,k,paddedImage.getPixel((height-1)-2*pW+i,j,k));
			}
		}
	}

	for(int i=0;i<pImHeight;++i){							// Right + Left Border Padding
		for(int j=0;j<pW;++j){
			for(int k = 0;k<depth;++k){
				paddedImage.setPixel(i,j,k,paddedImage.getPixel(i,2*pW-j,k));
				int rJ = (pImWidth-1)-j;			// "Reverse j" - reverse column  Index
				paddedImage.setPixel(i,rJ,k,paddedImage.getPixel(i,(width-1)-2*pW+j,k));
			}
		}
	}

	return paddedImage;
}

Image Image::filter3(int filterArray[3][3], float filterScalar){
	int a = 1;	// distance from center pixel		padding width = 1 since 3x3
	
	int f_row = 0;
	int f_col = 0;

	int sum = 0;
	float scaledSum = 0;
	int pixelValue = 0;

	Image filteredImage(width,height,depth);
	
	for(int i=1;i<height-1;++i){						
		for(int j=1;j<width-1;++j){
			for(int k = 0;k<depth;++k){
				
				f_row = 0;
				sum = 0;

				for (int iFilter = i-a; iFilter <= i+a; ++iFilter){
					f_col=0;
		
					for (int jFilter = j-a; jFilter <= j+a; ++jFilter){
						pixelValue = int(ImageArray[iFilter][jFilter][k]);
						sum += filterArray[f_row][f_col]*pixelValue;
						f_col++;
					}
		
					f_row++;
				}
				
				scaledSum = filterScalar*sum;
				filteredImage.setPixel(i,j,k,(unsigned char)(int(scaledSum+0.5)));
			}
		}

        printf( "\r\rLow Pass Filter: %d", i );
	}

    printf( "\r\n" );
	
	return filteredImage.getBlock(1,1,height-2,width-2);
}
Image Image::filter5(int filterArray[5][5], float filterScalar){
	int a = 2;	// distance from center pixel		padding width = 1 since 3x3
	
	int f_row = 0;
	int f_col = 0;

	int sum = 0;
	float scaledSum = 0;
	int pixelValue = 0;

	Image filteredImage(width,height,depth);
	
	for(int i=2;i<height-2;++i){						
		for(int j=2;j<width-2;++j){
			for(int k = 0;k<depth;++k){
				
				f_row = 0;
				sum = 0;

				for (int iFilter = i-a; iFilter <= i+a; ++iFilter){			// iFilter - i index of filter
					f_col=0;												// jFilter - j index of filter
		
					for (int jFilter = j-a; jFilter <= j+a; ++jFilter){
						pixelValue = int(ImageArray[iFilter][jFilter][k]);
						sum += filterArray[f_row][f_col]*pixelValue;
						f_col++;
					}
		
					f_row++;
				}
				
				scaledSum = filterScalar*sum;
				filteredImage.setPixel(i,j,k,(unsigned char)(int(scaledSum+0.5)));
			}
		}

        printf( "\r\rLow Pass Filter: %d", i );
	}

    printf( "\r\n" );
	
	return filteredImage.getBlock(3,3,height-2,width-2);
}
int Image::median(int filterArray[5]){

	int minIndex;
	int minValue;
	int temp;

	for(int i=0;i<5;++i){
		minIndex = i;
		minValue = filterArray[i];

		for (int j=i+1;j<5;++j){
			if (filterArray[j]<minValue){
				minIndex = j;
				minValue = filterArray[j];
			}
		}
		temp = filterArray[i];
		filterArray[i] = filterArray[minIndex];
		filterArray[minIndex] = temp;
	}
	
	return filterArray[2];
}

Image Image::medianFilter(){
	
	int filterArray[5]={0};
	Image filteredImage(width,height,depth);

	for(int i=1;i<height-1;++i){						
		for(int j=1;j<width-1;++j){
			for(int k = 0;k<depth;++k){
				
				filterArray[0] =	int(ImageArray[i][j][k]);		// Wanted to minimize for loops
				filterArray[1] =	int(ImageArray[i][j-1][k]);		// Pixels for 3x3 Cross
				filterArray[2] =	int(ImageArray[i-1][j][k]);		
				filterArray[3] =	int(ImageArray[i+1][j][k]);
				filterArray[4] =	int(ImageArray[i][j+1][k]);

				filteredImage.setPixel(i,j,k,(unsigned char)(median(filterArray)));
			}
		}
	
        printf( "\r\rMedian Filter: %d", i );

	}
	
    printf( "\r\n" );

	return filteredImage.getBlock(1,1,height-2,width-2);
}
Image Image::dblFilter3(double filterArray[3][3], double filterScalar){
	int a = 1;	// distance from center pixel		padding width = 1 since 3x3
	
	int f_row = 0;
	int f_col = 0;

	double sum = 0;
	double scaledSum = 0;
	int pixelValue = 0;

	Image filteredImage(width,height,depth);
	
	for(int i=1;i<height-1;++i){						
		for(int j=1;j<width-1;++j){
			for(int k = 0;k<depth;++k){
				
				f_row = 0;
				sum = 0;

				for (int iFilter = i-a; iFilter <= i+a; ++iFilter){
					f_col=0;
		
					for (int jFilter = j-a; jFilter <= j+a; ++jFilter){
						pixelValue = int(ImageArray[iFilter][jFilter][k]);
						sum += filterArray[f_row][f_col]*pixelValue;
						f_col++;
					}
		
					f_row++;
				}
				
				scaledSum = filterScalar*sum;
				filteredImage.setPixel(i,j,k,(unsigned char)(int(scaledSum+0.5)));
			}
		}

        printf( "\r\rLow Pass Filter: %d", i );
	}

    printf( "\r\n" );
	
	return filteredImage.getBlock(1,1,height-2,width-2);
}
Image Image::dblFilter5(double filterArray[5][5], double filterScalar){
	int a = 2;	// distance from center pixel		padding width = 2 since 5x5
	
	int f_row = 0;
	int f_col = 0;

	double sum = 0;
	double scaledSum = 0;
	int pixelValue = 0;

	Image filteredImage(width,height,depth);
	
	for(int i=2;i<height-2;++i){						
		for(int j=2;j<width-2;++j){
			for(int k = 0;k<depth;++k){
				
				f_row = 0;
				sum = 0;

				for (int iFilter = i-a; iFilter <= i+a; ++iFilter){			// iFilter - i index of filter
					f_col=0;												// jFilter - j index of filter
		
					for (int jFilter = j-a; jFilter <= j+a; ++jFilter){
						pixelValue = int(ImageArray[iFilter][jFilter][k]);
						sum += filterArray[f_row][f_col]*pixelValue;
						f_col++;
					}
		
					f_row++;
				}
				
				scaledSum = filterScalar*sum;
				filteredImage.setPixel(i,j,k,(unsigned char)(int(scaledSum+0.5)));
			}
		}
        
        printf( "\r\rLow Pass Filter: %d", i );
	}

    printf( "\r\n" );
	
	return filteredImage.getBlock(3,3,height-2,width-2);
}
void Image::fillTriangle(int aR, int aC, int bR, int bC, int cR, int cC, int val){
	
	int centerR = int(((aR+bR+cR)/3.0)+0.5);
	int centerC = int(((aC+bC+cC)/3.0)+0.5);
	
	
	if(int(ImageArray[centerR][centerC][0]) != val){
		ImageArray[centerR][centerC][0] = (unsigned char)(val);
		ImageArray[centerR][centerC][1] = (unsigned char)(val);
		ImageArray[centerR][centerC][2] = (unsigned char)(val);

		fillTriangle(centerR, centerC, bR, bC, cR, cC, val);
		fillTriangle(aR, aC, centerR, centerC, cR, cC, val);
		fillTriangle(aR, aC, bR, bC, centerR, centerC, val);
	}
}
void Image::copy(Image toCopy){
// Full Matrix copy
	int width2 = toCopy.getWidth();
	int height2 = toCopy.getHeight();

	int widthCopy, heightCopy;

	widthCopy = width;
	heightCopy = height;

	if(width2 < width)
		widthCopy = width2;
	if(height2 < height)
		widthCopy = height2;

	for(int i=0;i<heightCopy;++i){
		for(int j=0;j<widthCopy;++j){
			for(int k=0;k<depth;++k){
				ImageArray[i][j][k]=toCopy.getPixel(i,j,k);
			}
		}
			
	}
}
