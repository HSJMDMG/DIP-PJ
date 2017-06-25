//
// Created by 卢颖 on 2017/5/22.
//


#include <iostream>
#include <cstdio>
#include <cmath>
#include <cv.h>
#include <cxcore.h>
#include <highgui.h>
#include <iostream>
#include <cstring>


#include "IntensityDetector.h"

using namespace std;
using namespace cv;

class color{
public:
    float y;
    float u;
    float v;
};

IntensityDetector::IntensityDetector() {}
IntensityDetector::~IntensityDetector() {}

IplImage* IntensityDetector:: HisztogramSzethuzas(IplImage* img){
    int x = img->width;
    int y = img->height;
    int step = img->widthStep;
    uchar* data = (uchar*)img->imageData;
    IplImage* NewImg = cvCreateImage( cvSize(img->width, img->height), IPL_DEPTH_8U, 1 );
    uchar* datan = (uchar*)NewImg->imageData;
    int i, j;

    double min=255;
    double max=0;

    for (j = 0; j < y; j++){
        for (i = 0; i < x; i++){
            if(data[j*step+i] > max)
            {
                max = data[j*step+i];
            }
            if(data[j*step+i] < min)
            {
                min = data[j*step+i];
            }
        }
    }

    for (j = 0; j < y; j++){
        for (i = 0; i < x; i++){
            datan[j*step+i] = (int)(255.0/(max - min)*(data[j*step+i])-min);
        }
    }
    return NewImg;


}
IplImage* IntensityDetector:: createHistogram(IplImage* img,bool exceptnull=false,bool tisztitas=false){
    int height = img->height;
    int width = img->width;
    int channels = img->nChannels;
    int step = img->widthStep;
    uchar* data = (uchar*)img->imageData;
    IplImage* hist= cvCreateImage(cvSize(256,200),8,1);
    int h_height = hist->height;
    int h_width = hist->width;
    int h_channels = hist->nChannels;
    int h_step = hist->widthStep;
    uchar* h_data = (uchar*)hist->imageData;

    unsigned int hist_data[256];
    for (int i=0;i<256;i++){hist_data[i]=0;}
    for(int i=0;i<height;i++){
        for(int j=0;j<width;j++){
            hist_data[(int)data[i*step+j]]++;
        }
    }
    int max=0;
    if(exceptnull){
        for (int i=1;i<256;i++){max=hist_data[i]>max?hist_data[i]:max;}
    }else{
        for (int i=0;i<256;i++){max=hist_data[i]>max?hist_data[i]:max;}
    }
    if(tisztitas){
        for (int i=0;(i<256)&&(hist_data[i]!=max);i++){
            hist_data[i]=0;
        }
    }
    for (int i=0;i<256;i++){hist_data[i]=(int)(((float)hist_data[i]/(float)(max))*100);}

    for(int i=0;i<h_height;i++){
        for(int j=0;j<h_width;j++){
            h_data[i*h_step+j]=0;
            if(i>(4*h_height/5)){
                h_data[i*h_step+j]=j;
            }else{
                if(hist_data[j]>=((4*h_height/5)-i)){
                    h_data[i*h_step+j]=200;
                }
            }

        }
    }

    return hist;
}
IplImage* IntensityDetector:: drawLine(IplImage* img,int line){
    int height = img->height;
    int width = img->width;
    int channels = img->nChannels;
    int step = img->widthStep;
    uchar* data = (uchar*)img->imageData;
    IplImage* hist= cvCreateImage(cvSize(width+50,256),8,1);
    int h_height = hist->height;
    int h_width = hist->width;
    int h_channels = hist->nChannels;
    int h_step = hist->widthStep;
    uchar* h_data = (uchar*)hist->imageData;

    for(int i=0;i<h_height;i++){
        for(int j=0;j<h_width;j++){
            if(j>width&&i!=0){
                h_data[i*h_step+j]=((i-1));
            }else{
                if(data[line*step+j*channels]<(i-1)){
                    h_data[i*h_step+j]=200;
                    data[line*step+j*channels]=0;
                }
                else{
                    h_data[i*h_step+j]=0;
                }
            }
        }
    }
    return hist;
}
IplImage* IntensityDetector:: drawLineGroup(IplImage* img,int line){
    int height = img->height;
    int width = img->width;
    int channels = img->nChannels;
    int step = img->widthStep;
    uchar* data = (uchar*)img->imageData;
    IplImage* hist= cvCreateImage(cvSize(width+50,256),8,1);
    int h_height = hist->height;
    int h_width = hist->width;
    int h_channels = hist->nChannels;
    int h_step = hist->widthStep;
    uchar* h_data = (uchar*)hist->imageData;
    int num1,num2;
    double good[256*256];
    num1=0;
    num2=0;
    for(num1=0;num1<256;num1++){
        for(num2=0;num2<256;num2++){
            good[num1*256+num2]=0;
        }
    }
    for(num1=0;num1<256;num1++){
        for(num2=0;num2<256;num2++){
            for(int j=0;j<width;j++){
                if(abs(data[line*step+j*channels]-num1)<abs(data[line*step+j*channels]-num2)){
                    good[num1*256+num2]+=abs(data[line*step+j*channels]-num1);
                }else{
                    good[num1*256+num2]+=abs(data[line*step+j*channels]-num2);
                }

            }
            good[num1*256+num2]/=width;
        }
    }
    double min=good[0];
    int minplace1=0;
    int minplace2=0;
    for(num1=0;num1<256;num1++){
        for(num2=0;num2<256;num2++){
            if(good[num1*256+num2]<min){min=good[num1*256+num2];minplace1=num1;minplace2=num2;}
        }
    }
    printf("%d,%d...",minplace1,minplace2);


    for(int i=0;i<h_height;i++){
        for(int j=0;j<h_width;j++){

            if(j>width&&i!=0){
                h_data[i*h_step+j]=((i-1));
            }
            else{
                h_data[i*h_step+j]=0;
                if(abs(data[line*step+j*channels]-minplace1)<abs(data[line*step+j*channels]-minplace2)){
                    if(minplace1==(i-1)){
                        h_data[i*h_step+j]=200;
                        data[line*step+j*channels]=minplace1;

                    }
                    else{
                        h_data[i*h_step+j]=0;
                    }
                }
                else{
                    if(minplace2==(i-1)){
                        h_data[i*h_step+j]=200;
                        data[line*step+j*channels]=minplace2;

                    }else{
                        h_data[i*h_step+j]=0;
                    }


                }

            }

        }


    }
    return hist;
}
void IntensityDetector:: blur(IplImage* image,int size){

    IplImage* pad=cvCreateImage(cvSize(image->width+size*2,image->height+size*2),8,image->nChannels);
    int height,width, widthpad,heightpad,pstep,  step, channels;
    uchar *data, *pdata;

    height    = image->height;
    width     = image->width;
    step      = image->widthStep;
    pstep =	pad->widthStep;
    channels  = image->nChannels;
    data      = (uchar *)image->imageData;
    heightpad = pad->height;
    widthpad  = pad->width;
    pdata = (uchar*)pad->imageData;

    for(int i=0;i<heightpad;i++){
        for(int j=0;j<widthpad;j++){
            for(int k=0;k<channels;k++){
                if(((i-size)>=0) && ((j-size)>=0)&&((i-size)<height)&&((j-size)<width)){
                    pdata[i*pstep+j*channels+k]=data[(i-size)*step+(j-size)*channels+k];
                }else{
                    pdata[i*pstep+j*channels+k]=255;
                }
            }
        }
    }
    double sum;
    for(int k=0;k<channels;k++){
        for(int i=size;i<(heightpad-size);i++){
            for(int j=size;j<(widthpad-size);j++){
                sum=0.;
                for(int ii=-size/2;ii<size/2+1;ii++){
                    for(int jj=-size/2;jj<size/2+1;jj++){
                        sum+=(double)pdata[(i+ii)*pstep+(j+jj)*channels+k]/(double)(size*size);
                    }
                }


                //std::cout<<"\n"<<sum;
                pdata[i*pstep+j*channels+k]=(int)sum;
            }
        }
    }

    cvSaveImage("o_padded.jpg",pad);
    for(int i=size;i<(heightpad-size);i++){
        for(int j=size;j<(widthpad-size);j++){
            for(int k=0;k<channels;k++){
                data[(i-(size))*step+(j-(size))*channels+k]=pdata[i*pstep+j*channels+k];
            }
        }
    }
    cvSaveImage("o_blured.jpg",image);
    cvReleaseImage(&pad);
    //return image;
}
IplImage* IntensityDetector:: findShadow2(const char* filename, const char* maskname){



    IplImage* original=cvLoadImage(filename,1);
    //cout<<(char*)filename<<endl;
    //cout<<original->nSize<<endl;

    int height,width,step,channels,stepm;
    uchar *data,*data2,*datam,*datag,*datah;



    height    = original->height;
    width     = original->width;
    step      = original->widthStep;
    channels  = original->nChannels;
    data      = (uchar *)original->imageData;




    IplImage* edge=cvCreateImage(cvSize(original->width,original->height),8,1);
    IplImage* gray=cvCreateImage(cvSize(original->width,original->height),8,1);
    IplImage* blured=cvCreateImage(cvSize(original->width,original->height),8,3);
    IplImage* histo =cvCreateImage(cvSize(original->width,original->height),8,1);
    IplImage* mask=cvCreateImage(cvSize(original->width,original->height),8,1);






    cvCopy(original,blured);
    blur(blured,9);
    cvCvtColor(original,gray,CV_BGR2GRAY);
    cvCanny(gray,edge,100,200);
    data      = (uchar *)original->imageData;
    data2      = (uchar *)blured->imageData;
    datah      = (uchar *)histo->imageData;
    datam      = (uchar *)mask->imageData;
    datag      = (uchar *)gray->imageData;
    stepm      = mask->widthStep;
    color* orig = new color[step*height];
    for(int j=0;j<height;j++){
        for(int i=0;i<width;i++){
            datam[j*stepm+i]=0;
        }
    }
    double fullaverage;
    int fullcount;
    int maxa,maxb;
    fullaverage=0.;
    fullcount=0;
    int maxwindow=31;
    double average ;
    int count;
    for(int j=0;j<height;j++){
        for(int i=0;i<width;i++){
            //if(datam[j*stepm+i]==0){
            orig[j*step+i].y=(float)data[j*step+i*channels+0]*0.114+(float)data[j*step+i*channels+1]*0.587+(float)data[j*step+i*channels+2]*0.299;
            orig[j*step+i].u=(float)data[j*step+i*channels+0]*0.436+(float)data[j*step+i*channels+1]*-0.289+(float)data[j*step+i*channels+2]*-0.147;
            orig[j*step+i].v=(float)data[j*step+i*channels+0]*-0.1+(float)data[j*step+i*channels+1]*-0.515+(float)data[j*step+i*channels+2]*0.615;

            datah[j*stepm+i]=orig[j*step+i].y;
            //}
        }
    }

    int a,b;
    IplImage* szethuzott = HisztogramSzethuzas(histo);
    uchar* dataszeth=(uchar*)szethuzott->imageData;
    for(int j=0;j<height;j++){
        for(int i=0;i<width;i++){
            //if(datam[j*stepm+i]==0){
            orig[j*step+i].y=dataszeth[j*stepm+i];
            fullaverage+=orig[j*step+i].y;
            fullcount++;
            //}
        }
    }
    fullaverage/=fullcount;
    //feloszt锟絪
    for(int window=81;window>3;window-=16){
        maxa=maxb=window;
        for(int j=0;j<height;j++){
            for(int i=0;i<width;i++){
                for(a=0;a<maxa;a++){
                    for(b=0;b<maxb;b++){
                        average=0.;
                        count=0;
                    }
                }
                //printf("3");
                for(a=0;a<maxa;a++){
                    for(b=0;b<maxb;b++){

                        if((j+a)<height&&(i+b)<width){
                            if(datam[(j+a)*stepm+(i+b)]==0){
                                average+=orig[(j+a)*step+(i+b)].y;
                                count++;
                            }
                        }

                    }
                }
                average/=count;
                for(a=0;a<maxa;a++){
                    for(b=0;b<maxb;b++){
                        if((j+a)<height&&(i+b)<width){
                            if(fullaverage*0.6>orig[(j+a)*step+(i+b)].y){
                                datam[(j+a)*stepm+(i+b)]=255;
                                data[(j+a)*step+(i+b)*channels+1]=255;
                            }else{
                                if(average*0.6>orig[(j+a)*step+(i+b)].y){
                                    if(datam[(j+a)*stepm+(i+b)]==0){
                                        datam[(j+a)*stepm+(i+b)]=255;
                                        data[(j+a)*step+(i+b)*channels+2]=255;
                                    }
                                }
                            }
                        }
                    }
                }

            }
        }



    }
    cvSaveImage(maskname,mask);
    //cvNamedWindow("edge", CV_WINDOW_AUTOSIZE);
    //cvShowImage("edge",original);
    //cvWaitKey(0);
    //cvDestroyAllWindows();

    return mask;
}







IplImage* IntensityDetector::detectShadows(string img_dir, string mask_dir) {

    return findShadow2(img_dir.c_str(), mask_dir.c_str());
}