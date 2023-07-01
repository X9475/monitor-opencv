#ifndef FACEDETECTION_H
#define FACEDETECTION_H

#include <QObject>
#include <QImage>

class FaceDetection : public QObject
{
    Q_OBJECT
public:
    explicit FaceDetection(QObject *parent = nullptr);

signals:
public slots:
    // 人脸识别
    void onFaceDetection(QImage);
};

#endif // FACEDETECTION_H
