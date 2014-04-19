
class Matrix
{

private:
	int width, height, depth;
	double *** matrixArray;


public:
	Matrix(int w, int h, int d);
	Matrix(Matrix const & preMatrix);
	~Matrix(void);
	double getElement(int i, int j, int k);
	void setElement(int i, int j, int k, double value);
	int getWidth(){return width;}
	int getHeight(){return height;}
	int getDepth(){return depth;}
	Matrix xScalar(double scalar);
	Matrix add(Matrix const & toAddMatrix);
	Matrix getBlock(int row_1, int col_1, int row_2, int col_2);
	Matrix convolve3(double filterArray[3][3], double filterScalar);
	Matrix convolve5(double filterArray[5][5], double filterScalar);
	Matrix convolve(int windowSize, double ** filterArray, double filterScalar);
	Matrix padMatrix(int pW);
	double ** toArray2D();
	void fill(double value);
	void copy(Matrix toCopy);


};
