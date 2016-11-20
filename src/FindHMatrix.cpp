// my own header
#include "../hdr/FindHMatrix.h"


#define offset 100
void printmatrix(float **matrix,int rows ,int col);
void printmatrixd(double **matrix,int rows ,int col);
void getHmatrix(float **Amatrix,float **Dmatrix,float **Hmatrix,int NumPts);
void getDmatrix(float **Dmatrix);
void getAmatrix(float **Amatrix);
void getCorPoint(float **Xmatrix,float **X_matrix,float **Hmatrix);
int getNumPoints(char filename[]);
float CheckCorrespondence(float **Xmatrix,float **X_matrix,float **Hmatrix);
std::vector<int>  Corpx(10,0);
std::vector<int>  Corpx_(10,0);
std::vector<int>  Corpy(10,0);
std::vector<int>  Corpy_(10,0);

void FindHMatrix(char argv[],float **HMatrix)
{
    int numPts=getNumPoints(argv);
    float **Amatrix=matrix(1,20*2,1,8);
    float **Dmatrix=matrix(1,20*2,1,1);
    float **TempHmatrix=matrix(1,3,1,3);
    //float **Hmatrix=matrix(1,3,1,3);
    /* Get all points */
    std::fstream myfile(argv, std::ios_base::in);
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
    int iterations=150;
    int maxCount=0;
    while(iterations !=0)
    {
        for(int PtNum=0;PtNum<10;PtNum++)
        {
            int randPt=rand()%numPts;
            Corpx[PtNum]=AllCorpx[randPt];
            Corpy[PtNum]=AllCorpy[randPt];
            Corpx_[PtNum]=AllCorpx_[randPt];
            Corpy_[PtNum]=AllCorpy_[randPt];
            //std::cout<<"\n"<<Corpx[PtNum]<<"\t"<<Corpy[PtNum]<<"\t"<<Corpx_[PtNum]<<"\t"<<Corpy_[PtNum]<<"\n";
        }
            getAmatrix(Amatrix);
            //printmatrix(Amatrix,20,8);
            getDmatrix(Dmatrix);
            //printmatrix(Dmatrix,20,1);
            getHmatrix(Amatrix,Dmatrix,TempHmatrix,20);
            //printmatrix(TempHmatrix,3,3);
            int count=0;
            for(int index =0;index<Corpx.size();index++)
            {
                float **Xmatrix;
                float **Xdmatrix;
                float **Xddmatrix;
                Xmatrix=matrix(1,3,1,1);
                Xdmatrix=matrix(1,3,1,1);
                Xddmatrix=matrix(1,3,1,1);
                Xmatrix[1][1]=Corpx[index];
                Xmatrix[2][1]=Corpy[index];
                Xmatrix[3][1]=1;
                Xdmatrix[1][1]=Corpx_[index];
                Xdmatrix[2][1]=Corpy_[index];
                Xdmatrix[3][1]=1;
                float val=CheckCorrespondence(Xdmatrix,Xddmatrix,TempHmatrix);
                int xval=abs((int)(Xddmatrix[1][1]-Xmatrix[1][1]));
                int yval=abs((int)(Xddmatrix[2][1]-Xmatrix[2][1]));
                std::cout<<"\n"<<Xmatrix[1][1]<<" "<<Xmatrix[2][1];
                std::cout<<"\n"<<Xdmatrix[1][1]<<" "<<Xdmatrix[2][1];
                std::cout<<"\n"<<Xddmatrix[1][1]<<" "<<Xddmatrix[2][1];
                std::cout<<"\n***************************************";
                std::cout<<"\n"<<val;
                val=xval+yval;
                if(val < 20)
                {
                    count++;
                }
            }
            if(count > maxCount)
            {
                HMatrix[1][1]=TempHmatrix[1][1];
                HMatrix[1][2]=TempHmatrix[1][2];
                HMatrix[1][3]=TempHmatrix[1][3];
                HMatrix[2][1]=TempHmatrix[2][1];
                HMatrix[2][2]=TempHmatrix[2][2];
                HMatrix[2][3]=TempHmatrix[2][3];
                HMatrix[3][1]=TempHmatrix[3][1];
                HMatrix[3][2]=TempHmatrix[3][2];
                HMatrix[3][3]=TempHmatrix[3][3];
                maxCount=count;
            }
            std::cout<<"\n"<<count<<"__"<<iterations;
            iterations--;
    }
    printmatrix(HMatrix,3,3);
}
float CheckCorrespondence(float **Xmatrix,float **X_matrix,float **Hmatrix)
{
    ifsmatmult(Hmatrix,Xmatrix,X_matrix,3,3,3,1); 
    X_matrix[1][1]=X_matrix[1][1]/X_matrix[3][1];
    X_matrix[2][1]=X_matrix[2][1]/X_matrix[3][1];
    X_matrix[3][1]=X_matrix[3][1]/X_matrix[3][1];
    
    return 0;
    
}
void getAmatrix(float **Amatrix)
{
    for(int row_A=0;row_A < (int)Corpx_.size();row_A++)
    {
        int xi=Corpx.at((row_A));
        int yi=Corpy.at((row_A));
        int xi_=Corpx_.at((row_A));
        int yi_=Corpy_.at((row_A));
        int index=row_A*2+1;
        Amatrix[index][1]=0;
        Amatrix[index][2]=0;
        Amatrix[index][3]=0;
        Amatrix[index][4]=-xi;
        Amatrix[index][5]=-yi;
        Amatrix[index][6]=-1;
        Amatrix[index][7]=xi*yi_;
        Amatrix[index][8]=yi*yi_;

        Amatrix[index+1][1]=xi;
        Amatrix[index+1][2]=yi;
        Amatrix[index+1][3]=1;
        Amatrix[index+1][4]=0;
        Amatrix[index+1][5]=0;
        Amatrix[index+1][6]=0;
        Amatrix[index+1][7]=-xi*xi_;
        Amatrix[index+1][8]=-yi*xi_;
    }

}
void getDmatrix(float **Dmatrix)
{
    /* get all the manual correspondences */
    int index=0;
    for(int row_A=0;row_A < (int)Corpx_.size();row_A++)
    {
        Dmatrix[index+1][1]=Corpx_.at(row_A);
        Dmatrix[index][1]=Corpy_.at(row_A);
        index+=2;
    }

}
void getHmatrix(float **Amatrix,float **Dmatrix,float **Hmatrix,int NumPts)
{
    float **AmatrixT,**ATxA,**finvATxA,**iATAxAT,**iATAATxD;
    double **invATxA;
    double **dATxA;
    AmatrixT = matrix(1,8,1,NumPts);
    ATxA=matrix(1,8,1,8);
    dATxA=dmatrix(1,8,1,8);
    invATxA=dmatrix(1,8,1,8);
    finvATxA=matrix(1,8,1,8);
    iATAxAT=matrix(1,8,1,NumPts);
    iATAATxD=matrix(1,8,1,1);

    transpose(Amatrix,NumPts,8,AmatrixT);
    ifsmatmult(AmatrixT,Amatrix,ATxA,8,NumPts,NumPts,8);
    for(int r=1;r<=8;r++)
    {
        for(int c=1;c<=8;c++)
        {
            dATxA[r][c]=ATxA[r][c];     
        }
    }
    ifsinverse(dATxA,invATxA,8);
    for(int r=1;r<=8;r++)
    {
        for(int c=1;c<=8;c++)
        {
            finvATxA[r][c]=(float)invATxA[r][c];
        }
    }
    ifsmatmult(finvATxA,AmatrixT,iATAxAT,8,8,8,NumPts);
    ifsmatmult(iATAxAT,Dmatrix,iATAATxD,8,NumPts,NumPts,1);
    Hmatrix[1][1]=iATAATxD[1][1];
    Hmatrix[1][2]=iATAATxD[2][1];
    Hmatrix[1][3]=iATAATxD[3][1];
    Hmatrix[2][1]=iATAATxD[4][1];
    Hmatrix[2][2]=iATAATxD[5][1];
    Hmatrix[2][3]=iATAATxD[6][1];
    Hmatrix[3][1]=iATAATxD[7][1];
    Hmatrix[3][2]=iATAATxD[8][1];
    Hmatrix[3][3]=1;


}
int getNumPoints(char filename[])
{
    std::fstream myfile(filename, std::ios_base::in);
    int a;
    int read=0;
    int count=0;
    std::vector<int> Corpx;
    /* get all the manual correspondences */
    while (myfile >> a)
    {
        switch(read)
        {
            case 0:
                Corpx.push_back(a);
                break;
            case 1:
                break;
            case 2:
                break;
            case 3:
                break;
            default:
                break;
        }
        read++;
        read=read%4;
    }
    return Corpx.size();
}
void printmatrix(float **matrix,int rows ,int col)
{
    std::cout<<"\n****************************************\n";
    for(int r=0;r<rows;r++)
    {
        std::cout<<r<<"\t";
        for(int c=0;c<col;c++){
            std::cout<<matrix[r+1][c+1]<<"\t";
        }
        std::cout<<"\n";
    }

}
void printmatrixd(double **matrix,int rows ,int col)
{
    std::cout<<"\n****************************************\n";
    for(int r=0;r<rows;r++)
    {
        std::cout<<r<<"\t";
        for(int c=0;c<col;c++){
            std::cout<<matrix[r+1][c+1]<<" ";
        }
        std::cout<<"\n";
    }

}

