#ifndef _includeheaders_
#define _includeheaders_
#include "includeheaders.h"
#endif

#ifndef _IFS2D_
#define _IFS2D_
#include "IFS2D.h"
#endif

class IFS3D{
    private:
        IFSIMG img;
        int *len;
        int rows;
        int columns;
        int frames;
        float ***ptr; 
    protected:

    public:
        IFS3D(char[]);
        IFS3D(IFSIMG);
        IFS3D(IFS3D*);
        IFS3D(char[],int , int, int);

        int* getSize(void);
        int getRows(void);
        int getColumns(void);
        int getFrames(void);
        float** getDataPtr(void); 
        inline float getPixel(int,int,int);
        inline float putPixel(int,int,int,float);
        IFSIMG getFrame(int);
        void putFrame(IFS2D,int);

        float getMax(void);
        float getMin(void);
        float getMean(void);
        float getMedian(void);
        float getMode(void);
        float getVariance(void);
        void getHistogram(void);
        void getHistogramRow(int);
        void getHistogramColumn(int);

        void add(IFS3D,IFS3D);
        void subtract(IFS3D,IFS3D);
        void multiply(IFS3D,IFS3D);
        void divide(IFS3D,IFS3D,float);
        void add(IFS3D);
        void subtract(IFS3D);
        void multiply(IFS3D);
        void divide(IFS3D,float);
        void powerOf(float);
        void square(void);

        void copyImg(IFS3D);
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
        void Laplacian(float);
        void QuadraticVariation();

        void GradientMagnitude(float);
        void GradientDirection(float);

        void SavetoText(char[]);
        int WritetoDisk(char[]);
};
