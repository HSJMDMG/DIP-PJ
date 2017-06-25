//
// Created by 卢颖 on 2017/5/22.
//

#ifndef BAYESIANREMOVAL_INTENSITYDETECTOR_H
#define BAYESIANREMOVAL_INTENSITYDETECTOR_H

class IntensityDetector {

public:
    IntensityDetector();
    virtual  ~IntensityDetector();
    IplImage* detectShadows(std::string img_dir, std::string mask_dir);


private:

    IplImage* HisztogramSzethuzas(IplImage* img);
    IplImage* createHistogram(IplImage* img,bool exceptnull,bool tisztitas);
    IplImage* drawLine(IplImage* img,int line);
    IplImage* drawLineGroup(IplImage* img,int line);
    void blur(IplImage* image,int size);
    IplImage* findShadow2(const char* filename, const char* maskname);



};


#endif //BAYESIANREMOVAL_INTENSITYDETECTOR_H
