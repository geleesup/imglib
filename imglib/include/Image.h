// Name:	Geun Lee
// ID:		2005-8997-10
// Date:	9/17/2010 


class Image
{
    private:
    	int width, height, depth;
    	unsigned char *** ImageArray;			// Pointer --> Pointer --> Pointer --> char
    	int histArray[256];
    	int median(int filterArray[5]);
    
    public:
    	Image( int width,
               int height,
               int depth );	    	
        
        Image(Image const & preImage);				// custom copy constructor
    	~Image(void);								// deconstructor
    	void readImage (char* fileName);			// read data. file --> image array
    	void writeImage (char* fileName);			// write data. image array --> file 
    	unsigned char getPixel(int row, int column, int plane);					// get pixel value at position
    	void setPixel(int row, int column, int plane, unsigned char value);		// set pixel value at position
    	Image getBlock(int row_1, int col_1, int row_2, int col_2);				// get image block defined by corners
    	Image xScalar(float scalar);				// multiply each element in image by scalar
    	Image addImage(Image const & toAddImage);	// Image addition
    	void fillImage(int value);					// Fill Image with value
    	void makeHistogram();						// Generate Histogram for image
    	void writeHistogram(char* fileName);		// Write histogram to text file
    	int * getHistogram();						// Returns histogram array
    	Image padImage(int pW);						// Pad image with reflected pixels, specify pad width (pW)
    	Image filter3(int filterArray[3][3], float filterScalar);	// Applies 3x3 filters to image
    	Image filter5(int filterArray[5][5], float filterScalar);	// Applies 5x5 filters to image
    	Image medianFilter();						// Median filter
    	int getWidth(){return width;}				// Returns width of image
    	int getHeight(){return height;}				// Returns height of image
    	int getDepth(){return depth;}				// Returns depth of image
    	Image dblFilter3(double filterArray[3][3], double filterScalar);	// Double precision 3x3 filters
    	Image dblFilter5(double filterArray[5][5], double filterScalar);	// Double precision 5x5 filters
    	void fillTriangle(int aR, int aC, int bR, int bC, int cR, int cC, int val); // fill triangle
    	void copy(Image toCopy);
};
