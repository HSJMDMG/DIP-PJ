#include <cstdio>
#include <iostream>
#include <cv.h>
#include <highgui.h>
#include <fstream>


#include "src/RoughRemoval.h"
#include "src/IntensityDetector.h"
#include "src/BayesianRefine.h"
#include "src/HarmonizationRefine.h"


//#define SHADOWDETECTION
//#define SHADOWREMOVAL
#define SHADOWREFINE


#define COUNTTIME

int main() {
    std::cout << "Hello, World!" << std::endl;

    cv::Mat mask, img;
    std::string img_dir, mask_dir, output_dir;

    //img_dir = "../pic/black.jpg";
   // mask_dir = "../pic/black-mask.png";


#ifdef COUNTTIME
    double dur;
    clock_t start,end;

#endif


#ifdef SHADOWDETECTION
    IntensityDetector Detect_IntensityDetector;
    std::cout<<"Begin detection: "<<img_dir<<std::endl;

#ifdef COUNTTIME
    start = clock();
#endif


    mask = cv::Mat(Detect_IntensityDetector.detectShadows(img_dir, mask_dir));

    std::cout<<"Detection Finished."<<std::endl;

#ifdef COUNTTIME
    end = clock();
    dur = (double)(end - start);
    printf("Use Time:%f\n",(dur/CLOCKS_PER_SEC));
#endif
#endif // for detection




#ifdef SHADOWREMOVAL

#ifndef SHADOWDECTION
    img_dir = "../pic/lssd2.jpg";
    mask_dir = "../pic/lssd2.png";
    output_dir = "../pic/lssd2-rough.jpg";

    img = cv::imread(img_dir);
    mask = cv::imread(mask_dir, 0);


#endif



    RoughRemoval Remove_RoughRemover;
    cv::Mat RoughImage;
   // std::cout<<"AAAA"<<std::endl;
    RoughImage = Remove_RoughRemover.removeShadows(img, mask);
    cv::imwrite(output_dir, RoughImage);
#endif










    //阴影去除之后的refine, 需要初步去除阴影以后的图和体现阴影部分的mask


#ifdef SHADOWREFINE
    //img_dir = "../pic-report/test_4_free_nopair_15_12.jpg";
    //mask_dir = "../pic-report/test_4_binary.png";
    //output_dir = "../refine/test_4_binary-refine-rgb.png";

    img_dir = "../img/lssd58_free_nopair_15_12.jpg";
    mask_dir = "../img/lssd58_binary.png";
    output_dir = "../refine/new-lssd58-refine-rgb.png";


    img = cv::imread(img_dir);
    mask = cv::imread(mask_dir, 0);

    std::cout<<"AAA"<<std::endl;

    BayesianRefine Refine_BayesianRemover;
    Refine_BayesianRemover.refine(img, mask, output_dir);





    //检查refine的差异值
    /*

      cv::Mat refine = cv::imread(output_dir);
      std::ofstream tout("test.txt");
      for (int i=0; i< img.rows; i++)
          for (int j =0; j< img.cols; j++)
              tout<<i<<" , "<<j<<" : "<< (int)(img.at<uchar>(i,j) - refine.at<uchar>(i,j)) <<std::endl;

    */

#endif


    return 0;
}