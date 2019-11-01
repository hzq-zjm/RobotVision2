#include "eye_statue.h"

eye_statue::eye_statue()
{

    lenet.load_param("../lenet_new/lenet_gray.param");
    lenet.load_model("../lenet_new/lenet_gray.bin");


}

int eye_statue::detect_lenet_eye(const cv::Mat &bgr, std::vector<float> &cls_scores)//
{
//        ncnn::Net lenet;
//        lenet.load_param("../lenet_new/lenet_gray.param");
//        lenet.load_model("../lenet_new/lenet_gray.bin");


        ncnn::Mat in = ncnn::Mat::from_pixels_resize(bgr.data, ncnn::Mat::PIXEL_GRAY, bgr.cols, bgr.rows, 36, 28);//PIXEL_GRAY
        const float mean_vals[1] = { 60.f };


        in.substract_mean_normalize(mean_vals, 0);

        ncnn::Extractor ex = lenet.create_extractor();
        ex.set_light_mode(true);

        ex.input("data", in);

        ncnn::Mat out;
        ex.extract("prob", out);

        cls_scores.resize(out.c);
        for (int j = 0; j<out.c; j++)
        {
            const float* prob = out.data + out.cstep * j;
            cls_scores[j] = prob[0];
        }

        return 0;

}
