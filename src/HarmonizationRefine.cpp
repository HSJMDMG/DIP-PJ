//
// Created by 卢颖 on 2017/5/22.
//

#include "HarmonizationRefine.h"


HarmonizationRefine::HarmonizationRefine() {}
HarmonizationRefine::~HarmonizationRefine() {}

/*
//private functions
cv::Mat HarmonizationRefine::GenerateCorrectionRegion(cv::Mat mask, cv::Mat I, cv::Mat In) {
    //设置修正阈值
    double threshold = 1;
    Mat Region = cv::substract(In - I);

    //3x3 kernel的膨胀操作，填洞
    opencv :: dilation;

    return Region;

}


HarmonizationRefine::refine(cv::Mat I, cv::Mat In, cv::Mat mask) {

    //TODO: transfer all the Mat to LAB color space (CIELAB)

    //找到待修正区域
    std::vector<std::pair<int, int> > Rc = GenerateCorrectionRegion(mask, I, In);

    //用I和In合成Is （参见SIGGRAPH2012 Image Melding， 含matlab代码，此处直接调用）
    //cv::Mat Is = GetImageMelding(mask, I, In); //【生成需要的几个图片 + 调用matlab】
    cv::Mat Is = cv::imread('xxx.jpg');

    //计算参数
    //ComputingCorrectionParameters
    //颜色参数：gL, bA, bB

    //纹理参数：

    //置信度：



    //参数修正
    //CorrectionMapRefinement()
    //颜色参数修正：解线性系统
    //纹理参数修正


    //修正图像, 修正In(原始removal)
    //颜色修正
    //纹理修正

}

*/