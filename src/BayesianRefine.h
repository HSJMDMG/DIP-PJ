//
// Created by 卢颖 on 2017/5/20.
//

#ifndef BAYESIANREMOVAL_BAYESIANREFINE_H
#define BAYESIANREMOVAL_BAYESIANREFINE_H

class BayesianRefine {
    public:
        BayesianRefine();
        virtual ~BayesianRefine();
        void refine(cv::Mat img, cv::Mat mask, std::string output_dir);

    private:


};



#endif //BAYESIANREMOVAL_BAYESIANREFINE_H
