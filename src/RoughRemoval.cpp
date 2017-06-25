//
// Created by 卢颖 on 2017/5/21.
//

#include <iostream>
#include <fstream>
#include <highgui.h>
#include <cv.h>
#include "RoughRemoval.h"

using namespace cv;

RoughRemoval::RoughRemoval() {}
RoughRemoval::~RoughRemoval() {}


enum ConvolutionType
{
    /* Return the full convolution, including border */
            CONVOLUTION_FULL,
    /* Return only the part that corresponds to the original image */
            CONVOLUTION_SAME,
    /* Return only the submatrix containing elements that were not influenced by the border */
            CONVOLUTION_VALID
};

void RoughRemoval::conv2(const cv::Mat &img, const cv::Mat& kernel, ConvolutionType type, Mat& dest)
{
    Mat source = img;
    if(CONVOLUTION_FULL == type)
    {
        source = Mat();
        const int additionalRows = kernel.rows-1, additionalCols = kernel.cols-1;
        copyMakeBorder(img, source, (additionalRows+1)/2, additionalRows/2, (additionalCols+1)/2, additionalCols/2, BORDER_CONSTANT, Scalar(0));
    }

    Point anchor(kernel.cols - kernel.cols/2 - 1, kernel.rows - kernel.rows/2 - 1);
    int borderMode = BORDER_CONSTANT;

    //将kernel转置？
    cv::Mat f_kernel;
    flip(kernel, f_kernel, -1);
    filter2D(source, dest, img.depth(), f_kernel, anchor, 0, borderMode);

    if(CONVOLUTION_VALID == type)
    {
        dest = dest.colRange((kernel.cols-1)/2, dest.cols - kernel.cols/2)
                .rowRange((kernel.rows-1)/2, dest.rows - kernel.rows/2);
    }
}



cv::Mat RoughRemoval::removeShadows(cv::Mat img, cv::Mat mask) {

    //function result = shadowremoval(image, type, mask)

   // s_im = cv::size(image);



    // structuring element for the shadow mask buring, and the shadow/light


    int strel_array[5][5] = {{0, 1, 1, 1, 0},{ 1, 1, 1, 1, 1}, {1, 1, 1, 1, 1}, {1, 1, 1, 1, 1},{0, 1, 1, 1, 0}};
    cv::Mat strel = Mat(5, 5, CV_8U, strel_array);

    cv::Mat ones;
    ones.create(mask.rows, mask.cols, CV_8U);
    //ones = Mat::ones(mask.rows, mask.cols, CV_8U);
    ones = 255;

    // computing shadow/light  core (pixels not on the blured adge of the shadow area)


    cv::Mat shadow_core;
    cv::erode(mask, shadow_core, strel);

    //std::cout<<shadow_core<<std::endl;

    cv::Mat lit_core;
   // cv::Mat temp;

    cv::erode(ones - mask, lit_core, strel);

    Mat img2;
    lit_core.convertTo(img2, CV_32FC1); // or CV_32F works (too)




    std::ofstream fout("DEBUT.txt");

    for (int i = 0; i < lit_core.rows; i++) {
        for (int j = 0; j < lit_core.cols; j++) {
            fout << shadow_core.at<float>(i,j)<<" ";
        }
        fout<<std::endl;
    }




    //  smoothing the mask


    cv::Mat smoothmask;


    conv2(mask, (1.0 / 21) * strel , CONVOLUTION_SAME, smoothmask);





    float sum_shadow_core, sum_lit_core;
    sum_shadow_core = 0; sum_lit_core = 0;

    double shadowavg_red, shadowavg_green, shadowavg_blue;
    shadowavg_blue = 0;shadowavg_green = 0;shadowavg_red = 0;

    double litavg_red, litavg_green, litavg_blue;
    litavg_blue = 0; litavg_green = 0; litavg_red = 0;

    for (int i = 0; i < shadow_core.rows; i++)
        for (int j =0 ; j< shadow_core.cols; j++){
            sum_shadow_core += shadow_core.at<uchar>(i, j);
            shadowavg_blue += img.at<Vec3b>(i,j)[0] * (shadow_core.at<uchar>(i, j))  * 1.0 ;
            shadowavg_green += img.at<Vec3b>(i,j)[1] * (shadow_core.at<uchar>(i, j)) * 1.0 ;
            shadowavg_red += img.at<Vec3b>(i,j)[2] * shadow_core.at<uchar>(i, j) * 1.0 ;
        }
    shadowavg_blue /= sum_shadow_core;
    shadowavg_green /= sum_shadow_core;
    shadowavg_red /= sum_shadow_core;

   // std::cout<<shadowavg_blue<<std::endl;




    //std::cout<<lit_core.rows<<" "<<lit_core.cols<<std::endl;
    for (int i = 0; i < lit_core.rows; i++)
        for (int j =0 ; j< lit_core.cols; j++){
            sum_lit_core += lit_core.at<uchar>(i, j);
            litavg_blue += img.at<Vec3b>(i,j)[0] * lit_core.at<uchar>(i, j) * 1.0 ;
            litavg_green += img.at<Vec3b>(i,j)[1] * lit_core.at<uchar>(i, j) * 1.0 ;
            litavg_red += img.at<Vec3b>(i,j)[2] * lit_core.at<uchar>(i, j) * 1.0 ;
        }


    litavg_blue /= sum_lit_core;
    litavg_green /= sum_lit_core;
    litavg_red /= sum_lit_core;


    //% averaging pixel intensities in the shadow/lit areas
    /*
    float shadowavg_red = sum(sum(image(:,:,1).*shadow_core)) / sum(sum(shadow_core));
    float shadowavg_green = sum(sum(image(:,:,2).*shadow_core)) / sum(sum(shadow_core));
    float shadowavg_blue = sum(sum(image(:,:,3).*shadow_core)) / sum(sum(shadow_core));
    */

    /*float litavg_red = sum(sum(image(:,:,1).*lit_core)) / sum(sum(lit_core));
    float litavg_green = sum(sum(image(:,:,2).*lit_core)) / sum(sum(lit_core));
    float litavg_blue = sum(sum(image(:,:,3).*lit_core)) / sum(sum(lit_core));
    */


    cv::Mat result;
    result = img;

    int removalType = 0;
    if (removalType == 0){
        //% additive shadow removal
        //% compiting colour difference between the shadow/lit areas

        float diff_red = litavg_red - shadowavg_red;
        float diff_green = litavg_green - shadowavg_green;
        float diff_blue = litavg_blue - shadowavg_blue;

        std::cout<<diff_red<<" "<<diff_green<<" "<<diff_blue<<std::endl;
        std::cout<<"11111"<<std::endl;
        for (int i = 0; i < lit_core.rows; i++)
            for (int j =0 ; j < lit_core.cols; j++){
                //% adding the difference to the shadow pixels
                result.at<Vec3b>(i,j)[0] +=  smoothmask.at<Vec3b>(i,j)[0] * diff_blue;
                result.at<Vec3b>(i,j)[1] +=  smoothmask.at<Vec3b>(i,j)[1] * diff_green;
                result.at<Vec3b>(i,j)[2] +=  smoothmask.at<Vec3b>(i,j)[2] * diff_red;
            }

    }

    //std::cout<<smoothmask<< std::endl;


    cv::imshow("removal", result);
    cv::imshow("origin", img);
    cv::waitKey();

    return result;

}
