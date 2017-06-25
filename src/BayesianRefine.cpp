//
// Created by 卢颖 on 2017/5/20.
//

#include <cv.h>
#include <highgui.h>
#include <string>
#include <iostream>
#include <fstream>
#include <ctime>
#include "BayesianRefine.h"
#include <cmath>

//构造函数和析构函数
BayesianRefine::BayesianRefine() {

}

BayesianRefine::~BayesianRefine() {

}

// refine代码
void BayesianRefine::refine(cv::Mat img, cv::Mat mask, std::string output_dir) {

    //0: black; 255: white


    std::vector<std::pair<int, int> > Shadowpixel;




    //mask导入正确！
    for (int i = 0; i < mask.rows; i++)
        for (int j = 0; j < mask.cols; j++) {

            if (mask.at<uchar>(i, j) > 0) {
                Shadowpixel.push_back(std::make_pair(i, j));

            }

        }




//#define DEBUG

#ifdef  DEBUG
    std::ofstream fout("DEBUG.txt");
#endif



#define  GOCHECK

    std::vector<cv::Mat> channels;
    std::vector<cv::Mat> recoveryChannels;

    //recoveryChannels.clear();
    //channels.clear();

    /*channelMode:
 * 0:BGR
 * 1:HLS
 * 2;LAB
 * */

    int channelMode = 0;

    int lastchannel = 0;
    int _ = 0;
    if (channelMode == 0) {_ = 0; lastchannel = 2;}
    if (channelMode == 1) {cv::cvtColor(img, img, CV_BGR2HLS); _ = 1; lastchannel = 2;}
    if (channelMode == 2) {cv::cvtColor(img, img, CV_BGR2Lab); _ = 0; lastchannel = 0;}


    //cv::imshow("hsl", img);
    //cv::cvtColor(img, img, CV_HLS2BGR);
    //cv::imshow("bgr", img);
    //cv::waitKey();

    cv::split(img, channels);



#ifdef GOCHECK



#define UNHIDDEN

    cv::Mat currentChannel;



    for (int i = 0; i < _; i++) {
        currentChannel = channels.at(i);
        recoveryChannels.push_back(currentChannel);
    }


    for (; _ <= lastchannel; _++) {



        //std::cout<<"AAAAA"<<std::endl;
        currentChannel = channels.at(_);




        //对每个通道上都做这个处理，计算阴影部分移除图像后的像素点值


        /* epsilon: 误差阈值
         * delta: 误差
         * alpha, beta: R分量和阴影的参数
         * Image: 去除阴影以后的像素值(不断更新)
         * */


        float epsilon = 0.01;
        float delta = 1;
        int it_threshold = 3000;


        int Len = Shadowpixel.size();
        float Image[2][Len];
        float alpha[2][Len];
        float beta[2][Len];


        // OpenCV
        //cv::Mat alphaMatrix(img.rows, img.cols, CV_32FC1, cv::Scalar(0));
        //cv::Mat betaMatrix(img.rows, img.cols, CV_32FC1, cv::Scalar(0));
        //cv::Mat imageMatrix;

        float alphaMatrix[img.rows][img.cols];
        float betaMatrix[img.rows][img.cols];
        float imageMatrix[img.rows][img.cols];




#ifdef UNHIDDEN

        for (int i = 0; i < img.rows; i++)
            for (int j = 0; j < img.cols; j++) {
                imageMatrix[i][j] = currentChannel.at<uchar>(i,j);
                alphaMatrix[i][j] = 0;
                betaMatrix[i][j] = 1;
            }






        //currentChannel.convertTo(imageMatrix, CV_32FC1);
        //currentChannel.copyTo(imageMatrix);

        //std::cout<<imageMatrix[100][100]<<std::endl;



        //initialize alpha, beta, image
        srand((unsigned)time(NULL));
        float value;

        for (int i = 0; i < Len; i++) {
            Image[0][i] = currentChannel.at<uchar>(Shadowpixel[i].first, Shadowpixel[i].second);
            Image[1][i] = 0;


           // value = 0; =-= fine tune
            if (channelMode == 0 )
                value =  0 + (100 - (rand() % 200) * 1.0) / 1000 ;
            else
                value = (100 - (rand() % 200) * 1.0) / 1000;

            alpha[0][i] = value;
            alpha[1][i] = 0;
            alphaMatrix[Shadowpixel[i].first][Shadowpixel[i].second] = value;



            value = 1;
            if (channelMode == 0)
                value =  1 + (100 - (rand() % 200) * 1.0) / 1000 ;
            else
                value = 1 + (100 - (rand() % 200) * 1.0) / 1000;

            beta[0][i] = value;
            beta[1][i] = 1;
            betaMatrix[Shadowpixel[i].first][Shadowpixel[i].second] = value;

            Image[1][i] = 0;
            Image[0][i] = currentChannel.at<uchar>(Shadowpixel[i].first, Shadowpixel[i].second) * 1.0 / beta[0][i] + alpha[0][i];

            //fout<<"Img "<<i<<" :"<< Image[0][i]<<std::endl;
        }


        // variables for em
        int now = 0;
        int iteration = 0;

        float a, b, rb, ra, ri, a_mean, b_mean, i_mean, oa, ob, oi, dI, Ih, Is;

        int Neighbour[9][2] = {{-1, -1},
                               {-1, 0},
                               {-1, 1},
                               {1,  -1},
                               {1,  0},
                               {1,  1},
                               {0,  -1},
                               {0,  0},
                               {0,  1}};
        int NeighbourNum = 9;
        float DescentRate = 0.5;


        while (delta > epsilon) {
            iteration++;
            if (iteration > it_threshold) break;
            for (int i = 0; i < Len; i++) {
                int x = Shadowpixel[i].first;
                int y = Shadowpixel[i].second;

                //calculate parameters: ra, rb, a_mean, b_mean, dI

                //calculate a_mean,b_mean,i_mean
                a_mean = 0;
                b_mean = 0;
                i_mean = 0;
                int shadowneighbour = 0;

                for (int j = 0; j < NeighbourNum; j++) {
                    int u, v;
                    u = x + Neighbour[j][0];
                    v = y + Neighbour[j][1];

                    if (0 <= u && u < img.rows && 0 <=v && v < img.cols) {
                        shadowneighbour++;
                        a_mean += alphaMatrix[u][v];
                        b_mean += betaMatrix[u][v];
                        i_mean += imageMatrix[u][v];
                    }
                }

                a_mean = a_mean / shadowneighbour;
                b_mean = b_mean / shadowneighbour;
                i_mean = i_mean / shadowneighbour;

                //fout<<"mean a, b, i: " <<a_mean<<" "<<b_mean<<" "<<i_mean<<std::endl;


                //calculate ra,rb,ri, or square deviation oa, ob, oi
                oa = 0;
                ob = 0;
                oi = 0;
                for (int j = 0; j < NeighbourNum; j++) {
                    int u, v;
                    u = x + Neighbour[j][0];
                    v = y + Neighbour[j][1];

                    if (0 <= u && u < img.rows && 0 <=v && v < img.cols) {
                        // if (mask.at<uchar>(u, v) == 255) {
                        oa += (alphaMatrix[u][v] - a_mean) * (alphaMatrix[u][v] - a_mean);
                        ob += (betaMatrix[u][v] - b_mean) * (betaMatrix[u][v] - b_mean);
                        oi += (imageMatrix[u][v] - i_mean) * (imageMatrix[u][v] - i_mean);
                    }
                }
                oa = oa / shadowneighbour;
                ob = ob / shadowneighbour;
                oi = oi / shadowneighbour;

                //fout<<"o_ a, b, i: " <<oa<<" "<<ob<<" "<<oi<<std::endl;
                //std::cout<<oa<<" "<<ob<<" "<<oi<<std::endl;


                //ra = sqrt(shadowneighbour / oa);
                //rb = sqrt(shadowneighbour / ob);
                //ri = sqrt(shadowneighbour / oi);

                //define Ih, Is, dI, a, b
                Ih = Image[now][i];
                Is = currentChannel.at<uchar>(x, y) * 1.0;
                dI = Is - Ih;
                a = alpha[now][i];
                b = beta[now][i];


                //fout<<"a, b, Is, Ih:  "<<a <<" "<<b<<" "<<" "<<Is<<" "<<Ih<<std::endl;

                double e_zero = 0.00000001;
                double over, below = 0;


                //if (x,y) in U (全影)
                //approximate new_b

                over = (double)(oi * b_mean - ob * (a * Is - Ih * Is));
                below = (double)(oi  - ob * (2 * Ih * a - a * a - Ih * Ih));
                //if ((-e_zero < over) && (over < e_zero))
                //    value = 0;
                //else

                /* value = fmin(1, (oi * b_mean - ob * (a * Is - Ih * Is)) /
                         (oi  - ob * (2 * Is * a - a * a - Ih * Ih)));
*/
                value = over / below;

                beta[1 - now][i] = value;
                //fout<<x<<" "<<y<<" beta: " << over<<"/ "<<below<<std::endl;


                //approximate new_a
                over = (double)(oi * a_mean - oa * (b * Is - Ih * b * b));
                below = (double)(oi + oa * b * b);
                //if ((-e_zero < over) && (over < e_zero))
                //    value = 0;
                //  else
                //alpha max: I(x,y)

                //value = fmin(255, (oi * a_mean - oa * (b * Is - Ih * b * b)) / (oi + oa * b * b));

                value = over / below;

                alpha[1 - now][i] = value;

                //fout<<x<<" "<<y<<" alpha: " << over<<"/ "<<below<<std::endl;

                // alpha[1 - now][i] = (oi * a_mean - oa * (b * Is - Ih * b * b)) / (oi + oa * b * b);


                //calculate new_I
                Image[1 - now][i] = Is * 1.0 / beta[1 - now][i] + alpha[1 - now][i];

                //fout<<x<<" "<<y<<" " << beta[1-now][i]<<" "<<alpha[1-now][i]<<std::endl;


            }
            /*
             * update delta, alpha, beta, image matrix
             * delta = ||alpha[1-now] - alpha[now] + beta[1-now] - beta[now]||;
            */
            delta = 0;

            for (int i = 0; i < Len; i++) {
                int x, y;
                x = Shadowpixel[i].first;
                y = Shadowpixel[i].second;

                delta += (alpha[1 - now][i] - alpha[now][i] + beta[1 - now][i] - beta[now][i]) *
                         (alpha[1 - now][i] - alpha[now][i] + beta[1 - now][i] - beta[now][i]);

                //delta += (alpha[1 - now][i] - alpha[now][i]) * (alpha[1 - now][i] - alpha[now][i])  + (beta[1 - now][i] - beta[now][i]) * (beta[1 - now][i] - beta[now][i]);



                alpha[1 - now][i] = alpha[now][i] - DescentRate * (alpha[now][i] - alpha[1 - now][i]);
                beta[1 - now][i] = beta[now][i] - DescentRate * (beta[now][i] - beta[1 - now][i]);
                Image[1 - now][i] = currentChannel.at<uchar>(x, y) * 1.0 / beta[1 - now][i] + alpha[1 - now][i];

                alphaMatrix[x][y] = alpha[1 - now][i];
                betaMatrix[x][y] = beta[1 - now][i];
                imageMatrix[x][y] = Image[1 - now][i];

            }

            //std::cout<<"Iteration End, delta:" << delta<< std::endl;
            //if (iteration > 3000) break;
            delta = sqrt(delta / 2 * Len);

            now = 1 - now;

            std::cout << "Iteration Times: " << iteration << " " << "delta: " << delta << std::endl;
            //fout << "Iteration Times: " << iteration << " " << "delta: " << delta << std::endl;


        }

        // fout<<"-------------------------------------------------------------------------"<<std::endl;

        //fout<<"Channel "<<_<<"  Finish!"<<std::endl;


        //add recoveried image to recovery channel

        /*
        for (int i = 0; i < Len; i++){
            int x, y;
            x = Shadowpixel[i].first;
            y = Shadowpixel[i].second;

            fout<<"RecoveryChannel("<<_<<","<<x<<","<<y<<") = " << imageMatrix[x][y]<< std::endl;
            fout<<"OriginChannel("<<_<<","<<x<<","<<y<<") = " << currentChannel.at<uchar>(x,y)<< std::endl;
        }

         */


        //fine tune 2
        //float  enlarger = 20;
        float enlarger = 1;




        cv::Mat channelResult(img.rows, img.cols, CV_8UC1);
        for (int i = 0; i < img.rows; i++)
            for (int j = 0; j < img.cols; j++) {
                channelResult.at<uchar>(i,j) = (int)(imageMatrix[i][j] - currentChannel.at<uchar>(i,j)) * enlarger + imageMatrix[i][j];
            }
        if (_ == 0) {
            cv::imshow("0-Channel", channelResult);
        }
        if (_ == 1) {
            cv::imshow("1-Channel", channelResult);
        }
        if (_ == 2) {
            cv::imshow("2-Channel", channelResult);
        }

        recoveryChannels.push_back(channelResult);

#endif
    }


    for (int i = lastchannel + 1 ; i < 3; i++) {
        currentChannel = channels.at(i);
        recoveryChannels.push_back(currentChannel);
    }


    cv::Mat RecoveryImage;
    cv::merge(recoveryChannels, RecoveryImage);




    //std::cout<<RecoveryImage.rows<<std::endl;

//    cv::imwrite(output_dir, RecoveryImage);

    if (channelMode == 1) {cv::cvtColor(img, img, CV_HLS2BGR);cv::cvtColor(RecoveryImage, RecoveryImage, CV_HLS2BGR);}
    if (channelMode == 2) {cv::cvtColor(img, img, CV_Lab2BGR);cv::cvtColor(RecoveryImage, RecoveryImage, CV_Lab2BGR);}



    cv::imshow("removal", RecoveryImage);
    cv::imwrite(output_dir, RecoveryImage);

    cv::imshow("img", img);
    //std::cout<<mask.rows<<std::endl;
    cv::imshow("mask", mask);
    //std::cout<<"MIAOMIAOMIAO"<<std::endl;


    cv::waitKey();


#endif



}

