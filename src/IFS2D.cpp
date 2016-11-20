/* Class IFS2D */
/* Contains class definition to operate on the IFS image*/
#include "IFS2D.h"
inline int DTofPixel(float **data,int rowind,int colind,int ro,int co);
inline void pause(int if1)
{
    static int  count=1;
    std::cout<<"\n stopped at: "<<count;
    count++;
    if(if1==1)
        getchar();

}
/*  CONSTRUCTOR  *********************************************************************************************
Description : this constructor opens the 2D IFS image with name "filename" and populates the ifsimage class data
Note        : the data member will be loaded only if the ifs image data type is float.
Arguments   : char filename[] : string for filename with path to the file 
 **************************************************************************************************************
 */
IFS2D::IFS2D(char filename[]){
    this->img=ifspin(filename);
    len=ifssiz(this->img);
    rows=len[2];
    columns=len[1];
    if(len[0]==2 && 0x200==(this->img->dtype & 0x200)) /*if data type is a float then assign */
    {
        ptr=fifs2farr(this->img);
    }
}

/*  CONSTRUCTOR  *********************************************************************************************
Description : this constructor creates a the IFS image of datatype "type" ,  and dimensions ROWS x COLUMNS
Note        : the data member will be loaded only if the ifs image data type is float.
Arguments   :   
type    : the (char *) to be used to set the type of the image
ROWS    : number of rows
COLUMNS : number of columns in the image
 *************************************************************************************************************
 */
IFS2D::IFS2D(char type[] , int ROWS , int COLUMNS):rows(ROWS) , columns(COLUMNS) 
{
    int len[3]={2 , columns , rows};
    this->img=ifscreate(type , len , IFS_CR_ALL , 0);
    if(0x200==(this->img->dtype & 0x200)) /*if data type is a float then assign */
    {
        ptr=fifs2farr(this->img);
    }
}
/*  CONSTRUCTOR  *********************************************************************************************
Description : this constructor creates IFS2D objecgt with Data initialised to thte IFSIMG 
Note        : the data member will be loaded only if the ifs image data type is float.
Arguments   :   
IFSIMG tar_1
 *************************************************************************************************************
 */
IFS2D::IFS2D(IFSIMG tar_1) 
{
    flcp(tar_1,this->img);
    len=ifssiz(this->img);
    rows=len[2];
    columns=len[1];
    if(0x200==(this->img->dtype & 0x200)) /*if data type is a float then assign */
    {
        ptr=fifs2farr(this->img);
    }


}
/*  CONSTRUCTOR  *********************************************************************************************
Description : this constructor creates IFS2D objecgt with Data initialised to thte argument IFS2D object 
Note        : the data member will be loaded only if the ifs image data type is float.
Arguments   :   
IFSIMG tar_1
 *************************************************************************************************************
 */
IFS2D::IFS2D(const IFS2D& src)
{
    this->len=ifssiz(src.img);
    this->rows=len[2];
    this->columns=len[1];
    img=ifscreate((char *)"float",len,IFS_CR_ALL,0);
    flcp(src.img,this->img);
    if(0x200==(this->img->dtype & 0x200)) /*if data type is a float then assign */
    {
        ptr=fifs2farr(this->img);
    }

}
/*  Function  ***********************************************************************************************
Description :   gets the pointer array describing the dimensions of the IFS image
Arguments   :   NONE
Return      :   int* : pointer to array , index 0=dimension , 1=columns , 2=rows
 *************************************************************************************************************
 */
int* IFS2D::getSize(void)
{
    return len;
}

/*  Function  ***********************************************************************************************
Description :   gets the number of ROWS in  the IFS image
Arguments   :   NONE
Return      :   int : number of rows
 *************************************************************************************************************
 */
int IFS2D::getRows(void)
{
    return rows;
}

/*  Function  ***********************************************************************************************
Description :   gets the number of COLUMNS in  the IFS image
Arguments   :   NONE
Return      :   int : number of columns
 *************************************************************************************************************
 */
int IFS2D::getColumns(void)
{
    return columns;
}

/*  Function  ***********************************************************************************************
Description :   gets the data pointer to the IFS image
Arguments   :   NONE
Return      :   float** : gets the data pointer to the image
 *************************************************************************************************************
 */
float** IFS2D::getDataPtr(void)
{
    if(0x200==(this->img->dtype & 0x200)) /*if data type is a float then assign */
    {
        return ptr;
    }
    else
    {
        std::cout<<"Not float type returning NULL POINTER";
        return NULL;
    }
}


/*  Function  ***********************************************************************************************
Description :   gets the pixel at a location in an image
Arguments   :   
int ROW
int COLUMN
Return      :   float : pixel value if the data is float
 *************************************************************************************************************
 */
float IFS2D::getPixel(int ROW , int COLUMN)
{
    return ifsfgp(this->img , ROW , COLUMN);
}

/*  Function  ***********************************************************************************************
Description :   sets the pixel at a location in an image
Arguments   :   
int ROW
int COLUMN
float VALUE
Return      :   float : pixel value if the data is float
 *************************************************************************************************************
 */
void IFS2D::putPixel(int ROW , int COLUMN , float VALUE)
{
    ifsfpp(this->img , ROW , COLUMN , VALUE);
}

/*  Function  ***********************************************************************************************
description :   adds tar_1 and tar_2 and loads it to img
arguments   :   
ifsimg tar_1
ifsimg tar_2
return      :   void
 *************************************************************************************************************
 */
void IFS2D::add(IFS2D tar_1 , IFS2D tar_2)
{
    for(int r=0;r<this->rows;r++)
    {
        for(int c=0;c<this->columns;c++)
        {
            float val_tar_1=tar_1.getPixel(r , c);
            float val_tar_2=tar_2.getPixel(r , c);
            this->putPixel(r , c , (float)(val_tar_1+val_tar_2));
        }
    }

}

/*  Function  ***********************************************************************************************
description :   subtracts tar_1 from tar_2 and loads it to img
arguments   :   
ifsimg tar_1
ifsimg tar_2
return      :   void
 *************************************************************************************************************
 */
void IFS2D::subtract(IFS2D tar_1 , IFS2D tar_2)
{
    for(int r=0;r<this->rows;r++)
    {
        for(int c=0;c<this->columns;c++)
        {
            float val_tar_1=tar_1.getPixel(r , c);
            float val_tar_2=tar_2.getPixel(r , c);
            this->putPixel(r , c , (float)(val_tar_1-val_tar_2));
        }
    }
}

/*  Function  ***********************************************************************************************
description :   multiplies tar_1 to tar_2 element wise and loads it to img
arguments   :   
ifsimg tar_1
ifsimg tar_2
return      :   void
 *************************************************************************************************************
 */
void IFS2D::multiply(IFS2D tar_1 , IFS2D tar_2)
{
    for(int r=0;r<this->rows;r++)
    {
        for(int c=0;c<this->columns;c++)
        {
            float val_tar_1=tar_1.getPixel(r , c);
            float val_tar_2=tar_2.getPixel(r , c);
            this->putPixel(r , c , (float)(val_tar_1*val_tar_2));
        }
    }
}

/*  Function  ***********************************************************************************************
description :   divides tar_1 by tar_2 element wise and loads it to img
note        : in case denominator is zero ,  division with default value of 0.01 done
arguments   :   
ifsimg tar_1
ifsimg tar_2
float divisor : value to use when denominator is zero
return      :   void
 *************************************************************************************************************
 */
void IFS2D::divide(IFS2D tar_1 , IFS2D tar_2 , float divisor=0.01)
{
    for(int r=0;r<this->rows;r++)
    {
        for(int c=0;c<this->columns;c++)
        {
            float val_tar_1=tar_1.getPixel(r , c);
            float val_tar_2=tar_2.getPixel(r , c);
            if(val_tar_2 == 0)
            {
                this->putPixel(r , c , (float)(val_tar_1/divisor));
            }
            else
            {
                this->putPixel(r , c , (float)(val_tar_1/val_tar_2));
            }
        }
    }
}

/*  Function  ***********************************************************************************************
description :   adds tar_1 to img
arguments   :   
ifsimg tar_1
return      :   void
 *************************************************************************************************************
 */
void IFS2D::add(IFS2D tar_1)
{
    for(int r=0;r<this->rows;r++)
    {
        for(int c=0;c<this->columns;c++)
        {
            float val_tar_1 = tar_1.getPixel(r , c);
            float val_img   = this->getPixel(r , c);
            this->putPixel(r , c , val_tar_1 + val_img);
        }
    }

}

/*  Function  ***********************************************************************************************
description :   subtracts tar_1 img
arguments   :   
ifsimg tar_1
return      :   void
 *************************************************************************************************************
 */
void IFS2D::subtract(IFS2D tar_1)
{
    for(int r=0;r<this->rows;r++)
    {
        for(int c=0;c<this->columns;c++)
        {
            float val_tar_1 = tar_1.getPixel(r , c);
            float val_img   = this->getPixel(r , c);
            this->putPixel(r , c , val_img - val_tar_1);
        }
    }
}

/*  Function  ***********************************************************************************************
description :   multiplies tar_1 with img
arguments   :   
ifsimg tar_1
return      :   void
 *************************************************************************************************************
 */
void IFS2D::multiply(IFS2D tar_1)
{
    for(int r=0;r<this->rows;r++)
    {
        for(int c=0;c<this->columns;c++)
        {
            float val_tar_1 = tar_1.getPixel(r , c);
            float val_img   = this->getPixel(r , c);
            this->putPixel(r , c , val_img * val_tar_1);
        }
    }
}

/*  Function  ***********************************************************************************************
description :   divides img with tar_1  element wise and loads it to img
note        : in case denominator is zero ,  division with default value of 0.01 done
arguments   :   
ifsimg tar_1
float divisor : value to use when denominator is zero
return      :   void
 *************************************************************************************************************
 */
void IFS2D::divide(IFS2D tar_1 , float divisor=0.01)
{
    for(int r=0;r<this->rows;r++)
    {
        for(int c=0;c<this->columns;c++)
        {
            float val_tar_1 = tar_1.getPixel(r , c);
            float val_img   = this->getPixel(r , c);
            if(val_tar_1 == 0)
            {
                this->putPixel(r , c , val_img / divisor);
            }
            else
            {
                this->putPixel(r , c , val_img / val_tar_1);
            }
        }
    }
}


/*  Function  ***********************************************************************************************
description :   adds scalar to img
arguments   :   
float scalar
return      :   void
 *************************************************************************************************************
 */
void IFS2D::add(float scalar)
{
    for(int r=0;r<this->rows;r++)
    {
        for(int c=0;c<this->columns;c++)
        {
            float val_img   = this->getPixel(r , c);
            this->putPixel(r , c , scalar + val_img);
        }
    }

}

/*  Function  ***********************************************************************************************
description :   subtracts tar_1 img
arguments   :   
float scalar
return      :   void
 *************************************************************************************************************
 */
void IFS2D::subtract(float scalar)
{
    for(int r=0;r<this->rows;r++)
    {
        for(int c=0;c<this->columns;c++)
        {
            float val_img   = this->getPixel(r , c);
            this->putPixel(r , c , val_img-scalar);
        }
    }
}

/*  Function  ***********************************************************************************************
description :   multiplies tar_1 with img
arguments   :   
float scalar
return      :   void
 *************************************************************************************************************
 */
void IFS2D::multiply(float scalar)
{
    for(int r=0;r<this->rows;r++)
    {
        for(int c=0;c<this->columns;c++)
        {
            float val_img   = this->getPixel(r , c);
            this->putPixel(r , c , val_img*scalar);
        }
    }
}

/*  Function  ***********************************************************************************************
description :   divides img with scalar  element wise and loads it to img
note        : in case denominator is zero ,  division with default value of 0.01 done
arguments   :   
float scalar
float divisor : value to use when denominator is zero
return      :   void
 *************************************************************************************************************
 */
void IFS2D::divide(float scalar)
{
    for(int r=0;r<this->rows;r++)
    {
        for(int c=0;c<this->columns;c++)
        {
            float val_img   = this->getPixel(r , c);
            this->putPixel(r , c , val_img / scalar);
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
void IFS2D::powerOf(float power)
{
    for(int r=0;r<this->rows;r++)
    {
        for(int c=0;c<this->columns;c++)
        {
            this->putPixel(r , c , (float)pow(this->getPixel(r,c) , power));
        }
    }
}
/*  Function  ***********************************************************************************************
description :   squares img
arguments   :   void
return      :   void
 *************************************************************************************************************
 */
void IFS2D::square(void)
{
    this->powerOf(2.0);
}

/*  Function  ***********************************************************************************************
description :   calculates the exponent of the image  element wise and loads it to img
arguments   :   void
return      :   void
 *************************************************************************************************************
 */
void IFS2D::exponent(void)
{
    for(int r=0;r<this->rows;r++)
    {
        for(int c=0;c<this->columns;c++)
        {
            this->putPixel(r , c , (float)exp(this->getPixel(r,c)));
        }
    }
}

/*  Function  ***********************************************************************************************
description :   copies the contents of tar_1 to img
Note        :   Please ensure that the dimensions match
arguments   :   
ifsimg2D tar_1
return      :   void
 *************************************************************************************************************
 */
void IFS2D::copyImg(IFS2D tar_1)
{
    for(int r=0;r<this->rows;r++)
    {
        for(int c=0;c<this->columns;c++)
        {
            this->putPixel(r , c , tar_1.getPixel(r , c));
        }
    }
}
/*  Function  ***********************************************************************************************
description :   copies the contents of tar_1 to img
Note        :   Please ensure that the dimensions match
arguments   :   
ifsimg2D tar_1
return      :   void
 *************************************************************************************************************
 */
void IFS2D::copyImg(IFS2D *tar_1)
{
    for(int r=0;r<this->rows;r++)
    {
        for(int c=0;c<this->columns;c++)
        {
            this->putPixel(r , c , tar_1->getPixel(r , c));
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
void IFS2D::threshold(float threshold)
{
    for(int r=0;r<this->rows;r++)
    {
        for(int c=0;c<this->columns;c++)
        {
            if(threshold < this->getPixel(r,c))
            {
                this->putPixel(r , c , 0);
            }
            else
            {
                this->putPixel(r , c , 255);
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
float IFS2D::getMax(void)
{
    float max=FLT_MIN;
    for(int r=0;r<this->rows;r++)
    {
        for(int c=0;c<this->columns;c++)
        {
            if(max < this->getPixel(r,c))
                max=this->getPixel(r,c);
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
float IFS2D::getMin(void)
{
    float min=FLT_MAX;
    for(int r=0;r<this->rows;r++)
    {
        for(int c=0;c<this->columns;c++)
        {
            if(min > this->getPixel(r,c))
                min=this->getPixel(r,c);
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
void IFS2D::scale(float NewMin , float NewMax)
{
    float OldMax=getMax();
    float OldMin=getMin();
    for(int r=0;r<this->rows;r++)
    {
        for(int c=0;c<this->columns;c++)
        {
            this->putPixel(r , c , ((NewMax-NewMin)*(this->getPixel(r,c)-NewMin)/(OldMax-OldMin)));
        }
    }
}

/*  Function  ***********************************************************************************************
description :   get Mean of the values
arguments   :   void
return      :   void
 *************************************************************************************************************
 */
float IFS2D::getMean(void)
{
    float sum=0.0;
    float count=0;
    for(int r=0;r<this->rows;r++)
    {
        for(int c=0;c<this->columns;c++)
        {
            sum+=this->getPixel(r,c);
            count++;
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
float IFS2D::getMedian(void)
{
    float  Median=0;
    std::vector<float> lexico;
    for(int r=0;r<this->rows;r++)
    {
        for(int c=0;c<this->columns;c++)
        {
            lexico.push_back(this->getPixel(r,c));
        }
    }
    std::sort(lexico.begin() , lexico.end());
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
float IFS2D::getMode(void)
{
    float  Mode=0 , ModeCount=0;
    std::vector<float> lexico;
    std::vector<float> histogram_value;
    std::vector<float> histogram_count;
    for(int r=0; r < rows ; r++)
    {
        for(int c=0; c < columns ; c++)
        {
            lexico.push_back(this->getPixel(r,c));
        }
    }
    std::sort(lexico.begin() , lexico.end());
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
void IFS2D::Gauss(float sigma)
{
    IFSIMG temp;
    temp=ifscreate((char *)"float" , ifssiz(this->img) , IFS_CR_ALL , 0);
    flcp(this->img,temp);
    flDoG(temp,this->img,(float)sigma , 0 , 0);
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
void IFS2D::DOG(float sigma , int order , int direction)
{
    IFSIMG temp;
    temp=ifscreate((char *)"float" , ifssiz(this->img) , IFS_CR_ALL , 0);
    flDoG(this->img , temp , sigma , order , direction);
    flcp(temp , this->img);
}
/*  Function  ***********************************************************************************************
description :   First Derivative  x direction  to the img
arguments   :   
float sigma
return      :   void
 *************************************************************************************************************
 */
void IFS2D::DOGx(float sigma)
{
    IFSIMG temp;
    temp=ifscreate((char *)"float" , ifssiz(this->img) , IFS_CR_ALL , 0);
    flDoG(this->img , temp , sigma , 1 , 0);
    flcp(temp , this->img);
}


/*  Function  ***********************************************************************************************
description :    First DerivativeG in y direction  to the img
arguments   :   
float sigma
return      :   void
 *************************************************************************************************************
 */
void IFS2D::DOGy(float sigma)
{
    IFSIMG temp;
    temp=ifscreate((char *)"float" , ifssiz(this->img) , IFS_CR_ALL , 0);
    flDoG(this->img , temp , sigma , 1 , 1);
    flcp(temp , this->img);
}
/*  Function  ***********************************************************************************************
description :  Second Derivative in  x direction  to the img
arguments   :   
float sigma
return      :   void
 *************************************************************************************************************
 */
void IFS2D::DOGx2(float sigma)
{
    IFSIMG temp;
    temp=ifscreate((char *)"float" , ifssiz(this->img) , IFS_CR_ALL , 0);
    flDoG(this->img , temp , sigma , 2 , 0);
    flcp(temp , this->img);
}


/*  Function  ***********************************************************************************************
description :   Second Derivative in y direction  to the img
arguments   :   
float sigma
return      :   void
 *************************************************************************************************************
 */
void IFS2D::DOGy2(float sigma)
{
    IFSIMG temp;
    temp=ifscreate((char *)"float" , ifssiz(this->img) , IFS_CR_ALL , 0);
    flDoG(this->img , temp , sigma , 2 , 1);
    flcp(temp , this->img);
}

/*  Function  ***********************************************************************************************
description :   first derivative of x followed by first derivative of y
arguments   :   
float sigma
return      :   void
 *************************************************************************************************************
 */
void IFS2D::DOGxy(float sigma)
{
    IFSIMG temp;
    temp=ifscreate((char *)"float" , ifssiz(this->img) , IFS_CR_ALL , 0);
    flDoG(this->img , temp , sigma , 1 , 0);
    flDoG(temp, this->img  , sigma , 1 , 1);
}
/*  Function  ***********************************************************************************************
description :    dx+dy of the img
arguments   :   
float sigma
return      :   void
 *************************************************************************************************************
 */
void IFS2D::DOGxDOGy(float sigma)
{
    IFSIMG tempx;
    tempx=ifscreate((char *)"float" , ifssiz(this->img) , IFS_CR_ALL , 0);
    flDoG(this->img , tempx , sigma , 1 , 0);

    IFSIMG tempy;
    tempy=ifscreate((char *)"float" , ifssiz(this->img) , IFS_CR_ALL , 0);
    flDoG(this->img , tempy , sigma , 1 , 1);
    fladdv(tempy , tempx,this->img);
}
/*  Function  ***********************************************************************************************
description :   Laplacian  to the img
arguments   :   
float sigma
return      :   void
 *************************************************************************************************************
 */
void IFS2D::Laplacian(float sigma)
{
    IFSIMG tar_1=ifscreate((char *)"float" , ifssiz(this->img) , IFS_CR_ALL , 0);
    IFSIMG tar_2=ifscreate((char *)"float" , ifssiz(this->img) , IFS_CR_ALL , 0);
    flDoG(this->img , tar_1 , sigma , 2 , 0);
    flDoG(this->img , tar_2 , sigma , 2 , 1);
    fladdv(tar_1 , tar_2 , this->img);
}


/*  Function  ***********************************************************************************************
description :  write img to disk with the filename filename
arguments   :   
char filename[]
return      :   int : 0 success , 1 failure
 *************************************************************************************************************
 */
int IFS2D::WritetoDisk(char filename[])
{
    int status=ifspot(this->img , filename);
    if(status == 0)
    {
        std::cout<<"\nERROR writing to disk :"<<filename<<"\n";
    }
    return status;
}

/*  Function  ***********************************************************************************************
description :   Magnitude of the gradient of the this->img at each pixel
arguments   :   
float sigma : value for sigma for DOG
return      :   void
 *************************************************************************************************************
 */
void IFS2D::GradientMagnitude(float sigma=1.0)
{
    IFSIMG tar_1=ifscreate((char *)"float" , ifssiz(this->img) , IFS_CR_ALL , 0);
    IFSIMG tar_2=ifscreate((char *)"float" , ifssiz(this->img) , IFS_CR_ALL , 0);

    flDoG(this->img , tar_1 , sigma , 1 , 0);
    flDoG(this->img , tar_2 , sigma , 1 , 1);
    flsq(tar_1 , tar_1);
    flsq(tar_2 , tar_2);
    fladdv(tar_1 , tar_2 , this->img);
    flsqrt(this->img , this->img);
}

/*  Function  ***********************************************************************************************
description :   Angle of the gradient of the img at each pixel
arguments   :   
float sigma : value for sigma for DOG
return      :   void
 *************************************************************************************************************
 */
void IFS2D::GradientDirection(float sigma=1.0)
{
    IFSIMG tar_1=ifscreate((char *)"float" , ifssiz(this->img) , IFS_CR_ALL , 0);
    IFSIMG tar_2=ifscreate((char *)"float" , ifssiz(this->img) , IFS_CR_ALL , 0);

    flDoG(this->img , tar_1 , sigma , 1 , 0);
    flDoG(this->img , tar_2 , sigma , 1 , 1);

    for(int r=0; r < rows ; r++)
    {
        for(int c=0; c < columns ; c++)
        {
            this->putPixel(r , c , atan2(ifsfgp(tar_2 , r , c) , ifsfgp(tar_1 , r , c)));
        }
    }
}

/*  Function  ***********************************************************************************************
description :   Angle of the gradient of the img at each pixel
arguments   :   
float sigma : value for sigma for DOG
return      :   void
 *************************************************************************************************************
 */
void IFS2D::GradientDirectionNormalized(float sigma=1.0)
{
    IFSIMG tar_1=ifscreate((char *)"float" , ifssiz(this->img) , IFS_CR_ALL , 0);
    IFSIMG tar_2=ifscreate((char *)"float" , ifssiz(this->img) , IFS_CR_ALL , 0);
    IFSIMG tar_3=ifscreate((char *)"float" , ifssiz(this->img) , IFS_CR_ALL , 0);

    flDoG(this->img , tar_1 , sigma , 1 , 0);
    flDoG(this->img , tar_2 , sigma , 1 , 1);
    flsq(tar_1 , tar_1);
    flsq(tar_2 , tar_2);
    fladdv(tar_1 , tar_2 , tar_3);
    flsqrt(tar_3 , tar_3);

    IFSIMG tar_11=ifscreate((char *)"float" , ifssiz(this->img) , IFS_CR_ALL , 0);
    IFSIMG tar_22=ifscreate((char *)"float" , ifssiz(this->img) , IFS_CR_ALL , 0);

    flDoG(this->img , tar_11 , sigma , 1 , 0);
    flDoG(this->img , tar_22 , sigma , 1 , 1);

    for(int r=0; r < rows ; r++)
    {
        for(int c=0; c < columns ; c++)
        {
            int xval=ifsfgp(tar_11 , r , c)/ifsfgp(tar_3 , r , c);
            int yval=ifsfgp(tar_22 , r , c)/ifsfgp(tar_3 , r , c);
            this->putPixel(r , c , atan2(yval,xval));
        }
    }
}

/*  Function  ***********************************************************************************************
description :   SubSample Image by a factor of 2
arguments   :   void
return      :   void
 *************************************************************************************************************
 */
void IFS2D::SubSample(void)
{
    IFSIMG temp=ifscreate((char *)"float",this->len,IFS_CR_ALL,0);
    flcp(this->img,temp);
    ifsfree(img,IFS_FR_ALL);
    this->len[1]=this->len[1]/2;
    this->len[2]=this->len[2]/2;
    this->rows=this->len[2];
    this->columns=this->len[1];

    this->img=ifscreate((char *)"float",this->len,IFS_CR_ALL,0);
    this->ptr=fifs2farr(this->img);
    for(int r=0;r<this->rows;r++)
    {
        for(int c=0;c<this->columns;c++)
        {
            float mean=ifsfgp(temp,(r*2),(c*2));
            mean+=ifsfgp(temp,(r*2)+1,(c*2));
            mean+=ifsfgp(temp,(r*2),(c*2)+1);
            mean+=ifsfgp(temp,(r*2)+1,(c*2)+1);
            mean=mean/4.0;
            ifsfpp(this->img,r,c,mean);
        }
    }


}

/*  Function  ***********************************************************************************************
description :   Find the neighborhood of pixels around the pixel 
arguments   :   
int row
int col
return      :   void
 *************************************************************************************************************
 */
void IFS2D::getNeighborhood(int ROW,int COL,float* neighborhood)
{
    if((ROW-1) >= 0)
    {
        if((COL-1) >= 0)
        { 
            neighborhood[0]=this->getPixel(ROW-1,COL-1);
        }
        else
        {
            neighborhood[0]=112233;
        }
        neighborhood[1]=this->getPixel(ROW-1,COL);
        if((COL+1) < this->getColumns())
        {
            neighborhood[2]=this->getPixel(ROW-1,COL+1);
        }
        else
        {
            neighborhood[2]=112233;
        }
    }
    else
    {
        neighborhood[0]=112233;
        neighborhood[1]=112233;
        neighborhood[2]=112233;

    }
    if((COL-1) >= 0)
    { 
        neighborhood[3]=this->getPixel(ROW,COL-1);
    }
    else
    {
        neighborhood[3]=112233;
    }
    neighborhood[4]=this->getPixel(ROW,COL);
    if((COL+1) < this->getColumns())
    {
        neighborhood[5]=this->getPixel(ROW,COL+1);
    }
    else
    {
        neighborhood[5]=112233;
    }
    if((ROW+1) < this->getRows())
    {
        if((COL-1) >= 0)
        { 
            neighborhood[6]=this->getPixel(ROW+1,COL-1);
        }
        else
        {
            neighborhood[6]=112233;
        }
        neighborhood[7]=this->getPixel(ROW+1,COL);
        if((COL+1) < this->getColumns())
        {
            neighborhood[8]=this->getPixel(ROW+1,COL+1);
        }
        else
        {
            neighborhood[8]=112233;
        }
    }
    else
    {
        neighborhood[0]=112233;
        neighborhood[1]=112233;
        neighborhood[2]=112233;
    }
}

/* Code for drawing circle from the following reference 
 * https://en.wikipedia.org/wiki/Midpoint_circle_algorithm : Variant with Integer-Based Arithmetic */
void IFS2D::DrawCircle(int x0, int y0, int radius)
{
    int x = radius;
    int y = 0;
    int decisionOver2 = 1 - x;   // Decision criterion divided by 2 evaluated at x=r, y=0

    while( y <= x )
    {
        ptr[x + x0][y + y0]=1.0; // Octant 1
        ptr[y + x0][x + y0]=1.0; // Octant 2
        ptr[-x + x0][y + y0]=1.0; // Octant 4
        ptr[-y + x0][x + y0]=1.0; // Octant 3
        ptr[-x + x0][-y + y0]=1.0; // Octant 5
        ptr[-y + x0][-x + y0]=1.0; // Octant 6
        ptr[x + x0][-y + y0]=1.0; // Octant 7
        ptr[y + x0][-x + y0]=1.0; // Octant 8
        y++;
        if (decisionOver2<=0)
        {
            decisionOver2 += 2 * y + 1;   // Change in decision criterion for y -> y+1
        }
        else
        {
            x--;
            decisionOver2 += 2 * (y - x) + 1;   // Change for y -> y+1, x -> x-1
        }
    }
}

void IFS2D::DrawCircle(int x0, int y0, int radius,float value)
{
    int x = radius;
    int y = 0;
    int decisionOver2 = 1 - x;   // Decision criterion divided by 2 evaluated at x=r, y=0

    while( y <= x )
    {
        ptr[x + x0][y + y0]=value; // Octant 1
        ptr[y + x0][x + y0]=value; // Octant 2
        ptr[-x + x0][y + y0]=value; // Octant 4
        ptr[-y + x0][x + y0]=value; // Octant 3
        ptr[-x + x0][-y + y0]=value; // Octant 5
        ptr[-y + x0][-x + y0]=value; // Octant 6
        ptr[x + x0][-y + y0]=value; // Octant 7
        ptr[y + x0][-x + y0]=value; // Octant 8
        y++;
        if (decisionOver2<=0)
        {
            decisionOver2 += 2 * y + 1;   // Change in decision criterion for y -> y+1
        }
        else
        {
            x--;
            decisionOver2 += 2 * (y - x) + 1;   // Change for y -> y+1, x -> x-1
        }
    }
}


void IFS2D::DistanceTransform(void)
{
    int row=0,col=0;
    int DT=1;
    int count=0;
    /*invert all 1's to 0's and 0's to 1's*/
    for(row=0;row<this->rows;row++)
    {
        for(col=0;col<this->columns;col++)
        {
            if(ptr[row][col]==1)
                ptr[row][col]=0;
            else
                ptr[row][col]=255.0;
        }
    }
    do{
        count=0;
        for(row=0;row<this->rows;row++)
        {
            for(col=0;col<this->columns;col++)
            {
                if(ptr[row][col]!=0) 
                {
                    DT=DTofPixel(ptr,row,col,this->rows,this->columns);
                    if(DT!=-1)
                    {
                        if(ptr[row][col] != (DT+1))
                        {
                            count++;  
                        }
                        ptr[row][col]=DT+1;
                    }
                }
            }
        }
    }while(count>0);
}

inline int DTofPixel(float **data,int rowind,int colind,int ro,int co)
{
    int DT=0;
    int right=255,left=255,bottom=255,top=255;
    if((rowind+1) < ro){    bottom=data[rowind+1][colind];  }
    if((rowind-1) > 0) {    top=data[rowind-1][colind];  }
    if((colind+1) < co){    right=data[rowind][colind+1];  }
    if((colind-1) > 0) {    left=data[rowind][colind-1];  }

    if((right==left)&&(top==bottom)&&(right==top))  {
        DT=-1;
    }
    else
    {
        DT=std::min(right,std::min(left,std::min(top,bottom)));
    }
    return DT;
}

void IFS2D::InvertDTInside(void)
{
    int startCirc=0,endCirc=this->columns-1;
    int invert=0;
    for(int row=0;row<this->rows;row++)
    {
        invert=0;
        for(int col=0;col<this->columns;col++)
        {
            if(this->ptr[row][col]==0)
            {
                invert++;
                startCirc=col;
                break;
            }
        }
        for(int col=this->columns-1;col>=0;col--)
        {
            if(this->ptr[row][col]==0)
            {
                invert++;
                endCirc=col;
                break;
            }
        }
        if(invert==2)
        {
            for(int col = startCirc ; col < endCirc ;col++)
            {
                ptr[row][col]=-ptr[row][col];
            }
        }
    }
}
/*  Function  ***********************************************************************************************
description :   Calculate the curvature at a point
arguments   :   void
return      :   void
 *************************************************************************************************************
 */
void IFS2D::Curvature(void)
{
    IFS2D DvX ((char *)"float",this->rows,this->columns);
    IFS2D DvY ((char *)"float",this->rows,this->columns);
    IFS2D Dv2X ((char *)"float",this->rows,this->columns);
    IFS2D Dv2Y ((char *)"float",this->rows,this->columns);
    IFS2D DvXY ((char *)"float",this->rows,this->columns);
    IFS2D denominator ((char *)"float",this->rows,this->columns);
    IFS2D numerator ((char *)"float",this->rows,this->columns);
    DvX.copyImg(this);
    DvY.copyImg(this);
    Dv2X.copyImg(this);
    Dv2Y.copyImg(this);
    DvXY.copyImg(this);

    DvX.DOGx(1.0);pause(0);
    DvY.DOGy(1.0);pause(0);
    Dv2X.DOGx2(1.0);pause(0);
    Dv2X.DOGy2(1.0);pause(0);
    DvXY.DOGxy(1.0);pause(0);

    DvXY.multiply(DvX);pause(0);
    DvXY.multiply(DvY);pause(0);
    DvXY.multiply(2.0);pause(0);
    DvY.square();pause(0);
    DvX.square();pause(0);
    Dv2Y.multiply(DvY);pause(0);
    Dv2X.multiply(DvX);pause(0);

    numerator.add(Dv2Y,Dv2X);pause(0);
    numerator.add(DvXY);pause(0);

    denominator.add(DvX,DvY);pause(0);
    denominator.add(0.01);
    denominator.powerOf(1.5); pause(0);
    this->divide(numerator,denominator,0.01);pause(0);

}

void IFS2D::Normalize(void)
{
    float max=this->getMax();
    for(int r=0;r<this->rows;r++)
    {
        for(int c=0;c<this->columns;c++)
        {
            this->putPixel(r,c,this->getPixel(r,c)/max);
        }
    }
}

