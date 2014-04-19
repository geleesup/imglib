#include "Matrix.h"
#include <iostream>

using namespace std;

Matrix::Matrix(int w, int h, int d)
{
	width = w;
	height = h;
	depth = d;

	
	// Dynamic memory allocation
	
	// ImageArray was defined in Image.h as Pointer --> Pointer --> Pointer --> char.
	// The dynamic array is defined as an array of pointers. 

	matrixArray = new double**[height];	// Defines 2nd dimension.  Pointer --> Pointer --> char 
													
	for(int i = 0;i<height;++i){
		matrixArray[i] = new double*[width];			// Defines 3rd Dimension. Pointer --> char. These array
															// elements point to the actual location in memory where
		for(int j = 0;j<width;++j){							// the char will be stored.
			matrixArray[i][j] = new double[depth];	// <-- These are the locations of type unsigned char.
		}
	}

	for (int i=0;i<height;++i){								// Set each value in the array to 0;
		for (int j = 0;j<width;++j){						
			for (int k = 0;k<depth;++k)
				matrixArray[i][j][k] = 0;    // CR issues.
		}
	}
}
Matrix::Matrix(Matrix const & preMatrix)		// preImage is the image array that is to be copied
{
	width = preMatrix.width; height = preMatrix.height; depth = preMatrix.depth;

	matrixArray = new double**[height];			// Same as default constructor to create image array

	for(int i = 0;i<height;++i){						
		matrixArray[i] = new double*[width];

		for(int j = 0;j<width;++j){
			matrixArray[i][j] = new double[depth];
		}
	}

	for (int i=0;i<height;++i){
		for (int j = 0;j<width;++j){
			for (int k = 0;k<depth;++k)
				matrixArray[i][j][k] = preMatrix.matrixArray[i][j][k];   // Copies each element in pre-image to ImageArray
		}
	}
}

Matrix::~Matrix(void)
{
	for (int i=0;i<height;++i){
		for (int j=0;j<width;++j){
			delete [] matrixArray[i][j];			// deleting 3D pointers
		}
		delete [] matrixArray[i];				// deleting 2D pointers
	}

	delete [] matrixArray;						// delete the final pointer array
}

double Matrix::getElement(int i, int j, int k){
	return matrixArray[i][j][k];
}
void Matrix::setElement(int i, int j, int k, double value){
	matrixArray[i][j][k]=value;
}
Matrix Matrix::xScalar(double scalar){
	double temp=0;
	double value=0;
	
	Matrix xArray(width,height,depth);	

	for (int i=0;i<height;++i){
		for (int j = 0;j<width;++j){
			for (int k = 0;k<depth;++k){
				temp = matrixArray[i][j][k]*scalar;
				xArray.setElement(i,j,k,temp);	// write pixel
			}
		}
	}

	return xArray;
}

Matrix Matrix::add(Matrix const & toAddMatrix){
	double firstValue=0;
	double secondValue=0;
	double sum = 0;

	Matrix sumArray(width,height,depth);
	
	// Check if images are same size
	if(!(toAddMatrix.width == width && toAddMatrix.height == height))
		exit(1);
	else
	{
		for (int i=0;i<height;++i){
				for (int j = 0;j<width;++j){
					for (int k = 0;k<depth;++k){
						firstValue = matrixArray[i][j][k];			
						secondValue = toAddMatrix.matrixArray[i][j][k];
						sum = firstValue+secondValue;					// multiply by scalar and assign
																
						sumArray.setElement(i,j,k,sum);	// write pixel
					}
				}
		}
	}

	return sumArray;
}
Matrix Matrix::getBlock(int row_1, int col_1, int row_2, int col_2){		// two coordinates equiv to y1,x1, y2,x2.
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

	Matrix matrixBlock((col_max-col_min)+1, (row_max-row_min)+1, depth);  // Allocate memory with the sub-matrix dimensions
																		// +1 because going from image coordinates to
	int to_col = 0;		// Index values of sub-matrix array				// array coordinates
	int to_row = 0;
	

	for (int i = (row_min); i <= row_max; ++i){ 			// Row indexer
		to_col = 0;											// Set sub-matrix col index to 0 each time a new row starts
		for (int j = (col_min); j <= col_max; ++j){ 		// Column indexer
			for (int k = 0;k<depth;++k){					// Depth indexer
				matrixBlock.setElement(to_row,to_col,k,matrixArray[i][j][k]);	// Set values of sub-matrix array to values from
			}																// the original image.
			to_col++;		// Increment sub-image column index
		}
		to_row++;			// Increment sub-image row index
	}

	return matrixBlock;		// returns the sub-image array
}

Matrix Matrix::convolve3(double filterArray[3][3], double filterScalar){
	int a = 1;	// distance from center pixel		padding width = 1 since 3x3
	
	int f_row = 0;
	int f_col = 0;

	int sum = 0;
	float scaledSum = 0;
	int pixelValue = 0;

	Matrix convolvedMatrix(width,height,depth);
	
	for(int i=1;i<height-1;++i){						
		for(int j=1;j<width-1;++j){
			for(int k = 0;k<depth;++k){
				
				f_row = 0;
				sum = 0;

				for (int iFilter = i-a; iFilter <= i+a; ++iFilter){
					f_col=0;
		
					for (int jFilter = j-a; jFilter <= j+a; ++jFilter){
						pixelValue = int(matrixArray[iFilter][jFilter][k]);
						sum += filterArray[f_row][f_col]*pixelValue;
						f_col++;
					}
		
					f_row++;
				}
				
				scaledSum = filterScalar*sum;
				convolvedMatrix.setElement(i,j,k,scaledSum);
			}
		}
		//cout << "\r\r" << "Convolving: " << i; 
	}

	//cout << endl;
	
	return convolvedMatrix.getBlock(1,1,height-2,width-2);
}
Matrix Matrix::convolve5(double filterArray[5][5], double filterScalar){
	int a = 2;	// distance from center pixel		assume windowSize is odd
								
	int f_row = 0;
	int f_col = 0;

	int sum = 0;
	float scaledSum = 0;
	int pixelValue = 0;

	Matrix convolvedMatrix(width,height,depth);
	
	for(int i=a;i<height-a;++i){						
		for(int j=a;j<width-a;++j){
			for(int k = 0;k<depth;++k){
				
				f_row = 0;
				sum = 0;

				for (int iFilter = i-a; iFilter <= i+a; ++iFilter){
					f_col=0;
		
					for (int jFilter = j-a; jFilter <= j+a; ++jFilter){
						pixelValue = int(matrixArray[iFilter][jFilter][k]);
						sum += filterArray[f_row][f_col]*pixelValue;
						f_col++;
					}
		
					f_row++;
				}
				
				scaledSum = filterScalar*sum;
				convolvedMatrix.setElement(i,j,k,scaledSum);
			}
		}
		//cout << "\r\r" << "Convolving: " << i; 
	}

	//cout << endl;
	
	return convolvedMatrix.getBlock(a,a,height-a-1,width-a-1);
}
Matrix Matrix::convolve(int windowSize, double ** filterArray, double filterScalar){
	int a = (windowSize-1)/2;	// distance from center pixel		assume windowSize is odd
								
	int f_row = 0;
	int f_col = 0;

	int sum = 0;
	float scaledSum = 0;
	int pixelValue = 0;

	Matrix convolvedMatrix(width,height,depth);
	
	for(int i=a;i<height-a;++i){						
		for(int j=a;j<width-a;++j){
			for(int k = 0;k<depth;++k){
				
				f_row = 0;
				sum = 0;

				for (int iFilter = i-a; iFilter <= i+a; ++iFilter){
					f_col=0;
		
					for (int jFilter = j-a; jFilter <= j+a; ++jFilter){
						pixelValue = int(matrixArray[iFilter][jFilter][k]);
						sum += filterArray[f_row][f_col]*pixelValue;
						f_col++;
					}
		
					f_row++;
				}
				
				scaledSum = filterScalar*sum;
				convolvedMatrix.setElement(i,j,k,scaledSum);
			}
		}
		//cout << "\r\r" << "Convolving: " << i; 
	}

	//cout << endl;
	
	return convolvedMatrix.getBlock(a,a,height-a-1,width-a-1);
}
Matrix Matrix::padMatrix(int pW){
	// Pad Image

	// pW padding width. 3x3 - padding is 1 on all sides 5x5 - padding is 2

	int pImWidth = width+(2*pW);
	int pImHeight = height+(2*pW);

	Matrix paddedImage(pImWidth, pImHeight, depth); // Padding extra row to both top and bottom

	for(int i=0;i<height;++i){						// Copy original image to center of blank padded image
		for(int j = 0;j<width;++j){
			for(int k = 0;k<depth;++k){
				paddedImage.setElement(i+pW,j+pW,k,matrixArray[i][j][k]);
			}
		}
	}

	for(int i=0;i<pW;++i){							// Top + Bottom Border Padding
		for(int j=0;j<pImWidth;++j){
			for(int k = 0;k<depth;++k){
				paddedImage.setElement(i,j,k,paddedImage.getElement(2*pW-i,j,k));
				int rI = (pImHeight-1)-i;			// "Reverse i" - reversed row Index
				paddedImage.setElement(rI,j,k,paddedImage.getElement((height-1)-2*pW+i,j,k));
			}
		}
	}

	for(int i=0;i<pImHeight;++i){							// Right + Left Border Padding
		for(int j=0;j<pW;++j){
			for(int k = 0;k<depth;++k){
				paddedImage.setElement(i,j,k,paddedImage.getElement(i,2*pW-j,k));
				int rJ = (pImWidth-1)-j;			// "Reverse j" - reverse column  Index
				paddedImage.setElement(i,rJ,k,paddedImage.getElement(i,(width-1)-2*pW+j,k));
			}
		}
	}

	return paddedImage;
}
double ** Matrix::toArray2D(){
	double ** newArray;
	newArray = new double*[height];

	for(int i=0;i<height;++i){
		newArray[i] = new double[width];
	}

	for(int i=0;i<height;++i){
		for(int j=0;j<width;++j){
			newArray[i][j] = matrixArray[i][j][0];
		}
	}

	return newArray;
}
void Matrix::fill(double value){
	for (int i=0;i<height;++i){
		for (int j = 0;j<width;++j){
			for (int k = 0;k<depth;++k){
				matrixArray[i][j][k]=value;
			}
		}
	}
}
void Matrix::copy(Matrix toCopy){
// Full Matrix copy

	for(int i=0;i<height;++i){
		for(int j=0;j<width;++j){
			for(int k=0;k<depth;++k){
				matrixArray[i][j][k]=toCopy.getElement(i,j,k);
			}
		}
			
	}
}