#ifndef WIDGET_H
#define WIDGET_H
#include <stdio.h>
#include <algorithm>
#include <vector>
#include <math.h>
#include <iostream>
#include <sys/time.h>
/**************************/
#include<mtcnn.h>
#include<eye_statue.h>
/*************************/
#include <string>
#include <fstream>
#include <time.h>
#include <QWidget>
#include "videodevice.h"


                                                      //2018.6.8 By hzq
namespace Ui {
    class Widget;
}

class Widget : public QWidget {
    Q_OBJECT
public:
    Widget(QWidget *parent = 0);

    ~Widget();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::Widget *ui;
    VideoDevice *m_camera;  //摄像头
    uchar *imgBuf;  //QImage图像空间
    int camReturn;  //摄像头使用过程返回的结果，用于状态判断
    QImage *frame;  //QImage图像
    QTimer *timer;
    cv::Mat perclos_f(cv::Mat & cv_img_,mtcnn &mt_,eye_statue &ey_);
    CvMemStorage *storage;
    QString cascadeFile;
    CvHaarClassifierCascade *cascade;
    int m_FaceCount;

    const float rate =25;   //摄像头采集视频帧率
    mtcnn mt;
    eye_statue ey;


    //前馈run lenet网络
    vector<float> cls_scores;

    //查找识别结果标签
    vector<int> index;
    vector<float> score;

    string s1;
    string s2;
    string eye_filename;//眼图路径
    string face_filename;//视频帧路径
    char  num_string[25];

    std::vector<Bbox> oneBbox1, twoBbox1, finalBbox1;
    float tallTime;
    char str_tallTime[30];
    int currentFrame;
    char str_Frame[30];


    //**人眼参数
    std::vector<float> vec_eye_status;
    //float ary_eye_status[2000]

    struct Bbox* its ;//= new struct Bbox; //定义用来保存最大人脸的结构体指针
    int eye_close_times;
    int blink_num;
    char str_blink[20];

    float w_dst, h_dst;


    //***点头参数
    int nod_num;
    float nod_threshold_mean,nod_threshold_sum;
    std::vector<float> vec_nod_threshold;

    char str_nod[20];

    float eye_mean_h, mouth_mean_h, nose_mean_h;

    //以下两个函数用于YUV格式转换成RGB格式
    int convert_yuv_to_rgb_pixel(int y, int u, int v);
    int convert_yuv_to_rgb_buffer(unsigned char *yuv, unsigned char *rgb, unsigned int width, unsigned int height);
    IplImage *QImageToIplImage(const QImage * qImage);
    //void detect_and_draw(IplImage *img);


private slots:
    void paintEvent(QPaintEvent *);
    void display_error(QString err);
    void on_pushButton_2_clicked();
//    void on_pushButton_clicked();
};

#endif // WIDGET_H
