# 移动端实时疲劳驾驶检测
在ARM端实现疲劳驾驶实时检测，检测速度20FPS以上。
软件环境: 移动端框架ncnn、opencv、qt。
硬件环境: samsung6818A53系列ARM,cpu: Cortex-A53,8核。
# 检测流程
通过USB（红外）摄像头采集驾驶员面部图像,基于MTCNN检测驾驶员人脸，然后借助CNN对驾驶员眼部状态进行识别，最后依据眼部PERCLOS准则做出疲劳预警。
# 具体实现
1.交叉编译Opencv\ncnn的arm库；
2.基于QT的Qwidget类创建QT widget Application，videodevice类管理摄像头，eye_statue类实现眼睛状态识别，mouth_statue类实现嘴部状态识别，mtcnn类实现人脸及特征点检测，widget类实现PERCLOS疲劳检测，及低头预警；
3.疲劳检测时延后1s检测，从当前往前5s内闭眼帧数大于阈值时视为疲劳，发出警告。当头部低于一定阈值时，提示“注意！看前方”。
# ToDo
竞赛作品，代码有些粗糙，可能存在未发现bug，后续可能完善。
