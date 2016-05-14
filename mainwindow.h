#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <string>
#include <opencv2/core.hpp>
#include <QtWidgets>
class DigitRecognition;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    DigitRecognition* digit_recognition;
    std::string net_prototext_path;
    std::string net_caffemodle_path;
    void openFile();
    void display(cv::Mat img);
    void setPrototxt();
    void setCaffeModel();
    void InitDNNModel();
    bool predict();
    cv::Mat  test_img;

};

#endif // MAINWINDOW_H
