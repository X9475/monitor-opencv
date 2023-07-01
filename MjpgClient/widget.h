#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QLabel>
#include <QVector>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include <QThread>
#include <QDebug>
#include "facedetection.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
signals:
    // 自定义信号
    void imageAcquired(QImage);
private slots:
    // 接收到视频流数据时
    void onMjpgReadyRead();
    void on_btnGetMjpg_clicked();
    // 接收到快照数据时，处理readyRead信号的槽函数
    void onJpegReadyRead();
    void on_btnGetJpeg_clicked();

private:
    Ui::Widget *ui;

    QNetworkAccessManager* manager;     // 管理整个通信过程
    QNetworkRequest request;            // 根据url生成请求包

    QNetworkReply* mjpgReply;           // 获取视频流响应数据包
    QByteArray mjpgBuff;                // 保存视频流响应数据
    QImage mjpgImg;                     // 保存从视频流中截取的一张图片

    QNetworkReply* jpegReply;           // 获取快照响应数据包
    QByteArray jpegBuff;                // 保存快响应数据
    QImage jpegImg;                     // 保存从快照数据中剥离的图片

    QThread* thread;
    FaceDetection* detectWork;          // 人脸检测类对象

    QVector<QLabel*> labelVector;       // 保存用来显示快照的4个QLabel
    int m_index;                        // 索引
};
#endif // WIDGET_H
