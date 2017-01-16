#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "digitrecognition.h"
#include "opencv2/highgui.hpp"
#include <QtGui>
#include<iostream>
#include <QtWidgets>
using namespace std;
using namespace cv;
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    digit_recognition = new DigitRecognition();
    ui->setupUi(this);
    /*
     * 设置ui空间与函数的连接
     * */
    connect(ui->actionOpen,&QAction::triggered,this,&MainWindow::openFile);
    connect(ui->actionSetPrototext,&QAction::triggered,this,&MainWindow::setPrototxt);
    connect(ui->actionSetCaffeModel,&QAction::triggered,this,&MainWindow::setCaffeModel);
    connect(ui->actionExit,&QAction::triggered,this,&MainWindow::close);
    connect(ui->actionInitModel,&QAction::triggered,this,&MainWindow::InitDNNModel);
    connect(ui->predictbtn,&QPushButton::clicked,this,&MainWindow::predict);
}
/*
 * 初始化模型参数
 * */
void MainWindow::InitDNNModel()
{
    if(!net_caffemodle_path.empty()&&!net_prototext_path.empty())
    {
        cout<<net_caffemodle_path<<endl;

        cout<<net_prototext_path<<endl;
        digit_recognition->InitDNN(net_prototext_path,net_caffemodle_path);
        ui->actionInitModel->setDisabled(this);
        ui->predictbtn->setEnabled(this);
        QMessageBox::information(this, "Init","init model sucessful.");
    }
    else
        QMessageBox::warning(this, "Init",
                                "can not load the model.");

}
MainWindow::~MainWindow()
{
    delete ui;
    delete digit_recognition;
}
/*
 * 预测结果
 * */
bool MainWindow::predict()
{
    if(!test_img.empty())
    {
        int classId;
        double prob;
        auto flag = digit_recognition->predict(test_img,classId,prob);
        if(flag)
        {
        string result = "number: "+std::to_string(classId)+"  probability: "+std::to_string(prob);
        auto qresult = QString::fromStdString(result);
        ui->result_label->setText(qresult);
        return flag;
        }
        else
        {
            QMessageBox::warning(this, "Ops",
                                "there is something wrong in predict.");
            return false;

        }

    }
       else
    {
           QMessageBox::warning(this, "Ops",
                                "there is no image at all.");
           return false;
    }
}
void MainWindow::setPrototxt()
{
    QString path = QFileDialog::getOpenFileName(this,
                                                   tr("Open File"),
                                                   ".",
                                                   tr("Text Files(*.prototxt)"));
    if(!path.isEmpty())
    {
           QTextCodec *code = QTextCodec::codecForName("gb18030");
           net_prototext_path = code->fromUnicode(path).data();
    }
       else
           QMessageBox::warning(this, "Path",
                                "You did not select any file.");

}
void MainWindow::setCaffeModel()
{

    QString path = QFileDialog::getOpenFileName(this,
                                                   tr("Open File"),
                                                   ".",
                                                   tr("Text Files(*.caffemodel)"));
    if(!path.isEmpty())
    {
           QTextCodec *code = QTextCodec::codecForName("gb18030");
           net_caffemodle_path = code->fromUnicode(path).data();
    }
       else
           QMessageBox::warning(this, "Path",
                                "You did not select any file.");
}
void MainWindow::openFile()
{
    QString path = QFileDialog::getOpenFileName(this,
                                                   tr("Open File"),
                                                   ".",
                                                   tr("Text Files(*.bmp *.jpg *jpeg *.png)"));
       if(!path.isEmpty()) {
           QTextCodec *code = QTextCodec::codecForName("gb18030");
           string image_path = code->fromUnicode(path).data();
           test_img = cv::imread(image_path);
           if(!test_img.data)
           {
               QMessageBox msgbox;
               msgbox.setText(tr("the image is null"));
               msgbox.exec();
           }
           display(test_img);
       }
       else
           QMessageBox::warning(this, "Path",
                                "You did not select any file.");
}


void MainWindow::display(cv::Mat mat)
{
    cv::Mat rgb;
    QImage img;
    if(mat.channels()==3)
    {
        cv::cvtColor(mat,rgb,CV_BGR2RGB);
        img = QImage((const uchar*)(rgb.data),rgb.cols,rgb.rows,rgb.cols*rgb.channels(),QImage::Format_RGB888);
    }
    else
    {
        img = QImage((const uchar*)(mat.data),mat.cols,mat.rows,mat.cols*mat.channels(),QImage::Format_Indexed8);
    }
    ui->image->setPixmap(QPixmap::fromImage(img));
    ui->image->resize(ui->image->pixmap()->size());
    ui->image->show();
}
