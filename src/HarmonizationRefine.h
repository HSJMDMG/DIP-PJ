//
// Created by 卢颖 on 2017/5/22.
//

#ifndef BAYESIANREMOVAL_HARMONIZATIONREFINE_H
#define BAYESIANREMOVAL_HARMONIZATIONREFINE_H


class HarmonizationRefine {
public:
    HarmonizationRefine();
    virtual ~HarmonizationRefine();

    /* I: origin picture
     * In: picture after rough removal
     * mask: shadow mask*/
    //void refine(cv::Mat I, cv::Mat In, cv::Mat mask);
private:
   // cv::Mat GenerateCorrectionRegion(cv::Mat mask, cv::Mat I, cv::Mat In);
};


#endif //BAYESIANREMOVAL_HARMONIZATIONREFINE_H
