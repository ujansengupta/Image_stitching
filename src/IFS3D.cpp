/* Class IFS3D */
/* Contains class definition to operate on the IFS image*/
#include "IFS3D.h"

/*  CONSTRUCTOR  *********************************************************************************************
Description : this constructor opens the 3D IFS image with name "filename" and populates the ifsimage class data
Note        : the data member will be loaded only if the ifs image data type is float.
Arguments   : char filename[] : string for filename with path to the file 
 **************************************************************************************************************
 */
IFS3D::IFS3D(char filename[]){
    this->img=ifspin(filename);
    len=ifssiz(this->img);
    columns=len[1];
    rows=len[2];
    frames=len[3];
}

/*  CONSTRUCTOR  *********************************************************************************************
Description : this constructor creates a the IFS image of datatype "type", and dimensions ROWS x COLUMNS
Note        : the data member will be loaded only if the ifs image data type is float.
Arguments   :   
type    : the (char *) to be used to set the type of the image
ROWS    : number of rows
COLUMNS : number of columns in the image
 *************************************************************************************************************
 */
IFS3D::IFS3D(char type[],int ROWS,int COLUMNS,int FRAMES):rows(ROWS),columns(COLUMNS),frames(FRAMES)
{
    int len[4]={3,columns,rows,frames};
    this->img=ifscreate(type,len,IFS_CR_ALL,0);
}
/*  Function  ***********************************************************************************************
Description :   gets the pointer array describing the dimensions of the IFS image
Arguments   :   NONE
Return      :   int* : pointer to array , index 0=dimension , 1=columns , 2=rows
 *************************************************************************************************************
 */
int* IFS3D::getSize(void)
{
    return len;
}

/*  Function  ***********************************************************************************************
Description :   gets the number of ROWS in  the IFS image
Arguments   :   NONE
Return      :   int : number of rows
 *************************************************************************************************************
 */
int IFS3D::getRows(void)
{
    return rows;
}

/*  Function  ***********************************************************************************************
Description :   gets the number of COLUMNS in  the IFS image
Arguments   :   NONE
Return      :   int : number of columns
 *************************************************************************************************************
 */
int IFS3D::getColumns(void)
{
    return columns;
}

/*  Function  ***********************************************************************************************
Description :   gets the number of Frames in  the IFS image
Arguments   :   NONE
Return      :   int : number of frames
 *************************************************************************************************************
 */
int IFS3D::getFrames(void)
{
    return frames;
}

/*  Function  ***********************************************************************************************
Description :   gets the data pointer to the IFS image
Arguments   :   NONE
NOTE        :   NOT POSSIBLE FOR 3D Image
Return      :   float** : gets the data pointer to the image
 *************************************************************************************************************
 */
float** IFS3D::getDataPtr(void)
{
    return NULL;
}


/*  Function  ***********************************************************************************************
Description :   gets the pixel at a location in an image
Arguments   :   
int ROW
int COLUMN
Return      :   float : pixel value if the data is float
 *************************************************************************************************************
 */
inline float IFS3D::getPixel(int ROW,int COLUMN,int FRAME)
{
    return ifsfgp3d(this->img,FRAME,ROW,COLUMN);
}

/*  Function  ***********************************************************************************************
Description :   sets the pixel at a location in an image
Arguments   :   
int ROW
int COLUMN
int FRAME
float VALUE
Return      :   float : pixel value if the data is float
 *************************************************************************************************************
 */
inline float IFS3D::putPixel(int ROW,int COLUMN,int FRAME,float VALUE)
{
    return ifsfpp3d(this->img,FRAME,ROW,COLUMN,VALUE);
}

/*  Function  ***********************************************************************************************
Description :   returns a frame
Arguments   :   
int FRAME
float VALUE
Return      :   float : pixel value if the data is float
 *************************************************************************************************************
 */
IFSIMG IFS3D::getFrame(int FRAME)
{
    IFSIMG img_1=ifsslice(this->img,(char *)"f",FRAME);
    return img_1;
}
/*  Function  ***********************************************************************************************
Description :   replaces the frame
Arguments   :   
int FRAME
float VALUE
Return      :   float : pixel value if the data is float
 *************************************************************************************************************
 */
void  IFS3D::putFrame(IFS2D img_1,int frameNumber)
{
    for(int r=0;r<rows;r++)
    {
        for(int c=0;c<columns;c++)
        {
            this->putPixel(r,c,frameNumber,img_1.getPixel(r,c));
        }
    }

}
/*  Function  ***********************************************************************************************
description :   adds img_1 and img_2 and loads it to img
arguments   :   
ifsimg img_1
ifsimg img_2
return      :   void
 *************************************************************************************************************
 */
void IFS3D::add(IFS3D img_1,IFS3D img_2)
{
    for(int f=0;f<frames;f++)
    {
        for(int r=0;r<rows;r++)
        {
            for(int c=0;c<columns;c++)
            {
                float val_img_1=img_1.getPixel(r,c,f);
                float val_img_2=img_2.getPixel(r,c,f);
                this->putPixel(r,c,f,(float)(val_img_1+val_img_2));
            }
        }
    }

}

/*  Function  ***********************************************************************************************
description :   subtracts img_1 from img_2 and loads it to img
arguments   :   
ifsimg img_1
ifsimg img_2
return      :   void
 *************************************************************************************************************
 */
void IFS3D::subtract(IFS3D img_1,IFS3D img_2)
{
    for(int f=0;f<frames;f++)
    {
        for(int r=0;r<rows;r++)
        {
            for(int c=0;c<columns;c++)
            {
                float val_img_1=img_1.getPixel(r,c,f);
                float val_img_2=img_2.getPixel(r,c,f);
                this->putPixel(r,c,f,(float)(val_img_1-val_img_2));
            }
        }
    }
}

/*  Function  ***********************************************************************************************
description :   multiplies img_1 to img_2 element wise and loads it to img
arguments   :   
ifsimg img_1
ifsimg img_2
return      :   void
 *************************************************************************************************************
 */
void IFS3D::multiply(IFS3D img_1,IFS3D img_2)
{
    for(int f=0;f<frames;f++)
    {
        for(int r=0;r<rows;r++)
        {
            for(int c=0;c<columns;c++)
            {
                float val_img_1=img_1.getPixel(r,c,f);
                float val_img_2=img_2.getPixel(r,c,f);
                this->putPixel(r,c,f,(float)(val_img_1*val_img_2));
            }
        }
    }
}

/*  Function  ***********************************************************************************************
description :   divides img_1 by img_2 element wise and loads it to img
note        : in case denominator is zero, division with default value of 0.01 done
arguments   :   
ifsimg img_1
ifsimg img_2
float divisor : value to use when denominator is zero
return      :   void
 *************************************************************************************************************
 */
void IFS3D::divide(IFS3D img_1,IFS3D img_2,float divisor=0.01)
{
    for(int f=0;f<frames;f++)
    {
        for(int r=0;r<rows;r++)
        {
            for(int c=0;c<columns;c++)
            {
                float val_img_1=img_1.getPixel(r,c,f);
                float val_img_2=img_2.getPixel(r,c,f);
                if(val_img_2 == 0)
                {
                    this->putPixel(r,c,f,(float)(val_img_1/divisor));
                }
                else
                {
                    this->putPixel(r,c,f,(float)(val_img_1/val_img_2));
                }
            }
        }
    }
}

/*  Function  ***********************************************************************************************
description :   adds img_1 to img
arguments   :   
ifsimg img_1
return      :   void
 *************************************************************************************************************
 */
void IFS3D::add(IFS3D img_1)
{
    for(int f=0;f<frames;f++)
    {
        for(int r=0;r<rows;r++)
        {
            for(int c=0;c<columns;c++)
            {
                float val_img=this->getPixel(r,c,f);
                float val_img_1=img_1.getPixel(r,c,f);
                this->putPixel(r,c,f,(float)(val_img+val_img_1));
            }
        }
    }
}

/*  Function  ***********************************************************************************************
description :   subtracts img_1 img
arguments   :   
ifsimg img_1
return      :   void
 *************************************************************************************************************
 */
void IFS3D::subtract(IFS3D img_1)
{
    for(int f=0;f<frames;f++)
    {
        for(int r=0;r<rows;r++)
        {
            for(int c=0;c<columns;c++)
            {
                float val_img=this->getPixel(r,c,f);
                float val_img_1=img_1.getPixel(r,c,f);
                this->putPixel(r,c,f,(float)(val_img-val_img_1));
            }
        }
    }
}

/*  Function  ***********************************************************************************************
description :   multiplies img_1 with img
arguments   :   
ifsimg img_1
return      :   void
 *************************************************************************************************************
 */
void IFS3D::multiply(IFS3D img_1)
{
    for(int f=0;f<frames;f++)
    {
        for(int r=0;r<rows;r++)
        {
            for(int c=0;c<columns;c++)
            {
                float val_img=this->getPixel(r,c,f);
                float val_img_1=img_1.getPixel(r,c,f);
                this->putPixel(r,c,f,(float)(val_img*val_img_1));
            }
        }
    }
}

/*  Function  ***********************************************************************************************
description :   divides img with img_1  element wise and loads it to img
note        : in case denominator is zero, division with default value of 0.01 done
arguments   :   
ifsimg img_1
float divisor : value to use when denominator is zero
return      :   void
 *************************************************************************************************************
 */
void IFS3D::divide(IFS3D img_1,float divisor=0.01)
{
    for(int f=0;f<frames;f++)
    {
        for(int r=0;r<rows;r++)
        {
            for(int c=0;c<columns;c++)
            {
                float val_img=this->getPixel(r,c,f);
                float val_img_1=img_1.getPixel(r,c,f);
                if(val_img_1 == 0)
                {
                    this->putPixel(r,c,f,(float)(val_img/divisor));
                }
                else
                {
                    this->putPixel(r,c,f,(float)(val_img/val_img_1));
                }
            }
        }
    }
}

/*  Function  ***********************************************************************************************
description :   calculates the power  of the image  element wise and loads it to img
arguments   :   
float power
return      :   void
 *************************************************************************************************************
 */
void IFS3D::powerOf(float power)
{
    for(int f=0;f<frames;f++)
    {
        for(int r=0;r<rows;r++)
        {
            for(int c=0;c<columns;c++)
            {
                float val_img=this->getPixel(r,c,f);
                this->putPixel(r,c,f,(float)pow(val_img,power));
            }
        }
    }
}
/*  Function  ***********************************************************************************************
description :   squares img
arguments   :   void
return      :   void
 *************************************************************************************************************
 */
void IFS3D::square(void)
{
    this->powerOf(2.0);
}

/*  Function  ***********************************************************************************************
description :   copies the contents of img_1 to img
Note        :   Please ensure that the dimensions match
arguments   :   
ifsimg3D img_1
return      :   void
 *************************************************************************************************************
 */
void IFS3D::copyImg(IFS3D img_1)
{
    for(int f=0;f<frames;f++)
    {
        for(int r=0;r<rows;r++)
        {
            for(int c=0;c<columns;c++)
            {
                float val_img_1=img_1.getPixel(r,c,f);
                this->putPixel(r,c,f,(float)(val_img_1));
            }
        }
    }
}

/*  Function  ***********************************************************************************************
description :   thresholds the img with value passed and sets values with binary 0|255
the value is 0 if value is less thann threshold. 255 otherwise
arguments   :   
float threshold
return      :   void
 *************************************************************************************************************
 */
void IFS3D::threshold(float threshold)
{
    for(int f=0;f<frames;f++)
    {
        for(int r=0;r<rows;r++)
        {
            for(int c=0;c<columns;c++)
            {
                float val_img=this->getPixel(r,c,f);
                if(threshold<val_img)
                {
                    this->putPixel(r,c,f,0.0);
                }
                else
                {
                    this->putPixel(r,c,f,255.0);
                }
            }
        }
    }
}

/*  Function  ***********************************************************************************************
description :   get the maximum of the values 
arguments   :   void
return      :   float Max
 *************************************************************************************************************
 */
float IFS3D::getMax(void)
{
    float max=FLT_MIN;
    for(int f=0;f<frames;f++)
    {
        for(int r=0;r<rows;r++)
        {
            for(int c=0;c<columns;c++)
            {
                if(max < this->getPixel(r,c,f))
                {
                    max=this->getPixel(r,c,f);
                }
            }
        }
    }
    return max;
}
/*  Function  ***********************************************************************************************
description :   get the minimum of the values 
arguments   :   void
return      :   float Min
 *************************************************************************************************************
 */
float IFS3D::getMin(void)
{
    float min=FLT_MAX;
    for(int f=0;f<frames;f++)
    {
        for(int r=0;r<rows;r++)
        {
            for(int c=0;c<columns;c++)
            {
                if(min > this->getPixel(r,c,f))
                {
                    min=this->getPixel(r,c,f);
                }
            }
        }
    }
    return min;
}
/*  Function  ***********************************************************************************************
description :   scale the values of the image to the limits mentioned
arguments   :   
float min
float max
return      :   void
 *************************************************************************************************************
 */
void IFS3D::scale(float NewMin,float NewMax)
{
    float OldMax=this->getMax();
    float OldMin=this->getMin();
    for(int f=0;f<frames;f++)
    {
        for(int r=0;r<rows;r++)
        {
            for(int c=0;c<columns;c++)
            {
                float newValue=((NewMax-NewMin)*(this->getPixel(r,c,f)-NewMin)/(OldMax-OldMin));
                this->putPixel(r,c,f,newValue);
            }
        }
    }
}

/*  Function  ***********************************************************************************************
description :   get Mean of the values
arguments   :   void
return      :   void
 *************************************************************************************************************
 */
float IFS3D::getMean(void)
{
    float sum=0.0;
    float count=0;
    for(int f=0;f<frames;f++)
    {
        for(int r=0;r<rows;r++)
        {
            for(int c=0;c<columns;c++)
            {
                sum+=this->getPixel(r,c,f);
                count++;
            }
        }
    }
    return (sum/count);
}

/*  Function  ***********************************************************************************************
description :   get Median of the values
arguments   :   void
return      :   void
 *************************************************************************************************************
 */
float IFS3D::getMedian(void)
{
    float  Median=0;
    std::vector<float> lexico;
    for(int f=0;f<frames;f++)
    {
        for(int r=0;r<rows;r++)
        {
            for(int c=0;c<columns;c++)
            {
                lexico.push_back(this->getPixel(r,c,f));
            }
        }
    }
    std::sort(lexico.begin(),lexico.end());
    if(lexico.size()%2 ==0)
    {
        Median=(lexico.at(lexico.size()/2)+lexico.at((lexico.size()/2)+1))/2;
    }
    else
    {
        Median=lexico.at(lexico.size()/2);
    }
    return Median;
}

/*  Function  ***********************************************************************************************
description :   get Mode of the values
arguments   :   void
return      :   void
 *************************************************************************************************************
 */
float IFS3D::getMode(void)
{
    float  Mode=0,ModeCount=0;
    std::vector<float> lexico;
    std::vector<float> histogram_value;
    std::vector<float> histogram_count;
    for(int f=0;f<frames;f++)
    {
        for(int r=0;r<rows;r++)
        {
            for(int c=0;c<columns;c++)
            {
                lexico.push_back(this->getPixel(r,c,f));
            }
        }
    }
    std::sort(lexico.begin(),lexico.end());
    for(unsigned int i=0; i< lexico.size() ; i++)
    {
        if(i==0)
        {
            histogram_value.push_back(lexico.at(i));
            histogram_count.push_back(1);
        }
        else
        {
            if(histogram_value.back() == lexico.at(i))
            {
                histogram_count.back()=histogram_count.back()+1;
            }
            else
            {
                histogram_value.push_back(lexico.at(i));
                histogram_count.push_back(1);
            }
        }
    }
    for(unsigned int i=0 ; i < lexico.size() ; i++)
    {
        if(ModeCount>histogram_count.at(i))
        {
            Mode=histogram_value.at(i);
            ModeCount=histogram_count.at(i);
        }
    }
    return Mode;
}

/*  Function  ***********************************************************************************************
description :   apply gaussian blur with mean=0 and SD defined to the img
arguments   :   
float sigma
return      :   void
 *************************************************************************************************************
 */
void IFS3D::Gauss(float sigma)
{
    IFSIMG temp;
    temp=ifscreate((char *)"float",ifssiz(this->img),IFS_CR_ALL,0);
    flDoG(img,temp,sigma,0,0);
    flcp(temp,this->img);
}

/*  Function  ***********************************************************************************************
description :   Apply DOG with ifs parameter to the img
arguments   :   
float sigma
int order
int direction
return      :   void
 *************************************************************************************************************
 */
void IFS3D::DOG(float sigma,int order,int direction)
{
    IFSIMG temp;
    temp=ifscreate((char *)"float",ifssiz(img),IFS_CR_ALL,0);
    flDoG(this->img,temp,sigma,order,direction);
    flcp(temp,this->img);
}
/*  Function  ***********************************************************************************************
description :   First Derivative  x direction  to the img
arguments   :   
float sigma
return      :   void
 *************************************************************************************************************
 */
void IFS3D::DOGx(float sigma)
{
    IFSIMG temp;
    temp=ifscreate((char *)"float",ifssiz(this->img),IFS_CR_ALL,0);
    flDoG(this->img,temp,sigma,1,0);
    flcp(temp,this->img);
}


/*  Function  ***********************************************************************************************
description :    First DerivativeG in y direction  to the img
arguments   :   
float sigma
return      :   void
 *************************************************************************************************************
 */
void IFS3D::DOGy(float sigma)
{
    IFSIMG temp;
    temp=ifscreate((char *)"float",ifssiz(this->img),IFS_CR_ALL,0);
    flDoG(this->img,temp,sigma,1,1);
    flcp(temp,this->img);
}
/*  Function  ***********************************************************************************************
description :  Second Derivative in  x direction  to the img
arguments   :   
float sigma
return      :   void
 *************************************************************************************************************
 */
void IFS3D::DOGx2(float sigma)
{
    IFSIMG temp;
    temp=ifscreate((char *)"float",ifssiz(this->img),IFS_CR_ALL,0);
    flDoG(this->img,temp,sigma,2,0);
    flcp(temp,this->img);
}


/*  Function  ***********************************************************************************************
description :   Second Derivative in y direction  to the img
arguments   :   
float sigma
return      :   void
 *************************************************************************************************************
 */
void IFS3D::DOGy2(float sigma)
{
    IFSIMG temp;
    temp=ifscreate((char *)"float",ifssiz(this->img),IFS_CR_ALL,0);
    flDoG(this->img,temp,sigma,2,1);
    flcp(temp,this->img);
}

/*  Function  ***********************************************************************************************
description :   Laplacian  to the img
arguments   :   
float sigma
return      :   void
 *************************************************************************************************************
 */
void IFS3D::Laplacian(float sigma)
{
    IFSIMG img_1=ifscreate((char *)"float",ifssiz(this->img),IFS_CR_ALL,0);
    IFSIMG img_2=ifscreate((char *)"float",ifssiz(this->img),IFS_CR_ALL,0);
    flDoG(this->img,img_1,sigma,2,0);
    flDoG(this->img,img_2,sigma,2,1);
    fladdv(img_1,img_2,this->img);
}


/*  Function  ***********************************************************************************************
description :  write img to disk with the filename filename
arguments   :   
char filename[]
return      :   int : 0 success , 1 failure
 *************************************************************************************************************
 */
int IFS3D::WritetoDisk(char filename[])
{
    int status=ifspot(this->img,filename);
    if(status == 0)
    {
        std::cout<<"\nERROR writing to disk :"<<filename<<"\n";
    }
    return status;
}

/*  Function  ***********************************************************************************************
description :   Magnitude of the gradient of the img at each pixel
arguments   :   
float sigma : value for sigma for DOG
return      :   void
 *************************************************************************************************************
 */
void IFS3D::GradientMagnitude(float sigma=1.0)
{
    IFSIMG img_1=ifscreate((char *)"float",ifssiz(img),IFS_CR_ALL,0);
    IFSIMG img_2=ifscreate((char *)"float",ifssiz(img),IFS_CR_ALL,0);

    flDoG(img,img_1,sigma,1,0);
    flDoG(img,img_2,sigma,1,1);
    flsq(img_1,img_1);
    flsq(img_2,img_2);
    fladdv(img_1,img_2,img);
    flsqrt(img,img);
}

/*  Function  ***********************************************************************************************
description :   Angle of the gradient of the img at each pixel
arguments   :   
float sigma : value for sigma for DOG
return      :   void
 *************************************************************************************************************
 */
void IFS3D::GradientDirection(float sigma=1.0)
{
    IFSIMG img_1=ifscreate((char *)"float",ifssiz(img),IFS_CR_ALL,0);
    IFSIMG img_2=ifscreate((char *)"float",ifssiz(img),IFS_CR_ALL,0);

    flDoG(img,img_1,sigma,1,0);
    flDoG(img,img_2,sigma,1,1);

    for(int r=0; r < rows ; r++)
    {
        for(int c=0; c < columns ; c++)
        {
            //ptr[r][c]=atan2(ifsfgp(img_2,r,c),ifsfgp(img_1,r,c));
        }
    }
}

