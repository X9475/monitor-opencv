#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent): QWidget(parent), ui(new Ui::Widget)
{
    ui->setupUi(this);

    labelVector.append(ui->labelJpeg1);
    labelVector.append(ui->labelJpeg2);
    labelVector.append(ui->labelJpeg3);
    labelVector.append(ui->labelJpeg4);

    m_index = 0;

    manager = new QNetworkAccessManager(this);
    thread = new QThread(this);
    detectWork = new FaceDetection;   // 不能指定父对象
    detectWork->moveToThread(thread); // 将人脸检测动作对象移动到线程
    // 自定义信号和人脸检测类的槽函数建立连接
    connect(this,SIGNAL(imageAcquired(QImage)),detectWork,SLOT(onFaceDetection(QImage)));
    thread->start();  // 启动线程
}

Widget::~Widget()
{
    delete ui;
    thread->terminate();
    thread->wait();
}

// "获取视频"
void Widget::on_btnGetMjpg_clicked()
{
    // 1. 准备请求包数据，设置url地址，根据url自动生成请求包
    request.setUrl(QUrl("http://192.168.1.112:8080?action=stream"));
    // 2. 发送请求包，使用get方式请求获取视频流数据，返回专门用于接收视频流数据的对象
    mjpgReply = manager->get(request);
    // 3. 当服务器发送来视频流数据时，发送readyRead()信号
    connect(mjpgReply,SIGNAL(readyRead()), this,SLOT(onMjpgReadyRead()));
}
// 接收mjpg视频格式数据，从中剥离出一张jpeg图片帧
void Widget::onMjpgReadyRead()
{
    // 1. 接收数据
    mjpgBuff += mjpgReply->readAll();
    // 2. 剥离图片帧
    // 1) 定义变量保存开始标记和结束标记
    char startFlag[3] = {(char)0xff,(char)0xd8}; // 开始标记  0xff 0xd8
    char endFlag[3] = {(char)0xff,(char)0xd9};   // 结束标记  0xff 0xd9
    // 2) 查找开始标记和结束标记
    int startIndex = mjpgBuff.indexOf(startFlag,0); // 从0开始查找"开始标记"
    if(startIndex == -1) { // 没有找到
        mjpgBuff.clear(); // 缓冲区数据不完整，清空缓冲区
        return;
    }
    int endIndex = mjpgBuff.indexOf(endFlag,startIndex);// 从"开始标记"位置开始查找结束标记
    if(endIndex == -1) // 找到"开始标记",但是没有"结束标记":说明图片没有接收完
        return;
    // 3) 从视频流中截取一张完整图片帧的数据
    QByteArray imgBuff = mjpgBuff.mid(startIndex,endIndex+2);
    // 3 将QByteArray的数据加载到QImage中，图片格式为“JPG”
    bool res = mjpgImg.loadFromData(imgBuff,"JPG");
    if(!res) { // 图片加载失败，说明图片数据有问题
        mjpgBuff.clear();
        return;
    } else { // 图片加载成功
        // 4. 显示在label上
        ui->labelMjpg->setPixmap(QPixmap::fromImage(mjpgImg));
        // 5. 发射信号，进行人脸检测
        emit imageAcquired(mjpgImg);
        mjpgBuff.clear();
        return;
    }
}
// "获取快照"
void Widget::on_btnGetJpeg_clicked()
{
    // 1. 准备请求包数据，设置url地址，根据url自动生成请求包
    request.setUrl(QUrl("http://192.168.1.112:8080?action=snapshot"));
    // 2. 发送请求包，使用get方式请求获取快照数据，返回专门用于接收快照数据的对象
    jpegReply = manager->get(request);
    // 3. 当服务器发送来快照数据时，发送readyRead()信号
    connect(jpegReply,SIGNAL(readyRead()), this,SLOT(onJpegReadyRead()));
}
// 接收mjpg视频格式数据，从中剥离出一张jpeg图片帧
void Widget::onJpegReadyRead()
{
    // 1. 接收数据
    jpegBuff += jpegReply->readAll();
    // 2. 剥离图片帧
    // 1) 定义变量保存开始标记和结束标记
    char startFlag[3] = {(char)0xff,(char)0xd8}; // 开始标记  0xff 0xd8
    char endFlag[3] = {(char)0xff,(char)0xd9};   // 结束标记  0xff 0xd9
    // 2) 查找开始标记和结束标记
    int startIndex = jpegBuff.indexOf(startFlag,0); // 从0开始查找"开始标记"
    if(startIndex == -1) { // 没有找到
        jpegBuff.clear(); // 缓冲区数据不完整，清空缓冲区
        return;
    }
    int endIndex = jpegBuff.indexOf(endFlag,startIndex);// 从"开始标记"位置开始查找结束标记
    if(endIndex == -1) // 找到"开始标记",但是没有"结束标记":说明图片没有接收完
        return;
    // 3) 从视频流中截取一张完整图片帧的数据
    QByteArray imgBuff = jpegBuff.mid(startIndex,endIndex+2);
    // 3 将QByteArray的数据加载到QImage中，图片格式为“JPG”
    bool res = jpegImg.loadFromData(imgBuff,"JPG");
    if(!res) { // 图片加载失败，说明图片数据有问题
        jpegBuff.clear();
        return;
    } else { // 图片加载成功
        // 4. 显示在label上
        labelVector[m_index]->setPixmap(QPixmap::fromImage(
            jpegImg.scaled(labelVector[m_index]->size(),Qt::KeepAspectRatio)));
        jpegBuff.clear();
        m_index = (++m_index)%4;
        return;
    }
}
