#include "digitrecognition.h"
#include <iostream>
using namespace cv;
using namespace std;
DigitRecognition::DigitRecognition()
{
}
DigitRecognition::~DigitRecognition()
{

}

bool DigitRecognition::InitDNN(string prototxt,string caffemodel)
{
     cv::dnn::initModule();  //Required if OpenCV is built as static libs
    dnn_prototxt_path = prototxt;
    dnn_caffemodel_path = caffemodel;
    try
    {
         net = dnn::readNetFromCaffe(dnn_prototxt_path, dnn_caffemodel_path);
    }
    catch(const cv::Exception &err)
    {
        cerr<<err.msg<<endl;
    }
    if (net.empty())
    {
        std::cerr << "Can't load network by using the following files: " << std::endl;
        std::cerr << "prototxt:   " << dnn_prototxt_path<< std::endl;
        std::cerr << "caffemodel: " << dnn_caffemodel_path << std::endl;
        return false;
    }
    else
        std::cout<<"finish init dnn model"<<std::endl;
        return true;
}

bool DigitRecognition::predict(Mat &img, int &classId, double &classProb)
{
    if(img.empty())
        return false;
    resize(img,img,Size(28,28));//网络只接受28*28图像
    if(img.channels() ==3)//网络只接受1通道图像
        cvtColor(img,img,CV_BGR2GRAY);
    dnn::Blob inputBlob = dnn::Blob::fromImages(img);
    net.setBlob(".data",inputBlob);//设置输入blob
    try{
    net.forward();//网络前向计算
    }
    catch(const cv::Exception err){
        std::cerr<<err.msg<<std::endl;
        return false;

    }
    dnn::Blob prob = net.getBlob("prob");//获得概率结果
    getMaxClass(prob,&classId,&classProb);
    return true;


}
void DigitRecognition::getMaxClass(dnn::Blob &probBlob, int *classId, double *classProb)
{
    Mat probMat = probBlob.matRefConst().reshape(1, 1); //reshape the blob to 1x1000 matrix
    Point classNumber;
    minMaxLoc(probMat, NULL, classProb, NULL, &classNumber);//得到概率最大的id
    *classId = classNumber.x;
}
