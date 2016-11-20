#ifndef _includeheaders_
#define _includeheaders_
#include "includeheaders.h"
#endif

class IFS2D
{
    private:
        IFSIMG img;
        int *len;
        int rows;
        int columns;
        float **ptr; 
    protected:

    public:
        IFS2D(char[]);
        IFS2D(IFSIMG);
        IFS2D(const IFS2D&);
        IFS2D(char[],int , int);

        int* getSize(void);
        int getRows(void);
        int getColumns(void);
        float** getDataPtr(void); 
        float getPixel(int,int);
        void putPixel(int,int,float);

        float getMax(void);
        float getMin(void);
        float getMean(void);
        float getMedian(void);
        float getMode(void);
        float getVariance(void);
        void getHistogram(void);
        void getHistogramRow(int);
        void getHistogramColumn(int);

        void add(IFS2D,IFS2D);
        void subtract(IFS2D,IFS2D);
        void multiply(IFS2D,IFS2D);
        void divide(IFS2D,IFS2D,float);
        void add(IFS2D);
        void subtract(IFS2D);
        void multiply(IFS2D);
        void divide(IFS2D,float);
        void add(float);
        void subtract(float);
        void multiply(float);
        void divide(float);
        void powerOf(float);
        void square(void);
        void exponent(void);

        void copyImg(IFS2D);
        void copyImg(IFS2D*);
        void threshold(float);
        void scale(float,float);
        void applyKernel(float[]);
        void applyKernelXY(float[]);
        void Gauss(float);
        void DOG(float,int,int);
        void DOGx(float);
        void DOGy(float);
        void DOGx2(float);
        void DOGy2(float);
        void DOGxy(float);
        void DOGxDOGy(float);
        void Laplacian(float);
        void QuadraticVariation();
        void Curvature(void);

        void GradientMagnitude(float);
        void GradientDirection(float);
        void GradientDirectionNormalized(float);

        void SavetoText(char[]);
        int WritetoDisk(char[]);
        void SubSample(void);
        void getNeighborhood(int,int,float*);
        void DrawCircle(int,int,int);
        void DrawCircle(int,int,int,float);
        void DistanceTransform(void);
        void InvertDTInside(void);
		void Normalize(void);

};
