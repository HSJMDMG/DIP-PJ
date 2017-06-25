//
// Created by 卢颖 on 2017/5/21.
//

#ifndef BAYESIANREMOVAL_ROUGHREMOVAL_H
#define BAYESIANREMOVAL_ROUGHREMOVAL_H


class RoughRemoval {

    public:
        RoughRemoval();
        virtual ~RoughRemoval();
        cv::Mat removeShadows(cv::Mat img, cv::Mat mask);

    private:
    enum ConvolutionType
    {
        /* Return the full convolution, including border */
                CONVOLUTION_FULL,
        /* Return only the part that corresponds to the original image */
                CONVOLUTION_SAME,
        /* Return only the submatrix containing elements that were not influenced by the border */
                CONVOLUTION_VALID
    };
        void conv2(const cv::Mat &img, const cv::Mat& kernel, ConvolutionType type, cv::Mat& dest);

};


#endif //BAYESIANREMOVAL_ROUGHREMOVAL_H
