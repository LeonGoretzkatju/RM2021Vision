#include "../Include/Predictor.h"
#include <Eigen/Dense>

using namespace cv;
using namespace Eigen;

bool Predictor::coordinate_trans(Trace& trace){
    Point3f camera_pos = solve_pnp(trace);
    Matrix<double, 3, 1> camera_coo;
    Matrix<double, 3, 1> world_pos;
    Matrix3d r_inverse, r_x, r_y, r_yaw, r_pitch;

    camera_coo << camera_pos.x, camera_pos.y, camera_pos.z;
    // cout << "                      coo                " << camera_pos.x << " "
    //                                                     << camera_pos.y << " "
    //                                                     << camera_pos.z << endl;
    r_x << 1, 0, 0,
           0, cos(trace.pitch), sin(trace.pitch),
           0, -sin(trace.pitch), cos(trace.pitch);
    r_y << cos(-trace.yaw), 0, -sin(-trace.yaw),
           0, 1, 0,
           sin(-trace.yaw), 0, cos(-trace.yaw);
    r_pitch << 1, 0, 0,
               0, cos(trace.pitch), sin(trace.pitch),
               0,-sin(trace.pitch), cos(trace.pitch);
    r_yaw << cos(trace.yaw), 0, sin(trace.yaw),
             0, 1, 0,
             -sin(trace.yaw), 0, cos(trace.yaw);
    r_inverse = (r_pitch * r_yaw).transpose();

    camera_coo[0] += 20;
    camera_coo[1] += 0;
    camera_coo[2] += 50;

    world_pos = r_inverse * camera_coo;

    trace.world_position = Point3f(world_pos[0], world_pos[1], world_pos[2]);
    // std::cout << "              wddd               " << trace.world_position << std::endl;
    return true;
}

cv::Point3f Predictor::solve_pnp(Trace& trace){
    // 获取target box的四个点坐标
    Rect2d box = trace.armor.armor_rect;
    
    // 装甲板实际宽高
    float armor_real_height = trace.armor.get_real_heigt();
    float armor_real_width = trace.armor.get_real_width();

    // 特征点世界坐标（以特征点所在平面为xy）
    std::vector<Point3f> obj_pnts;
    // left top point.
    obj_pnts.emplace_back(Point3f(-0.5 * armor_real_width, -0.5 * armor_real_height, 0));
    // right top point.
    obj_pnts.emplace_back(Point3f(0.5 * armor_real_width, -0.5 * armor_real_height, 0));
    // right bottom point.
    obj_pnts.emplace_back(Point3f(0.5 * armor_real_width, 0.5 * armor_real_height, 0));
    // left bottom point.
    obj_pnts.emplace_back(Point3f(-0.5 * armor_real_width, 0.5 * armor_real_height, 0));

    // 特征点图像坐标
    std::vector<Point2f> img_pnts;
    img_pnts.emplace_back(Point2f(box.x, box.y));
    img_pnts.emplace_back(Point2f(box.x + box.width, box.y));
    img_pnts.emplace_back(Point2f(box.x + box.width, box.y + box.height));
    img_pnts.emplace_back(Point2f(box.x, box.y + box.height));

    // 相机内参矩阵 + 畸变矩阵
    // fx, 0, cx
    // 0, fy, cy
    // 0, 0, 1
    Mat inner_matrix = (Mat_<double>(3, 3) << 940.8970, 0, 325.7120,
                                                0, 942.1127, 255.2932,
                                                0, 0, 1.0000);
    Mat distortion_matrix = (Mat_<double>(5, 1) << -0.0904, -0.1856, 0.2290, 0.0, 0.0);

    // solve pnp
    Mat rvecs = cv::Mat::zeros(3, 1, CV_64FC1);
    Mat tvecs = cv::Mat::zeros(3, 1, CV_64FC1);
    // cv::Mat Rod_r ,TransMatrix ,RotationR;
    bool success = cv::solvePnP(
        obj_pnts, img_pnts, inner_matrix, distortion_matrix, rvecs, tvecs
    );
    trace.distance = 10700.0/box.height*10.0;
    // Rodrigues(Rod_r, RotationR);-10700.0/box.height*10.0
    // cout << "C(Camera center:):" << endl << -RotationR.inv()*TransMatrix << endl;//这个C果然是相机中心，十分准确
    // cout << "shiji distance                                     " << -tvecs.ptr<double>(0)[2] << endl;
    return Point3f(tvecs.ptr<double>(0)[0], -tvecs.ptr<double>(0)[1], -trace.distance);

}

void Predictor::predictor_init(Trace &target) {
    if (!KF->is_Initialized)
    {
        KF->is_Initialized = !KF->is_Initialized;
        auto inityaw = get_yaw(target.world_position.x,target.world_position.z);
        auto initpitch = get_pitch(target.world_position.x,target.world_position.y,target.world_position.z);
        KF->statePose << inityaw,initpitch,0.0,0.0;
    }
}

Point2f Predictor::predict(){
    Trace last, now;
    this->armor_traces.get_now2(now, last);
    cout << "now time" << "               " << now.time << endl;
    cout << "last time" << "                " << last.time << endl;
    double w_yaw,w_pitch;
    double old_w_yaw, old_w_pitch;
    double a_yaw,a_pitch;
    cout << "delta time" << (now.time - last.time) << endl;
    cout << "old delta time" << " " << (last.time - KF->time_stamp) << endl;
    auto now_yaw = get_yaw(now.world_position.x,now.world_position.z);
    auto now_pitch = get_pitch(now.world_position.x,now.world_position.y,now.world_position.z);
    auto last_yaw = get_yaw(last.world_position.x,last.world_position.z);
    auto last_pitch = get_pitch(last.world_position.x,last.world_position.y,last.world_position.z);
    w_yaw = (now_yaw - last_yaw) / (now.time - last.time);
    w_pitch = (now_pitch - last_pitch) / (now.time - last.time);
    this->A(0, 2) = now.time - last.time;
    this->A(1, 3) = now.time - last.time;
    if (this->KF->first_set)
    {
        KF->first_set = !KF->first_set;
        this->KF->u << 0.0,
                0.0;
        this->KF->B << 0.5* pow((now.time - last.time),2), 0,
                0, 0.5*pow((now.time - last.time), 2),
                (now.time - last.time), 0,
                0, (now.time - last.time);
        this->KF->measPose << now_yaw, now_pitch, w_yaw, w_pitch;
        KF->predict();
        KF->update();
        this->last_yaw_speed = w_yaw;
        this->last_pitch_speed = w_pitch;
        this->last_time_stamp = (now.time - last.time);
        return Point2f (KF->statePose(0),KF->statePose(1));
    }
    else
    {
        a_yaw = (w_yaw - this->last_yaw_speed)/(now.time- this->last_time_stamp);
        a_pitch = (w_pitch - this->last_pitch_speed)/(now.time- this->last_time_stamp);
        cout <<"delta yaw"<< "   " << ((now.time - last.time)- this->last_time_stamp) << endl;
        this->KF->u << a_yaw,
                a_pitch;
        this->KF->B << 0.5* pow((now.time - last.time),2), 0,
                0, 0.5*pow((now.time - last.time), 2),
                (now.time - last.time), 0,
                0, (now.time - last.time);
        this->KF->measPose << now_yaw, now_pitch, w_yaw, w_pitch;
        KF->predict();
        KF->update();
        this->last_yaw_speed = w_yaw;
        this->last_pitch_speed = w_pitch;
        this->last_time_stamp = now.time;
        return Point2f (KF->statePose(0),KF->statePose(1));
    }
}