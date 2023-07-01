#include "facedetection.h"
#include <opencv2/opencv.hpp>
#include <iostream>
#include <QDebug>
using namespace cv;

FaceDetection::FaceDetection(QObject *parent) : QObject(parent) { }

// 人脸识别
void FaceDetection::onFaceDetection(QImage img)
{
    // 将图片转换为RGB24位色
    QImage image = img.convertToFormat(QImage::Format_RGB888);
    // 将QImage图片的格式转换为Mat格式
    Mat matImg = Mat(image.height(), image.width(), CV_8UC3,image.bits(), image.bytesPerLine());
    // 将图片进行灰度转换
    Mat matGray;
    cvtColor(matImg,matGray,COLOR_RGB2GRAY);
    // 分类器
    CascadeClassifier faceClassfier;    // 创建分类器
    std::string classfierPath = "../MjpgClient/haarcascade_frontalface_default.xml";
    faceClassfier.load(classfierPath);  // 加载分类器
    // 创建一个向量，保存检测到的人脸的数据
    std::vector<Rect> faces;
    // 直方图均衡化处理
    equalizeHist(matGray,matGray);
    //多尺寸检测人脸
    faceClassfier.detectMultiScale(
                matGray,       // 待检测的输入图像(灰度图像)
                faces,         // 保存检测到的人脸数据的容器
                1.1,           // 每次检测时尺寸缩小的比例
                3,             // 至少检测3次才确认目标
                0,             // 默认值0
                Size(30,30)    // 目标最小尺寸
                );
    if(!faces.empty()){ // 检测到人脸
        QString str = "检测到" + QString::number(faces.size())+"人进入监控区域";
        qDebug() << str;
    }
}








