#ifndef DIGITRECOGNITION_H
#define DIGITRECOGNITION_H
#include<string>
#include<opencv2/core.hpp>
#include<opencv2/dnn/dnn.hpp>
#include<opencv2/dnn/blob.hpp>
#include<opencv2/imgproc.hpp>
using namespace std;


class DigitRecognition
{
public:
    DigitRecognition();
    bool InitDNN(string prototxt, string caffemodel);
    bool predict(cv::Mat& img,int& classId,double& classProb);
    ~DigitRecognition();
private:
    string dnn_prototxt_path;
    string dnn_caffemodel_path;
    cv::dnn::Net net;
    void getMaxClass(cv::dnn::Blob &probBlob, int *classId, double *classProb);

};

#endif // DIGITRECOGNITION_H
