// main.cpp


// my own header
#include "../hdr/main.h"


using namespace std;
// constants
#define Scale 1
#define ErrCheck 10
#define NeiSiz 20
#define Diffthreshold 10
#define offset 100
//#define threshold 10
typedef std::vector<int> IntPoints8;
typedef std::vector<IntPoints8> Sector16;
typedef std::vector<Sector16> imgVector;
// function declarations
    template <typename T>
string ntos ( T Number ,string name)
{
    ostringstream ss;
    ss << Number;
    string path="./../output/"+name+"_"+ss.str()+".ifs";
    return path;
}
void pause(int set)
{
    static int count=0;
    cout<<"\nstopped at :"<<count;
    if(set==1)
    {
        cout<<"\n Press enter to continue :";
        getchar();
    }
    count++;
}
void FindIntPt(std::vector<long>& Pts,IFS2D src_img,int filecount);
void FindTemplateCorrespondences(IFS2D img1,IFS2D img2,std::vector<long>& img1_IntPt,std::vector<long>& img2_IntPt);
void FindSIFTCorrespondences(IFS2D img1,IFS2D img2,std::vector<long>& img1_IntPt,std::vector<long>& img2_IntPt);
int CheckIfMax(int r,int c,int sigma);
float compareMin(float arr_1[],float arr_2[],float arr_3[]);
inline void GetNeighbor(IFS2D src_img,float neighbor[NeiSiz*2][NeiSiz*2],int rowInd,int colInd);
float CompareNeighborPercent(float neighbor1[NeiSiz*2][NeiSiz*2],float neighbor2[NeiSiz*2][NeiSiz*2]);
float CompareNeighborSum(float neighbor1[NeiSiz*2][NeiSiz*2],float neighbor2[NeiSiz*2][NeiSiz*2]);
void getFeatureVector(IFS2D Gradimg,int rowInd,int colInd,std::vector< std::vector < std::vector <int> > >& IMGVector,int index);
void matchVectors(imgVector& img1Vector,imgVector& img2Vector,int img1Pts,int img2Pts);
void DrawLine(IFS2D img,int x0,int y0,int x1,int y1);
void Line( IFS2D &img,const float x1, const float y1, const float x2, const float y2);
void Line3D( IFS3D &img,const float x1, const float y1, const float x2, const float y2);
void DrawCorrespondences(IFS2D img1,IFS2D img2,char filename[]);
void getCorPoint(float **Xmatrix,float **X_matrix,float **Hmatrix);

// global declarations
const float Sigma[]={0.707*Scale,1.0*Scale,1.414*Scale,2.0*Scale,2.83*Scale};
float threshold=0;
float PercentThreshold=0;
//const float Sigma[]={2.828,4.0,5.656,8.0,11.32};

/**************************************************************/

int main(int argc,char *argv[])
{
    IFS2D LeftImg (argv[1]);
    IFS2D RightImg (argv[2]);
    IFS2D StitchedImg ((char *)"float",LeftImg.getRows()*2,RightImg.getRows()*2);
    IFS2D StitchedImg2 ((char *)"float",LeftImg.getRows()*2,RightImg.getRows()*2);
    threshold=atof(argv[3]);
    PercentThreshold=atof(argv[4]);
    std::vector<long> img1_IntPt;
    std::vector<long> img2_IntPt;

    FindIntPt(img1_IntPt,LeftImg,1);
    FindIntPt(img2_IntPt,RightImg,2);

    cout<<"\nNumber of img1 Interest Pts :"<<img1_IntPt.size();
    cout<<"\nNumber of img2 Interest Pts :"<<img2_IntPt.size();

    LeftImg.WritetoDisk((char *)"./../output/firstnormal.ifs");
    RightImg.WritetoDisk((char *)"./../output/secondnormal.ifs");
    //FindTemplateCorrespondences(LeftImg,RightImg,img1_IntPt,img2_IntPt);
    FindSIFTCorrespondences(LeftImg,RightImg,img1_IntPt,img2_IntPt);
    DrawCorrespondences(LeftImg,RightImg,(char *)"./../output/BookInput.txt");
    float **Hmatrix=matrix(1,3,1,3);
    FindHMatrix((char *)"./../output/BookInput.txt",Hmatrix);

    for(int row=0;row < LeftImg.getRows();row++)
    {
        for(int col=0; col <LeftImg.getColumns();col++)
        {
            StitchedImg.putPixel(row+offset,col,LeftImg.getPixel(row,col));
            StitchedImg2.putPixel(row+offset,col,LeftImg.getPixel(row,col));
        }
    }
    for(int row=0;row < StitchedImg.getRows();row++)
    {
        for(int col=0; col < StitchedImg.getColumns();col++)
        {
            float **Xmatrix;
            float **Xdmatrix;
            Xmatrix=matrix(1,3,1,1);
            Xdmatrix=matrix(1,3,1,1);
            Xdmatrix[1][1]=row;
            Xdmatrix[2][1]=col;
            Xdmatrix[3][1]=1;
            getCorPoint(Xdmatrix,Xmatrix,Hmatrix);
            //cout<<"right "<<(int)Xmatrix[1][1]<<" "<<(int)Xmatrix[2][1]<<" left "<<(int)Xdmatrix[1][1]<<" "<<(int)Xdmatrix[2][1]<<"\n";
            int rtrow=(int)Xmatrix[1][1];
            int rtcol=(int)Xmatrix[2][1];
            if(rtrow > 0 && rtrow < RightImg.getRows() && rtcol > 0 && rtcol < RightImg.getColumns())
            {
                StitchedImg.putPixel(row+(offset),col,StitchedImg.getPixel(row+offset,col)+ RightImg.getPixel(rtrow,rtcol));
                StitchedImg2.putPixel(row+(offset),col,RightImg.getPixel(rtrow,rtcol));
            }
        }
    }    
    pause(1);
    StitchedImg.WritetoDisk((char *)"./../output/Stitched.ifs");
    pause(1);
    StitchedImg2.WritetoDisk((char *)"./../output/Stitched2.ifs");
    return 0;
}
void getCorPoint(float **Xmatrix,float **X_matrix,float **Hmatrix)
{
    ifsmatmult(Hmatrix,Xmatrix,X_matrix,3,3,3,1); 
    X_matrix[1][1]=X_matrix[1][1]/X_matrix[3][1];
    X_matrix[2][1]=X_matrix[2][1]/X_matrix[3][1];
    X_matrix[3][1]=X_matrix[3][1]/X_matrix[3][1];
}
void DrawCorrespondences(IFS2D img1,IFS2D img2,char filename[])
{
    IFS2D finalImg ((char *)"float",img2.getRows(),img1.getColumns()+img2.getColumns());
    IFS3D final3D ((char *)"float",img2.getRows(),img1.getColumns()+img2.getColumns(),3);

    for(int r=0;r<finalImg.getRows();r++)
    {
        for(int c=0;c<finalImg.getColumns();c++)
        {
            if(c>img1.getColumns())
            {
                finalImg.putPixel(r,c,img2.getPixel(r,c));
                final3D.putPixel(r,c,0,img2.getPixel(r,c));
            }
            else
            {
                finalImg.putPixel(r,c,img1.getPixel(r,c));
                final3D.putPixel(r,c,0,img1.getPixel(r,c));
            }

        }

    }
    std::fstream myfile(filename,std::ios_base::in);
    int a;
    int read=0;
    /* get all the manual correspondences */
    std::vector<int>  AllCorpx;
    std::vector<int>  AllCorpx_;
    std::vector<int>  AllCorpy;
    std::vector<int>  AllCorpy_;
    while (myfile >> a)
    {
        switch(read)
        {
            case 0:
                AllCorpx.push_back(a);
                break;
            case 1:
                AllCorpy.push_back(a);
                break;
            case 2:
                AllCorpx_.push_back(a);
                break;
            case 3:
                AllCorpy_.push_back(a);
                break;
            default:
                break;
        }
        read++;
        read=read%4;
    }
    for(int index=0;index<AllCorpx.size();index++)
    {
        cout<<"\n"<<AllCorpx[index]<<" "<<AllCorpy[index]<<" "<<AllCorpx_[index]<<" "<<AllCorpy_[index]+img1.getColumns();
        Line(finalImg,AllCorpx_[index],AllCorpy_[index],AllCorpx[index],AllCorpy[index]+img1.getColumns());
        Line3D(final3D,AllCorpx_[index],AllCorpy_[index],AllCorpx[index],AllCorpy[index]+img1.getColumns());
        cout<<".";
    }
    finalImg.WritetoDisk((char *)"./../output/final.ifs");
    final3D.WritetoDisk((char *)"./../output/final.ifs");
}
void FindSIFTCorrespondences(IFS2D img1,IFS2D img2,std::vector<long>& img1_IntPt,std::vector<long>& img2_IntPt)
{
    ofstream outputFile("./../output/BookInput.txt");
    IFS2D img1Grad (img1);
    IFS2D img2Grad (img2);
    img1.GradientDirection(1);
    img2.GradientDirection(2);

    int img1_NumPt=img1_IntPt.size();/* Number of INterest points in Image 1*/
    int img2_NumPt=img2_IntPt.size();/* NUmber of INterest points in Image 2 */
    imgVector img1Vector(img1_NumPt,Sector16(16,IntPoints8(8,0)));
    imgVector img2Vector(img1_NumPt,Sector16(16,IntPoints8(8,0)));

    /* Get the feaure vector for img 1*/ 
    cout<<"\nGetting Feature Vectors of Img1";
    for(int index=0 ;index != img1_NumPt ; index++)
    {
        int img_cols = img1.getColumns();
        int rowInd = img1_IntPt.at( index ) / img_cols;
        int colInd = img1_IntPt.at( index ) % img_cols;
        getFeatureVector(img1Grad,rowInd,colInd,img1Vector,index);
    }
    cout<<"\nGetting Feature Vectors of Img2";
    for(int index=0 ;index != img2_NumPt ; index++)
    {
        int img_cols = img2.getColumns();
        int rowInd = img2_IntPt.at( index ) / img_cols;
        int colInd = img2_IntPt.at( index ) % img_cols;
        getFeatureVector(img1Grad,rowInd,colInd,img2Vector,index);
    }
    cout<<"\nFinding Best Match";
    std::vector<int> img1MatchPt(img1_NumPt,-1);
    std::vector<int> img2MatchPt(img2_NumPt,-1);
    int countPts=0;
    int img1Cols = img1.getColumns();
    int img2Cols = img2.getColumns();
    for(int img1Index=0 ;img1Index != img1_NumPt ; img1Index++)
    {
        int img1Row = img1_IntPt[img1Index] / img1Cols;
        int img1Col = img1_IntPt[img1Index] % img1Cols;
        int minerror=PercentThreshold;
        int minerrorIndex=-1;
        for(int img2Index=0 ;img2Index != img2_NumPt ; img2Index++)
        {
            int error=0;
            int img2Row = img2_IntPt[img2Index] / img2Cols;
            int img2Col = img2_IntPt[img2Index] % img2Cols;
            if(img2MatchPt[img2Index]== -1 && (abs(img1Row-img2Row) < Diffthreshold) && (img1Col > (img1Cols/2)) && (img2Col < (img2Cols/2)))
            {
                for(int Rindex=0;Rindex<16;Rindex++)
                {
                    for(int Cindex=0;Cindex<8;Cindex++)
                    {
                        int img1Val=img1Vector[img1Index][Rindex][Cindex];
                        int img2Val=img2Vector[img2Index][Rindex][Cindex];
                        error+= abs((int)sqrt((img1Val*img1Val)-(img2Val*img2Val)));
                    }
                }
                if(minerror>error)
                {
                    minerrorIndex=img2Index;
                    minerror=error;
                }
            }
        }
        if(minerrorIndex != -1)
        {
            int img2Row = img2_IntPt[minerrorIndex] / img2Cols;
            int img2Col = img2_IntPt[minerrorIndex] % img2Cols;
            img1MatchPt[img1Index] = minerrorIndex;
            img2MatchPt[minerrorIndex] = img1Index;
            cout<<"\n"<<img2Row<<"\t"<<img2Col<<"\t"<<img1Row<<"\t"<<img1Col;
            outputFile<<"\n"<<img2Row<<"\t"<<img2Col<<"\t"<<img1Row<<"\t"<<img1Col;
            //cout<<" "<<minerror<<" "<<minerrorIndex;;
            countPts++;
        }
    }
    cout<<"\n$$$"<<countPts;

}
void DrawLine(IFS2D img,int x0,int y0,int x1,int y1)
{
    float deltax = x1 - x0;
    float deltay = y1 - y0;
    float error = -1.0;
    float deltaerr = fabs(deltay / deltax);    // Assume deltax != 0 (line is not vertical),
    // note that this division needs to be done in a way that preserves the fractional part
    int y = y0;
    for(int x=x0;x<(x1-1);x++)
    {
        img.putPixel(x,y,img.getMax()+50);
        error = error + deltaerr;
        if(error >= 0.0)
        {
            y = y + 1;
            error = error - 1.0;
        }
    }
}
void Line3D( IFS3D &img,float x1, float y1, float x2,  float y2)
{
    //Bresenham's line algorithm
    const bool steep = (fabs(y2 - y1) > fabs(x2 - x1));
    if(steep)
    {
        std::swap(x1, y1);
        std::swap(x2, y2);
    }

    if(x1 > x2)
    {
        std::swap(x1, x2);
        std::swap(y1, y2);
    }

    const float dx = x2 - x1;
    const float dy = fabs(y2 - y1);

    float error = dx / 2.0f;
    const int ystep = (y1 < y2) ? 1 : -1;
    int y = (int)y1;
    const int maxX = (int)x2;
    for(int x=(int)x1; x<maxX; x++)
    {
        if(steep)
        {
            img.putPixel(y,x,1,255);
        }
        else
        {
            img.putPixel(x,y,1,255);
        }

        error -= dy;
        if(error < 0)
        {
            y += ystep;
            error += dx;
        }
    }
}
void Line( IFS2D &img,float x1, float y1, float x2,  float y2)
{
    //Bresenham's line algorithm
    const bool steep = (fabs(y2 - y1) > fabs(x2 - x1));
    if(steep)
    {
        std::swap(x1, y1);
        std::swap(x2, y2);
    }

    if(x1 > x2)
    {
        std::swap(x1, x2);
        std::swap(y1, y2);
    }

    const float dx = x2 - x1;
    const float dy = fabs(y2 - y1);

    float error = dx / 2.0f;
    const int ystep = (y1 < y2) ? 1 : -1;
    int y = (int)y1;
    const int maxX = (int)x2;
    float **data=img.getDataPtr();
    for(int x=(int)x1; x<maxX; x++)
    {
        if(steep)
        {
            data[y][x]=255;
        }
        else
        {
            data[x][y]=255;
        }

        error -= dy;
        if(error < 0)
        {
            y += ystep;
            error += dx;
        }
    }
}
void getFeatureVector(IFS2D Gradimg,int rowInd,int colInd,imgVector& IMGVector,int index)
{

    /* Get the feaure vector for img 1*/ 
    float neighbor16x16[16][16];
    int img_cols = Gradimg.getColumns();
    int img_rows = Gradimg.getRows();
    float **dataptr= Gradimg.getDataPtr();
    int neighborrow=0;
    float DominantDirection=0.0;
    float DominantDirectioncount=0.0;
    std::vector<float> histogramVal;
    std::vector<float> histogramCount;
    /* For each point  get the 16x16 */
    std::vector<float> Directionlist;
    for(int row = rowInd-8 ; row != rowInd+8 ; row++)
    {
        int neighborcol=0;
        for(int col = colInd-8 ; col != colInd+8 ; col++)
        {
            if(row >= 0 && row < img_rows && col >= 0 && col < img_cols)
            {
                neighbor16x16[neighborrow][neighborcol]=dataptr[row][col];
                Directionlist.push_back(neighbor16x16[neighborrow][neighborcol]);
            }
            else
            {
                neighbor16x16[neighborrow][neighborcol]=0;
            }
            neighborcol++;
        }
        neighborrow++;
    }/* for get gthe nneighbor */
    std::sort (Directionlist.begin(),Directionlist.end());
    /* For each point  Compute the histogram*/
    for(int listind=0;listind < (int)Directionlist.size();listind++)
    {
        int count=0;
        int flag=0;
        while(count < (int)histogramCount.size())
        {
            if(histogramVal.at(count) == Directionlist.at(listind))
            {
                flag=count;
                break;
            }
            else
            {
                count++;
            }
        }
        if(flag == 0)
        {
            histogramVal.push_back(Directionlist.at(listind));
            histogramCount.push_back(1);
        }
        else
        {
            histogramCount.back()=histogramCount.back()+1;
        }
    }/* get the histogram */

    /* Find the dominant Direction */
    for(int listind=0;listind < (int)histogramCount.size();listind++)
    {
        if(DominantDirectioncount < histogramCount.at(listind))
        {
            DominantDirection = histogramVal.at(listind);
            DominantDirectioncount = histogramCount.at(listind);
        }
    }
    /* ind the direction relative to dominant direction */
    for(int row=0;row < neighborrow ; row++)
    {
        for(int col=0;col < neighborrow ; col++)
        {
            neighbor16x16[row][col]=neighbor16x16[row][col]-DominantDirection;
        }
    }
    /* quantize the direction into 8 direcction */
    for(int row=0;row < neighborrow ; row++)
    {
        for(int col=0;col < neighborrow ; col++)
        {
            float degrees=neighbor16x16[row][col];
            if(degrees < 0)
            {
                degrees=degrees+360;
            }    
            if(degrees <= 22.5)
            {
                degrees=0;
            }           
            else if(degrees <= 67.5 && degrees > 22.5)
            {
                degrees=1;
            }           
            else if(degrees <= 112.5 && degrees > 67.5)
            {
                degrees=2;
            }           
            else if(degrees <= 157.5 && degrees > 112.5)
            {
                degrees=3;
            }           
            else if(degrees <= 202.5 && degrees > 157.5)
            {
                degrees=4;
            }           
            else if(degrees <= 247.5 && degrees > 202.5)
            {
                degrees=5;
            }           
            else if(degrees <= 292.5 && degrees > 247.5)
            {
                degrees=6;
            }           
            else if(degrees <= 337.5 && degrees > 292.5)
            {
                degrees=7;
            }           
            if(degrees > 337.5)
            {
                degrees=0;
            }
            neighbor16x16[row][col]=degrees;
        }
    }
    /* compute histogram for each 4x4 */
    int vectorCount=0;
    for(int row=0;row<4;row++)
    {
        for(int col=0;col<4;col++)
        {
            int val=(int)neighbor16x16[row][col];
            IMGVector[index][vectorCount][val]++;

        }
        for(int col=4;col<8;col++)
        {
            int val=(int)neighbor16x16[row][col];
            IMGVector[index][vectorCount+1][val]++;
        }
        for(int col=8;col<12;col++)
        {
            int val=(int)neighbor16x16[row][col];
            IMGVector[index][vectorCount+2][val]++;
        }
        for(int col=12;col<16;col++)
        {
            int val=(int)neighbor16x16[row][col];
            IMGVector[index][vectorCount+3][val]++;
        }
    }
    vectorCount=vectorCount+4;

    for(int row=4;row<8;row++)
    {
        for(int col=0;col<4;col++)
        {
            int val=(int)neighbor16x16[row][col];
            IMGVector[index][vectorCount][val]++;
        }
        for(int col=4;col<8;col++)
        {
            int val=(int)neighbor16x16[row][col];
            IMGVector[index][vectorCount+1][val]++;
        }
        for(int col=8;col<12;col++)
        {
            int val=(int)neighbor16x16[row][col];
            IMGVector[index][vectorCount+2][val]++;
        }
        for(int col=12;col<16;col++)
        {
            int val=(int)neighbor16x16[row][col];
            IMGVector[index][vectorCount+3][val]++;
        }

    }
    vectorCount=vectorCount+4;

    for(int row=8;row<12;row++)
    {
        for(int col=0;col<4;col++)
        {
            int val=(int)neighbor16x16[row][col];
            IMGVector[index][vectorCount][val]++;
        }
        for(int col=4;col<8;col++)
        {
            int val=(int)neighbor16x16[row][col];
            IMGVector[index][vectorCount+1][val]++;
        }
        for(int col=8;col<12;col++)
        {
            int val=(int)neighbor16x16[row][col];
            IMGVector[index][vectorCount+2][val]++;
        }
        for(int col=12;col<16;col++)
        {
            int val=(int)neighbor16x16[row][col];
            IMGVector[index][vectorCount+3][val]++;
        }

    }

    vectorCount=vectorCount+4;
    for(int row=12;row<16;row++)
    {
        for(int col=0;col<4;col++)
        {
            int val=(int)neighbor16x16[row][col];
            IMGVector[index][vectorCount][val]++;
        }
        for(int col=4;col<8;col++)
        {
            int val=(int)neighbor16x16[row][col];
            IMGVector[index][vectorCount+1][val]++;
        }
        for(int col=8;col<12;col++)
        {
            int val=(int)neighbor16x16[row][col];
            IMGVector[index][vectorCount+2][val]++;
        }
        for(int col=12;col<16;col++)
        {
            int val=(int)neighbor16x16[row][col];
            IMGVector[index][vectorCount+3][val]++;
        }

    }
}
void FindTemplateCorrespondences(IFS2D img1,IFS2D img2,std::vector<long>& img1_IntPt,std::vector<long>& img2_IntPt)
{

    ofstream outputFile("./../output/BookInput.txt");
    int img1_NumPt=img1_IntPt.size();/* Number of INterest points in Image 1*/
    int img2_NumPt=img2_IntPt.size();/* NUmber of  INterest points in IMaged 2 */

    //
    /* For each point in image 1 find the best match in Image 2 */
    float maxever=0.0;
    float img1_neighbor[NeiSiz*2][NeiSiz*2];
    float img2_neighbor[NeiSiz*2][NeiSiz*2];
    std::vector<int> matched;
    for(int img2_index=0 ; img2_index != img2_NumPt ; img2_index++)
    {
        matched.push_back(0);
    }
    int maxcount=0;
    for(int img1_index=0 ;img1_index != img1_NumPt ; img1_index++)
    {
        int img1_cols = img1.getColumns();
        int img1_rowInd = img1_IntPt.at( img1_index ) / img1_cols;
        int img1_colInd = img1_IntPt.at( img1_index ) % img1_cols;
        GetNeighbor(img1,img1_neighbor,img1_rowInd,img1_colInd);
        /*float maxPercent=0;
          int maxPercentRow=0;
          int maxPercentCol=0;
          int maxPercentInd=0;
          int maxPercentnewRow=0;
          int maxPercentnewCol=0;
          int maxPercentnewInd=0;*/
        float minSum=FLT_MAX;
        int minSumRow=0;
        int minSumCol=0;
        int minSumInd=0;
        int minSumnewRow=0;
        int minSumnewCol=0;
        int count=0;
        int img2_cols = img2.getColumns();
        int img2_rows = img2.getRows();
        float **dataptr=img2.getDataPtr();
        for(int img2_index=0 ; img2_index != img2_NumPt ; img2_index++)
        {
            int img2_rowInd = 0;
            int img2_colInd = 0;
            img2_rowInd = img2_IntPt.at( img2_index ) / img2_cols;
            img2_colInd = img2_IntPt.at( img2_index ) % img2_cols;
            if(matched.at(img2_index) == 0 && abs(img2_rowInd-img1_rowInd) < 10 && img1_colInd > img1_cols/2 && img2_colInd < img2_cols/2)
            {
                count++;
                for(int newRowInd = img2_rowInd-ErrCheck;newRowInd != img2_rowInd+ErrCheck ; newRowInd++)
                {
                    for(int newColInd = img2_colInd-ErrCheck;newColInd != img2_colInd+ErrCheck ; newColInd++)
                    {
                        /***************************************************************************************/
                        int NeighborRow=0;
                        for(int row = newRowInd-NeiSiz ; row != newRowInd + NeiSiz ; row++)
                        {
                            int NeighborCol=0;
                            for(int col = newColInd-NeiSiz ; col != newColInd+NeiSiz ; col++)
                            {
                                if(row >= 0 && row <(int)img2_rows && col >= 0 && col < (int)img2_cols)
                                {
                                    img2_neighbor[NeighborRow][NeighborCol]=dataptr[row][col];
                                }
                                else
                                {
                                    img2_neighbor[NeighborRow][NeighborCol]=-1.0;
                                }
                                NeighborCol++;
                            }
                            NeighborRow++;
                        }
                        /******************************************************************************************************/
                        float sum=CompareNeighborSum(img1_neighbor,img2_neighbor);
                        if(minSum > sum)
                        {
                            minSum=sum;
                            minSumnewRow=newRowInd;
                            minSumnewCol=newColInd;
                            minSumRow=img2_rowInd;
                            minSumCol=img2_colInd;
                            minSumInd=img2_index;
                        }
                        /*if(maxPercent < percent)
                          {
                          maxPercent=percent;
                          maxPercentnewRow=newRowInd;
                          maxPercentnewCol=newColInd;
                          maxPercentnewInd=img2_index;
                          maxPercentRow=img2_rowInd;
                          maxPercentCol=img2_colInd;
                          maxPercentInd=img2_index;
                          }*/


                    }
                }
            }
        }
        /*if(maxever < maxpercent)
          {
          maxever=maxPercent;
          }*/
        /*if(maxPercent > PercentThreshold)
          {
          maxcount++;
          cout<<"\n index:"<<img1_index<<"/"<<img1_NumPt;
          cout<<"\tIMG1 "<<img1_rowInd<<",:"<<img1_colInd;
          cout<<"\tIMG2 "<<maxPercentnewRow<<",:"<<maxPercentnewCol;
          cout<<"\tIMG2 "<<maxPercentRow<<",:"<<maxPercentCol;
          cout<<"\tMatch :"<<(100*maxPercent)<<"% max:"<<maxever<<"\tcount :"<<count<<"$$$"<<maxcount<<"\n";
        //cout<<img1_rowInd<<" "<<img1_colInd<<" "<<maxPercentRow<<" "<<maxPercentCol<<"\n";

        // outputFile<<maxSumnewRow<<" "<<maxSumnewCol<<" "<<img1_rowInd<<" "<<img1_colInd<<"\n";
         *    matched.at(maxSumInd)=0;
         img1.putPixel(img1_rowInd,img1_colInd,255);
         img2.putPixel(maxSumnewRow,maxSumnewCol,255);
         }*/
        if(minSum < PercentThreshold*10000)
        {
            maxcount++;

            cout<<"\n index:"<<img1_index<<"/"<<img1_NumPt;
            cout<<"\tIMG1 "<<img1_rowInd<<",:"<<img1_colInd;
            cout<<"\tIMG2 "<<minSumnewRow<<",:"<<minSumnewCol;
            cout<<"\tIMG2 "<<minSumRow<<",:"<<minSumCol;
            cout<<"\tMatch :"<<(100*minSum)<<"% max:"<<maxever<<"\tcount :"<<count<<"$$$"<<maxcount<<"\n";
            //cout<<img1_rowInd<<" "<<img1_colInd<<" "<<minSumRow<<" "<<minSumCol<<"\n";

            outputFile<<minSumnewRow<<" "<<minSumnewCol<<" "<<img1_rowInd<<" "<<img1_colInd<<"\n";
            matched.at(minSumInd)=0;
            img1.putPixel(img1_rowInd,img1_colInd,255);
            img2.putPixel(minSumnewRow,minSumnewCol,255);
        }
        //cout << '\a';
        //X
        cout<<minSum<<"\n";
        minSum=FLT_MAX;

        cout<<".";

    }
    img1.WritetoDisk((char *)"./../output/output1.ifs");
    img2.WritetoDisk((char *)"./../output/output2.ifs");
}

float CompareNeighborSum(float neighbor1[NeiSiz*2][NeiSiz*2],float neighbor2[NeiSiz*2][NeiSiz*2])
{
    float sum=0;
    for(int row=0 ; row != NeiSiz*2 ; row++)
    {
        for(int col = 0 ; col != NeiSiz*2 ; col++)
        {
            if(neighbor1[row][col] != -1.0 && neighbor2[row][col] != -1.0)
            {
                float val=neighbor1[row][col] - neighbor2[row][col];
                sum+=val*val;
            }

        }
    }
    return sum;


}
float CompareNeighborPercent(float neighbor1[NeiSiz*2][NeiSiz*2],float neighbor2[NeiSiz*2][NeiSiz*2])
{
    float totalCount=0;
    float matches=0;
    for(int row=0 ; row != NeiSiz*2 ; row++)
    {
        for(int col = 0 ; col != NeiSiz*2 ; col++)
        {
            if(neighbor1[row][col] != -1.0 && neighbor2[row][col] != -1.0)
            {
                totalCount++;
                if(fabs(neighbor1[row][col] - neighbor2[row][col]) < Diffthreshold)
                {
                    matches++;
                }
            }

        }
    }
    return matches/totalCount;


}
inline void GetNeighbor(IFS2D src_img,float neighbor[NeiSiz*2][NeiSiz*2],int rowInd,int colInd)
{
    int NeighborRow=0;
    float **dataptr=src_img.getDataPtr();
    for(int row=rowInd-NeiSiz;row != rowInd+NeiSiz;row++)
    {
        int NeighborCol=0;
        for(int col=colInd-NeiSiz;col != colInd+NeiSiz;col++)
        {
            if(row >= 0 && row <(int)src_img.getRows() && col >= 0 && col < (int)src_img.getColumns())
            {
                //neighbor[NeighborRow][NeighborCol]=src_img.getPixel(row,col); 
                neighbor[NeighborRow][NeighborCol]=dataptr[row][col];
            }
            else
            {
                neighbor[NeighborRow][NeighborCol]=-1.0;
            }
            // cout<<"\nrowInd :"<<rowInd<<"\tcolInd:"<<colInd<<"\trow:"<<row<<"\tcol:"<<col;
            // pause(0);
            NeighborCol++;
        }
        NeighborRow++;
    }
}


void FindIntPt(std::vector<long>& Pts,IFS2D src_img,int filecount)
{
    IFS2D Scaledsrc (src_img);
    IFS2D Output ((char *)"float",src_img.getRows(),src_img.getColumns());
    std::vector<IFS2D> ScaleSpace;
    std::vector<IFS2D> DoG;

    int index=0;
    for(int i=0;i<5 && Scaledsrc.getRows()>25;i++)
    {
        ScaleSpace.push_back(IFS2D(Scaledsrc));
        ScaleSpace[i].Gauss(Sigma[i%5]);
        if(i%5 != 0)
        {
            DoG.push_back(ScaleSpace[i]);
            DoG[index].subtract(ScaleSpace[i-1]);
            char *path=(char *)ntos(i,"DoG").c_str();
            //DoG[index].threshold(DoG[index].getMax());
            std::cout<<"\nDOG "<<index<<" "<<Sigma[i%5]<<"\t"<<DoG[index].getMax()<<"  "<<DoG[index].getRows();
            DoG[index].WritetoDisk(path);
            index++;

        }
        if(i!=0 && ((i-4)%5) == 0)
        {
            Scaledsrc.SubSample();
        }
        //char *pathi=(char *)ntos(i,"Scale").c_str();
        //ScaleSpace[i].WritetoDisk(pathi);
    }
    std::vector<int> LocalMinRow;
    std::vector<int> LocalMinCol;
    std::vector<int> LocalMinScale;
    int count=0;
    for(int scale=1;scale<=2;scale++)
    {
        for(int r=1;r<DoG[0].getRows()-2;r++)
        {
            for(int c=1;c<DoG[0].getColumns()-2;c++)
            {
                float HigherNeighbor[9];
                float CurrentNeighbor[9];
                float LowerNeighbor[9];
                std::vector<float> VOXEL; 
                // get the neighborhood in the higher scale 
                DoG[scale-1].getNeighborhood(r,c,HigherNeighbor);
                // get the neighborhood in the current scale
                DoG[scale].getNeighborhood(r,c,CurrentNeighbor);
                // get the neighborhood in the lower scale 
                DoG[scale+1].getNeighborhood(r,c,LowerNeighbor);

                float CurVal=DoG[scale].getPixel(r,c);
                for(int index=0;index<9;index++)
                {
                    VOXEL.push_back(HigherNeighbor[index]);
                    if(index!=4) VOXEL.push_back(CurrentNeighbor[index]);
                    VOXEL.push_back(LowerNeighbor[index]);

                }
                int minflag=0;
                int maxflag=0;
                for(int index=0;index!=(int)VOXEL.size();index++)
                {
                    if(VOXEL.at(index)<CurVal)
                    {
                        maxflag++;
                    }
                    if(VOXEL.at(index)>CurVal)
                    {
                        minflag++;
                    }
                }
                if((maxflag==26 || minflag==26 ) && CurVal > threshold)
                {
                    //cout<<"*";
                    Output.putPixel(r,c,Output.getPixel(r,c)+10);
                    if(Output.getPixel(r,c) == 20)
                        count++;
                }
            }

        }

    }
    char *path=(char *)ntos(filecount,"POI_").c_str();
    Output.WritetoDisk(path);
    int pix=0;
    int cols=Output.getColumns();
    for(int r=0;r<Output.getRows();r++)
    {
        for(int c=0;c<Output.getColumns();c++)
        {
            if(Output.getPixel(r,c)>0)
            {
                Pts.push_back((cols*r)+c);
                //cout<<"\n r:"<<r<<" c:"<<c<<" val:"<<(cols*r)+c<<" cols:"<<cols;
                pix++;
            }
        }
    }
    //cout<<"####"<<pix<<"####";
}
