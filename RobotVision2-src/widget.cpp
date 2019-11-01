#include "widget.h"
#include "ui_widget.h"
#include <QtGui>
#ifndef PROPERTY
#define PROPERTY

#define image_width 640 //图片显示宽度
#define image_height 480    //图片显示高度
#define image_Format QImage::Format_RGB888 //图片显示格式
#define cameraDevice "/dev/video9"  //摄像头设备,注意!根据实际usb设备而定


#endif  //PROPERTY



/***********************************************************************************/

static float getElapse(struct timeval *tv1, struct timeval *tv2)
{
    float t = 0.0f;
    if (tv1->tv_sec == tv2->tv_sec)
        t = (tv2->tv_usec - tv1->tv_usec) / 1000.0f;
    else
        t = ((tv2->tv_sec - tv1->tv_sec) * 1000 * 1000 + tv2->tv_usec - tv1->tv_usec) / 1000.0f;
    return t;
}

/*****************************************NCNN*************************************************/
//static int detect_lenet(const cv::Mat& bgr, std::vector<float>& cls_scores)  //检测前resize到227X227进行检测，但检测后图片尺寸不变
//{
//    ncnn::Net lenet;

//    lenet.load_param("../lenet_new/lenet_gray.param");
//    lenet.load_model("../lenet_new/lenet_gray.bin");
//    ncnn::Mat in = ncnn::Mat::from_pixels_resize(bgr.data, ncnn::Mat::PIXEL_GRAY, bgr.cols, bgr.rows, 36, 28);//PIXEL_GRAY/PIXEL_RGB2GRAY
//    const float mean_vals[1] = { 60.f };


//    in.substract_mean_normalize(mean_vals, 0);

//    ncnn::Extractor ex = lenet.create_extractor();
//    ex.set_light_mode(true);

//    ex.input("data", in);

//    ncnn::Mat out;
//    ex.extract("prob", out);

//    cls_scores.resize(out.c);
//    for (int j = 0; j<out.c; j++)
//    {
//        const float* prob = out.data + out.cstep * j;
//        cls_scores[j] = prob[0];
//    }

//    return 0;
//}

static int print_topk(const std::vector<float>& cls_scores, int topk, vector<int>& index_result, vector<float>& score_result)
{
    // partial sort topk with index
    int size = cls_scores.size();
    std::vector< std::pair<float, int> > vec;
    vec.resize(size);
    for (int i = 0; i<size; i++)
    {
        vec[i] = std::make_pair(cls_scores[i], i);
    }

    std::partial_sort(vec.begin(), vec.begin() + topk, vec.end(), std::greater< std::pair<float, int> >());

    // print topk and score
    for (int i = 0; i<topk; i++)
    {
        float score = vec[i].first;
        int index = vec[i].second;
        index_result.push_back(index);
        score_result.push_back(score);

        fprintf(stderr, "%d = %f\n", index, score);
    }

    return 0;
}

/*************************************************************************************/

 Widget::Widget(){}

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    //ui->setupUi(this);


    Qt::WindowFlags flags=0;
    flags|=Qt::WindowMinimizeButtonHint;
    setWindowFlags(flags);
    setWindowFlags(windowFlags()&~Qt::WindowMinMaxButtonsHint);

    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->showFullScreen();

    its = new struct Bbox;
    tallTime =0.0f;
    currentFrame =0;

    //**人眼参数
    eye_close_times = 0;
    blink_num = 0;
    w_dst = 0;
    h_dst = 0;
    //***点头参数
    nod_num = 0;
    nod_threshold_mean = 0;
    nod_threshold_sum = 0;
    eye_mean_h = 0;
    mouth_mean_h = 0;
    nose_mean_h = 0;

    s1 = "../data/face/";
    s2 = "../data/eye/";

    imgBuf = (unsigned char *)malloc(image_width * image_height/*QWidget::width()*QWidget::height()*/* 3 * sizeof(char));
    frame = new QImage(imgBuf,image_width,image_height,image_Format);   //根据存储空间开辟QImage图像
    m_camera = new VideoDevice(tr(cameraDevice));  //获取摄像头设备

    connect(m_camera, SIGNAL(display_error(QString)), this,SLOT(display_error(QString)));
    camReturn = m_camera->open_device();    //打开摄像头
    if(-1==camReturn)
    {
        QMessageBox::warning(this,tr("error"),tr("open /dev/dsp error"),QMessageBox::Yes);
        m_camera->close_device();
    }

    camReturn = m_camera->init_device();    //初始化摄像头
    if(-1==camReturn)
    {
        QMessageBox::warning(this,tr("error"),tr("init failed"),QMessageBox::Yes);
        m_camera->close_device();
    }

    camReturn = m_camera->start_capturing();    //摄像头开始捕获图像
    if(-1==camReturn)
    {
        QMessageBox::warning(this,tr("error"),tr("start capture failed"),QMessageBox::Yes);
        m_camera->close_device();
    }

    if(-1==camReturn)
    {
        QMessageBox::warning(this,tr("error"),tr("get frame failed"),QMessageBox::Yes);
        m_camera->stop_capturing();
    }

   //QObject::connect(pushButton_2,SIGNAL(clicked()),this,SLOT(on_pushButton_2_clicked()));
    printf("*************cacacacacacacacacacacacacaccacacacac***************\n");
    printf("******currentFrame is %d\n",currentFrame);

    //设定时间间隔，对图像界面进行刷新
    timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(update()));   //update将调用paintEvent
    timer->start(30);
    printf("EEeeeeeeeeeeeeeeeeeeeeeeeee\n");


    // Storage for the rectangles detected
   // cascadeFile = haarXML;
    //cascade = (CvHaarClassifierCascade *) cvLoad(cascadeFile.toUtf8());
   // m_FaceCount = 0;
   // storage = cvCreateMemStorage(0);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void Widget::paintEvent(QPaintEvent *)
{

    struct timeval  tv5, tv6;
    struct timezone tz5, tz6;
    gettimeofday(&tv5, &tz5);


    printf("00\n");
    uchar * pImgBuf;
    unsigned int len;
    camReturn = m_camera->get_frame((void **)&pImgBuf,&len);
    printf("22\n");
    convert_yuv_to_rgb_buffer(pImgBuf,imgBuf,image_width,image_height/*QWidget::width(),QWidget::height()*/);
    printf("33\n");
    //frame->loadFromData((uchar *)imgBuf,/*len*/image_width * image_height * 3 * sizeof(char));
    //printf("44\n");
    //QImage转IplImage
    //IplImage* src = QImageToIplImage(frame);

    //  if (!src)
    // {
    //       printf("img error!");
    //    return;
    //  }
    //cv::Mat cv_img;
    // mtcnn a;
    // cv_img = cv::Mat(src,true);
    // perclos_f(cv_img,a);   //实现检测
    printf("55\n");
    cv::Mat I;
    //  I=cv::Mat(src);

    printf("66\n");
    I.create(480, 640, CV_8UC3);
    memcpy(I.data, imgBuf, 640 * 480 * 3);
    cvtColor(I,I,CV_RGB2BGR);
    perclos_f(I,mt,ey);   //实现检测
    cvtColor(I,I,CV_BGR2RGB);
    const uchar *rgb1 = (const uchar*)(I.data);
    printf("77\n");
    QImage* qimage = new QImage(rgb1,640,480,QImage::Format_RGB888);
    printf("******qqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqq*****\n");

    // QImage* mage = new QImage(rgb1,640,480,QImage::Format_RGB888);
    printf("88\n");
    QImage resultimg=qimage->scaled(ui->m_imgLabel->size(),Qt::KeepAspectRatio,Qt::SmoothTransformation);
    ui->m_imgLabel->setPixmap(QPixmap::fromImage(resultimg));

    // QImage qimage = QImage((uchar *)src->imageData, src->width,src->height, image_Format);
    //IplImage为BGR格式，QImage为RGB格式，所以要交换B和R通道显示才正常
    //可以用OpenCV的cvConcertImage函数交换，也可以用QImage的rgbSwapped函数交换；
    //qimage = qimage.rgbSwapped();
    printf("99\n");
    //ui->m_imgLabel->setPixmap(QPixmap::fromImage(qimage));
    camReturn = m_camera->unget_frame();
    printf("10\n");

    //cvReleaseImage(&src);   //释放图片内存
    I.release();
    printf("11\n");


    gettimeofday(&tv6, &tz6);

    printf("!!!!!!!!!!!!!%s = %g ms \n ", "Detection for 1 frame  time", getElapse(&tv5, &tv6));
}

void Widget::display_error(QString err)
{
    QMessageBox::warning(this,tr("error"), err,QMessageBox::Yes);
}

/*yuv格式转换为rgb格式*/
int Widget::convert_yuv_to_rgb_buffer(unsigned char *yuv, unsigned char *rgb, unsigned int width, unsigned int height)
{
 unsigned int in, out = 0;
 unsigned int pixel_16;
 unsigned char pixel_24[3];
 unsigned int pixel32;
 int y0, u, y1, v;
 for(in = 0; in < width * height * 2; in += 4) {
  pixel_16 =
   yuv[in + 3] << 24 |
   yuv[in + 2] << 16 |
   yuv[in + 1] <<  8 |
   yuv[in + 0];
  y0 = (pixel_16 & 0x000000ff);
  u  = (pixel_16 & 0x0000ff00) >>  8;
  y1 = (pixel_16 & 0x00ff0000) >> 16;
  v  = (pixel_16 & 0xff000000) >> 24;
  pixel32 = convert_yuv_to_rgb_pixel(y0, u, v);
  pixel_24[0] = (pixel32 & 0x000000ff);
  pixel_24[1] = (pixel32 & 0x0000ff00) >> 8;
  pixel_24[2] = (pixel32 & 0x00ff0000) >> 16;
  rgb[out++] = pixel_24[0];
  rgb[out++] = pixel_24[1];
  rgb[out++] = pixel_24[2];
  pixel32 = convert_yuv_to_rgb_pixel(y1, u, v);
  pixel_24[0] = (pixel32 & 0x000000ff);
  pixel_24[1] = (pixel32 & 0x0000ff00) >> 8;
  pixel_24[2] = (pixel32 & 0x00ff0000) >> 16;
  rgb[out++] = pixel_24[0];
  rgb[out++] = pixel_24[1];
  rgb[out++] = pixel_24[2];
 }
 return 0;
}

int Widget::convert_yuv_to_rgb_pixel(int y, int u, int v)
{
 unsigned int pixel32 = 0;
 unsigned char *pixel = (unsigned char *)&pixel32;
 int r, g, b;
 r = y + (1.370705 * (v-128));
 g = y - (0.698001 * (v-128)) - (0.337633 * (u-128));
 b = y + (1.732446 * (u-128));
 if(r > 255) r = 255;
 if(g > 255) g = 255;
 if(b > 255) b = 255;
 if(r < 0) r = 0;
 if(g < 0) g = 0;
 if(b < 0) b = 0;
 pixel[0] = r * 220 / 256;
 pixel[1] = g * 220 / 256;
 pixel[2] = b * 220 / 256;
 return pixel32;
}
/*yuv格式转换为rgb格式*/

//QImage 转 IplImage
IplImage* Widget::QImageToIplImage(const QImage * qImage)
{
    int width = qImage->width();
    int height = qImage->height();
    CvSize Size;
    Size.height = height;
    Size.width = width;
    IplImage *IplImageBuffer = cvCreateImage(Size, IPL_DEPTH_8U, 3);
    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            QRgb rgb = qImage->pixel(x, y);
            cvSet2D(IplImageBuffer, y, x, CV_RGB(qRed(rgb), qGreen(rgb), qBlue(rgb)));
        }
    }
    return IplImageBuffer;
}

//人脸检测函数
//void Widget::detect_and_draw(IplImage *img)
//{
//        static CvScalar colors[] =
//    {
//        {{0,0,255}},
//        {{0,128,255}},

//}
cv::Mat Widget::perclos_f(cv::Mat & cv_img_,mtcnn &mt_,eye_statue &ey_ )
{

        printf("551\n");
        memset(its, 0, sizeof(*its));
        bool face_exist =true;
        cv::Scalar text_face = cv::Scalar(144, 238, 144);

        //**人眼参数
        //int eye_close_times = 0;
        //struct Bbox* its = new struct Bbox; //定义用来保存最大人脸的结构体指针
        // std::vector<float> vec_eye_status(2000);

        //***点头参数

        printf("552\n");
        cv::Mat dst;//眼区域
        currentFrame++;
        printf("553\n");

        cout << "Now is reading" << currentFrame << "." << endl;
        sprintf(str_Frame, "%s%d", "CURRENT FRAME:", currentFrame);
        putText(cv_img_, str_Frame, Point(5, 30), CV_FONT_HERSHEY_SIMPLEX, 0.5, Scalar(255, 0, 0), 2, 2);
        printf("554\n");
        sprintf(num_string, "%d.jpg", currentFrame);

        printf("555\n");
        //eye_filename = ImageSave_Path +"eye/"+ num_string + ".jpg";
        printf("556\n");
        face_filename = s1 + num_string;
        eye_filename = s2 + num_string;

        printf("557\n");
        //mtcnn mm;
        ncnn::Mat ncnn_img = ncnn::Mat::from_pixels(cv_img_.data, ncnn::Mat::PIXEL_BGR2RGB, cv_img_.cols, cv_img_.rows);//PIXEL_BGR2RGB --->bgr-to-rgb//opencv读出的图片格式是BGR，需转成RGB,否则检出率会很低,转为GRAY检不到人脸
        printf("558\n");
        struct timeval  tv1, tv2;
        struct timezone tz1, tz2;
        gettimeofday(&tv1, &tz1);
        printf("559\n");
        mt_.detect(ncnn_img, oneBbox1, twoBbox1, finalBbox1); ////检测人脸，图片检测之前已转为RGB格式

        gettimeofday(&tv2, &tz2);
        printf("5510\n");
        printf("%s = %g ms \n ", "Detect All face time", getElapse(&tv1, &tv2));


        float maxface_w_size = 0;//最大人脸参数
        float maxface_h_size = 0;

        printf("5511\n");
        if(oneBbox1.size()==0||twoBbox1.size()==0||finalBbox1.size()==0){

            putText(cv_img_, "NO Faces are detected!", Point(50, 240), CV_FONT_HERSHEY_SIMPLEX, 1.5, Scalar(0, 0, 255), 2, 2);

            face_exist=false;
            currentFrame=0;
            nod_threshold_sum=0;
            w_dst = 0;
            h_dst = 0;
            vec_nod_threshold.clear();
            vec_eye_status.clear();

        }
        printf("5512\n");
   if(face_exist==true){

           for (vector<Bbox>::iterator it = finalBbox1.begin(); it != finalBbox1.end(); it++){

                    bool out_flag = 0;
                    for(int j=0;j<=9;j++){
                          if((float)*(it->ppoint + j)<0){
                                 printf("Face 0ut Frame!\n");
                                 out_flag=1;
                                 break;

                          }
                    }

                    if (out_flag==1){continue;}
                    printf("5513\n");
                    if ((*it).exist){


                         w_dst = abs((float)*(it->ppoint) - (float)*(it->ppoint + 1))*0.6;  //眼宽
                         h_dst = w_dst*0.6;  //0.5                                              //眼高

                        if (w_dst > maxface_w_size){

                             printf("Enter loop !!!!\n");

                             maxface_w_size = w_dst;  //初始为0
                             maxface_h_size = h_dst;

                             (*its) = (*it);          //将最大人脸复制到结构体中保存

                         }

                    }

           }

           printf("5514\n");
        /*********************************点头检测********************************start**/
        //eye_mean_h = ((float)*(its->ppoint + 5) + (float)*(its->ppoint + 6))*0.5;  //h1
        nose_mean_h = (float)*(its->ppoint + 7);                                   //h2
        //mouth_mean_h = ((float)*(its->ppoint + 8) + (float)*(its->ppoint + 9))*0.5;//h3

        printf("!!!!!!!!!!!!!!!!!!!!!mouth-mean_h= %.3f\n",nose_mean_h);
        printf("5515\n");

       // vec_nod_threshold[currentFrame] = mouth_mean_h;
        vec_nod_threshold.push_back(nose_mean_h);
        printf("!!!!!!!!!!!!!!!!!!!vec_nod_threshold[currentFrame-1]= %.3f\n",vec_nod_threshold[currentFrame-1]);
        printf("5516\n");

        if (currentFrame <= int(rate)){    //延后1s检测

            nod_threshold_sum = nod_threshold_sum + nose_mean_h;
            printf("!!!!!!!!!!!!!!!!!!!!!!!nod_threshold_sum= %.3f\n",nod_threshold_sum);
            printf("5517\n");
        }

        else{
            nod_threshold_mean = nod_threshold_sum / int(rate);
            printf("!!!!!!!!!!!!!!!nod_threshold_mean*1.15=%.3f\n",nod_threshold_mean*1.15);
            if (nose_mean_h >= (nod_threshold_mean*1.2)){
                putText(cv_img_, "Head_status:Noding!", Point(5, 150), CV_FONT_HERSHEY_SIMPLEX, 0.8, Scalar(0, 0, 255), 1.8, 2);

                cv::line(cv_img_,Point(560,10),Point(515,70),Scalar(0,0,255),1.5,8);
                cv::line(cv_img_,Point(560,10),Point(605,70),Scalar(0,0,255),1.5,8);
                cv::line(cv_img_,Point(605,70),Point(515,70),Scalar(0,0,255),1.5,8);
                putText(cv_img_, "!", Point(552, 59), CV_FONT_HERSHEY_SIMPLEX, 1.6, Scalar(0, 0, 255), 2.1, 2);
                putText(cv_img_, "Attention!Look Ahead!", Point(430, 100), CV_FONT_HERSHEY_SIMPLEX, 0.60, Scalar(0, 255, 255), 1.9, 2);
                if (vec_nod_threshold[currentFrame-1]>vec_nod_threshold[currentFrame - 2] && vec_nod_threshold[currentFrame - 2]<=nod_threshold_mean*1.2){
                    nod_num++;
                    printf("5518\n");
                }
            }
            else{
                 putText(cv_img_, "Head_status:Normal", Point(5, 150), CV_FONT_HERSHEY_SIMPLEX, 0.7, Scalar(0, 255, 0), 1.8, 2);
            }
        }
    /**************************************************************6-8-------debug by hzq********************************************************************/
        printf("5519\n");
        sprintf(str_nod, "%s%d", "Nod_num is:", nod_num);
        putText(cv_img_, str_nod, Point(5, 180), CV_FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 255, 255), 1, 2);
        printf("5520\n");
        /********************************点头检测**********************************end*/

        //在标记之前定义人眼区域
        //dst = cv_img_(Rect((float)*(its->ppoint) - maxface_w_size*0.5, (float)*(its->ppoint + 5) - maxface_h_size*0.5, maxface_w_size, maxface_h_size));

        //在标记之前定义人眼区域,并且保证抠取的眼睛区域在画面内，否则opencv出错
        if ((float)*(its->ppoint) - maxface_w_size*0.5 < 0){
            if ((float)*(its->ppoint + 5) - maxface_h_size*0.5 < 0){

                printf("The ROI of right eye is outside the frame!!! Length and Width  all out!!!\n");
                dst = cv_img_(Rect( 0 , 0 ,   maxface_w_size*0.5 + (float)*(its->ppoint)  ,   maxface_h_size*0.5 + (float)*(its->ppoint + 5) ));
            }
            else{

                printf("The ROI of right eye  is  outside the frame!!! Length out!!!\n");
                dst = cv_img_(Rect(0, (float)*(its->ppoint + 5) - maxface_h_size*0.5, maxface_w_size*0.5 + (float)*(its->ppoint), maxface_h_size));
            }
        }
        else{
            if ((float)*(its->ppoint + 5) - maxface_h_size*0.5 < 0){
                printf("The ROI of right eye  is  outside the frame!!! Width out!!!\n");
                dst = cv_img_(Rect( (float)*(its->ppoint) - maxface_w_size*0.5, 0 ,   maxface_w_size  ,   maxface_h_size*0.5 + (float)*(its->ppoint + 5)  ));
            }
            else{

                printf("ROI is going well!\n");
                dst = cv_img_(Rect((float)*(its->ppoint) - maxface_w_size*0.5, (float)*(its->ppoint + 5) - maxface_h_size*0.5, maxface_w_size, maxface_h_size));
            }
        }


        printf("5521\n");

        cvtColor(dst, dst, COLOR_BGR2GRAY);// -->to gray ,channel=1

        // bbbb=dst.channels();
        // printf("**********************************channels===== %d\n",bbbb);

        printf("5522\n");
        //前馈run lenet网络
        // vector<float> cls_scores;
        struct timeval  tv3, tv4;
        struct timezone tz3, tz4;
        gettimeofday(&tv3, &tz3);
        printf("5523\n");
       //detect_lenet(dst, cls_scores);
        ey_.detect_lenet_eye(dst,cls_scores);
        gettimeofday(&tv4, &tz4);
        printf("5524\n");
        printf("%s = %g ms \n ", "Detection eye time", getElapse(&tv3, &tv4));
        //printf("Time using cpu of detection eye status : %.3f ms\n",(clock()-t_eyedetect)*1.0/1000);

        printf("5525\n");
           tallTime= getElapse(&tv3, &tv4) + getElapse(&tv1, &tv2);
           sprintf(str_tallTime,"%s%g%s","Detection Speed:",tallTime,"ms");
           putText(cv_img_, str_tallTime, Point(5, 60), CV_FONT_HERSHEY_SIMPLEX, 0.5, Scalar(255, 0, 0), 2, 2);
        printf("5526\n");
        //查找识别结果标签
        //vector<int> index;
        //vector<float> score;
        print_topk(cls_scores, 1, index, score); //打印top-1
        printf("5527\n");
        printf("***************************************index[0]===%d\n", index[0]);
       //vec_eye_status[currentFrame] = index[0];
        vec_eye_status.push_back(index[0]);
        printf("5528\n");
        if (index[0] == 0){  // 0=close , 1=open
            eye_close_times++;
            putText(cv_img_, "Eye_status:Closing!", Point(5, 100), CV_FONT_HERSHEY_SIMPLEX, 0.7, Scalar(0, 0, 255), 1.8, 2);
            printf("5529\n");
        }

        else{
            putText(cv_img_, "Eye_status:Opening!", Point(5, 100), CV_FONT_HERSHEY_SIMPLEX, 0.7, Scalar(0, 255, 0), 1.8, 2);
        }

        printf("5530\n");
        //perclos检测是否疲劳,延后1s检测
        int sum_eye_status = 0;

        if (currentFrame > int(rate)){
            for (int i = 1; i <= int(rate); i++){
                sum_eye_status += vec_eye_status[currentFrame - i];
            }
            printf("5531\n");
            if (sum_eye_status <= int(rate*0.7)){
                putText(cv_img_, "Fatigue!", Point(250, 50), CV_FONT_HERSHEY_SIMPLEX, 1.5, Scalar(0, 0, 255), 2, 2);
                text_face = Scalar(0, 0, 255);
                printf("5532\n");
            }

            else{
                putText(cv_img_, "Clear!", Point(250, 50), CV_FONT_HERSHEY_SIMPLEX, 1.5, Scalar(0, 255, 0), 2, 2);
            }
            printf("5533\n");
            if (vec_eye_status[currentFrame - 1] == 1 && vec_eye_status[currentFrame - 2] == 0){  //前闭后睁视为1次眨眼
                blink_num++;
            }
             printf("5534\n");
        }
        sprintf(str_blink, "%s%d", "Blink_num is:", blink_num);
        putText(cv_img_, str_blink, Point(5, 200), CV_FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 255, 255), 1, 2);
        printf("5535\n");


        rectangle(cv_img_, Point((*its).x1, (*its).y1), Point((*its).x2, (*its).y2), text_face, 0.1, 8, 0);//画人脸
        printf("x,y===%d,%d,x.,y.===%d,%d\n",(*its).x1, (*its).y1,(*its).x2, (*its).y2);
        printf("5536\n");

        for (int num = 0; num < 5; num++){

            printf("Landmark [x,y]: [%.3f,%.3f] \n", (float)*(its->ppoint + num), (float)*(its->ppoint + num + 5)); //输出每个特征点坐标
            circle(cv_img_, Point((float)*(its->ppoint + num), (float)*(its->ppoint + num + 5)), 2, Scalar(0, 205, 205), -1);
        }

        printf("|x1-x2|= %.3f, |y1-y2|= %.3f \n", abs((float)*(its->ppoint) - (float)*(its->ppoint + 1)), abs((float)*(its->ppoint + 5) - (float)*(its->ppoint + 6)));

        //cv::imshow("face_roi", cv_img_);
        //imwrite(face_filename, cv_img_);

        printf("5537\n");

   }

   oneBbox1.clear();
   twoBbox1.clear();
   finalBbox1.clear();

   cls_scores.clear();
   index.clear();
   score.clear();


   printf("5538\n");
   return cv_img_;

}


void Widget::on_pushButton_2_clicked()
{
    close();
}

//void Widget::on_pushButton_clicked()
//{
//    //设定时间间隔，对图像界面进行刷新
//    timer = new QTimer(this);
//    connect(timer,SIGNAL(timeout()),this,SLOT(update()));   //update将调用paintEvent
//    timer->start(30);
//    printf("EEeeeeeeeeeeeeeeeeeeeeeeeee\n");
//}
