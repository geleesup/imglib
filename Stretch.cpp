#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <fstream>
#include "image.h"
#include <math.h>
#include "Matrix.h"

extern Matrix gradEnergy(Image inputImage);
extern Matrix gradEnergyH(Image inputImage);
extern Image mat2im(Matrix toConvert);
extern Matrix KminSeamV(Matrix Energy, double minEnergy);
extern Matrix KminSeamH(Matrix Energy, double minEnergy);

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
            
            gray.setPixel(i,j,0,(unsigned char)(int(R+G+B+0.5)));       // Round to integer and make to unsigned char
        }
    }

    //double currEnergy=0;
                                                                        
    Image output(width+numPixels,height,depth);     // Declare stretched output image
    Matrix maskStretch(width+numPixels,height,depth);

    // Copy image contents
    for( int i=0; i < height; ++i ) {
    for( int j=0; j < width; ++j ) {
    for( int k=0; k < depth; ++k ) {
        output.setPixel(i,j,k,inputImage.getPixel(i,j,k));
        //maskStretch.setElement(i,j,k,maskProtect.getElement(i,j,k));
    }}}

    Matrix Energy(gradEnergy(gray));            // Calculate gradient energy
    Energy.copy(Energy.add(maskProtect.xScalar(100000000/255)));        
    Matrix seamMatrix(KminSeamV(Energy,numPixels));             // Get seamMatrix
    
    // Seam Duplication

    for(int k=0;k<numPixels;++k){

        //Matrix seam2(1,height,1);

        for(int i=0;i<height;++i){
            int seamJ = (int)seamMatrix.getElement(i,k,0);

            for(int j=width+numPixels-1;j>seamJ;--j){
                
                output.setPixel(i,j,0,output.getPixel(i,j-1,0));
                output.setPixel(i,j,1,output.getPixel(i,j-1,1));
                output.setPixel(i,j,2,output.getPixel(i,j-1,2));
                                
                //maskStretch.setElement(i,j,0,maskStretch.getElement(i,j-1,0));

            }
                            
            output.setPixel(i,seamJ,0,255-100*k);
            output.setPixel(i,seamJ,1,0);
            output.setPixel(i,seamJ,2,100*k);
        }
    }
    

    
    return output;
}

Image stretchV(Image inputImage, Matrix maskProtect, int numPixels){
    
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
            
            gray.setPixel(i,j,0,(unsigned char)(int(R+G+B+0.5)));       // Round to integer and make to unsigned char
        }
    }

    //double currEnergy=0;
                                                                        
    Image output(width,height+numPixels,depth);     // Declare stretched output image
    Matrix maskStretch(width,height+numPixels,depth);

    // Copy image contents
    for( int i=0; i < height; ++i ) {
    for( int j=0; j < width; ++j ) {
    for( int k=0; k < depth; ++k ) {
        output.setPixel(i,j,k,inputImage.getPixel(i,j,k));
        //maskStretch.setElement(i,j,k,maskProtect.getElement(i,j,k));
    }}}

    Matrix Energy(gradEnergyH(gray));            // Calculate gradient energy
    Energy.copy(Energy.add(maskProtect.xScalar(100000000/255)));        
    Matrix seamMatrix(KminSeamH(Energy,numPixels));             // Get seamMatrix
    
    // Seam Duplication

    //for(int k=0;k<numPixels;++k){

    //    //Matrix seam2(1,height,1);

    //    for(int j=0;j<width;++j) {
    //        int seamI = (int)seamMatrix.getElement(k,j,0);

    //        for(int i=height+numPixels-1;j>seamI;--i){
    //            
    //            output.setPixel(i,j,0,output.getPixel(i,j-1,0));
    //            output.setPixel(i,j,1,output.getPixel(i,j-1,1));
    //            output.setPixel(i,j,2,output.getPixel(i,j-1,2));
    //                            
    //            //maskStretch.setElement(i,j,0,maskStretch.getElement(i,j-1,0));

    //        }
    //                        
    //        output.setPixel(j,seamI,0,255-100*k);
    //        output.setPixel(j,seamI,1,0);
    //        output.setPixel(j,seamI,2,100*k);
    //    }
    //}
    
    return output;
}
