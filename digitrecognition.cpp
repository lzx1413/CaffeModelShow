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
    dnn_prototxt_path = prototxt;
    dnn_caffemodel_path = caffemodel;
    cv::Ptr<dnn::Importer> importer;//创建caffe model的导入器
    try
    {
        importer = dnn::createCaffeImporter(this->dnn_prototxt_path,this->dnn_caffemodel_path);//导入lenet的模型和参数
    }
    catch(const cv::Exception &err)
    {
        cerr<<err.msg<<endl;
    }
    if(!importer)
    {
        std::cerr << "Can't load network by using the following files: " << std::endl;
        std::cerr << "prototxt:   " << this->dnn_prototxt_path << std::endl;
        std::cerr << "caffemodel: " << this->dnn_caffemodel_path << std::endl;

    }
    importer->populateNet(this->net);//初始化神经网络
    importer.release();
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
    dnn::Blob inputBlob = dnn::Blob(img);
    net.setBlob(".data",inputBlob);//设置输入blob
    try{
    net.forward();//网络前向计算
    }
    catch(const cv::Exception err){
        std::cerr<<err.msg<<std::endl;
        return false;

    }
    dnn::Blob prob = net.getBlob("loss");//获得概率结果
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
