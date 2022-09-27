/**
 * @copyright Copyright (c) 2021 PhiGent
 *
 */

#ifndef VISION_TYPE_VISION_TYPE_HPP_
#define VISION_TYPE_VISION_TYPE_HPP_

#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <memory>
#ifdef __ANDROID__
#include "opencv2/core/core_c.h"
#include "opencv2/core/core.hpp"
#else
#include "opencv2/core/core.hpp"
#endif

namespace phigent {
namespace vision {

template <typename Dtype>
struct Point_ {
  inline Point_() { x = y = 0; }
  inline Point_(Dtype x_, Dtype y_) {
    x = x_;
    y = y_;
  }

  Dtype x, y;
};
using Point = Point_<float>;

template <typename Dtype>
struct Point3d_ {
  Dtype x = 0;

  Dtype y = 0;

  Dtype z = 0;

  inline Point3d_(Dtype x_, Dtype y_, Dtype z_) : x(x_), y(y_), z(z_) {}
  inline Point3d_(Point_<Dtype> point) : x(point.x), y(point.y), z(0) {}
  inline Point3d_(Point_<Dtype> point, Dtype z_)
      : x(point.x), y(point.y), z(z_) {}
};
using Point3d = Point3d_<float>;

template <typename Dtype>
struct BBox_ {
  inline BBox_() {
    score = x1 = x2 = y1 = y2 = attribute = 0;
    category_name = "";
  }
  inline BBox_(Dtype x1_, Dtype y1_, Dtype x2_, Dtype y2_, float score_ = 0.0f,
               float attri_ = 0.0f, std::string category_name_ = "") {
    x1 = x1_;
    y1 = y1_;
    x2 = x2_;
    y2 = y2_;
    score = score_;
    attribute = attri_;
    category_name = category_name_;
  }
  inline Dtype width() const { return (x2 - x1); }
  inline Dtype height() const { return (y2 - y1); }
  inline Dtype cx() const { return (x1 + (x2 - x1) / 2); }
  inline Dtype cy() const { return (y1 + (y2 - y1) / 2); }
  inline friend std::ostream &operator<<(std::ostream &out, BBox_ &bbox) {
    out << "( x1: " << bbox.x1 << " y1: " << bbox.y1 << " x2: " << bbox.x2
        << " y2: " << bbox.y2 << " score: " << bbox.score << " )";
    return out;
  }
  inline static bool greater(const BBox_ &a, const BBox_ &b) {
    return a.score > b.score;
  }

  Dtype x1, y1, x2, y2;

  float score, attribute;

  std::string category_name = "";
};
using BBox = BBox_<float>;
typedef std::shared_ptr<BBox> spBBox;

template <typename Dtype>
struct BBox3D_ {
  inline BBox3D_() = default;

  // 中心点的x, y, z
  Dtype x = 0, y = 0, z = 0;
  // 物体的长宽高
  Dtype d = 0, w = 0, h = 0;
  // 物体的三个姿态角
  Dtype pitch = 0, yaw = 0, roll = 0;
  // 得分
  float score = 0;
  // 类别
  int category = 0;
  std::string category_name = "";
};
using BBox3D = BBox3D_<float>;
using spBBox3D = std::shared_ptr<BBox3D>;

template <typename Dtype>
struct Landmarks_ {
  inline Landmarks_() {
    num = 0;
    num_id = 0;
    pose = -1;
  }
  inline Landmarks_(int num_) {
    num = num_;
    score.resize(num);
    coords.resize(num << 1);
    num_id = 0;
    pose = -1;
  }
  inline void Push(float score_, Dtype x_, Dtype y_) {
    score.push_back(score_);
    coords.push_back(x_);
    coords.push_back(y_);
  }
  inline bool IsValidFace() {
    for (int i = 0; i < num; ++i) {
      if (score[i] <= float(-0.9)) {
        return false;
      }
    }
    return true;
  }
  inline void GetAnchoXY(std::vector<int> &anchor_id_list, Dtype &out_x,
                         Dtype &out_y) {
    int ancho_size = anchor_id_list.size();
    for (int i = 0; i < ancho_size; ++i) {
      int anchor_id = anchor_id_list[i];
      out_x += coords[anchor_id << 1];
      out_y += coords[(anchor_id << 1) + 1];
    }
    out_x /= ancho_size;
    out_y /= ancho_size;
  }

  int num, num_id;

  float pose;

  std::vector<Dtype> coords;

  std::vector<float> affine;

  std::vector<float> score;

  std::vector<int> anchor_points_idx_1, anchor_points_idx_2, anchor_center_idx;
};
using Landmarks = Landmarks_<float>;

template <typename Dtype>
struct HumanSkeleton_ {
  inline HumanSkeleton_() { point_num = 0; }
  inline HumanSkeleton_(int point_num_) {
    point_num = point_num_;
    points.resize(point_num);
    scores.resize(point_num);
  }

  int point_num;

  std::vector<Point> points;

  std::vector<float> scores;
};
typedef HumanSkeleton_<float> HumanSkeleton;

struct ComboBox {
  BBox main_bbox;
  BBox sub_bbox;
  int main_label;
  int sub_label;
  int attr_label;
  float attr_score;
  std::vector<float> attr_scores;
  HumanSkeleton pts; // point num is 2
  std::vector<int> pt_visible_labels;
  float subbox_truncated_v;
  float subbox_truncated_h;
  bool is_tracking;
  std::vector<float> tracking_feature;
};
typedef std::shared_ptr<ComboBox> spComboBox;

struct FaceAttributes {
  inline FaceAttributes() {
    num_id = 0;
    attribute_score.clear();
  }

  std::vector<float> attribute_score;

  int num_id;
};

// in the map, key is index_id of track, value is recognition object id
typedef std::map<int, int> RecogResultList;

template <typename Dtype>
struct HeadInfo_ {
  int32_t head_id = -1;
  bool valid = false;
  BBox_<Dtype> head_bbox;
};
typedef HeadInfo_<float> HeadInfo;

template <typename Dtype>
struct FaceInfo_ {
  int32_t face_id = -1;
  bool valid = false;
  BBox_<Dtype> face_bbox;
  int32_t sex = -1;
  int32_t age = -1;
  float female_score = 0.0f;
  float male_score = 0.0f;
  Landmarks_<Dtype> landmarks;
};
typedef FaceInfo_<float> FaceInfo;

template <typename Dtype>
struct BodyInfo_ {
  int32_t body_id = -1;
  bool valid = false;
  BBox_<Dtype> body_bbox;
  float body_ration = 0.0f;
  HumanSkeleton_<Dtype> skeleton;
};
typedef BodyInfo_<float> BodyInfo;

template <typename Dtype>
struct PersonInfo_ {
  int32_t cam_id = -1;
  int32_t frame_id = -1;
  int64_t time = 0;
  int32_t person_id = -1;
  HeadInfo_<Dtype> head_info;
  FaceInfo_<Dtype> face_info;
  BodyInfo_<Dtype> body_info;
};
typedef PersonInfo_<float> PersonInfo;

}  // namespace vision


namespace road {
enum class LanemarkColor { // the color of lane
	UNKNOWN = 0,
	WHITE = 1, 
	YELLOW = 2, 
	BLUE = 3, 
	GREEN = 4, 
	RED = 5
};

enum class RoadEdgeType {
	UNKNOWN = 0,
	LANE_MARK = 1, 
	CURB_EDGE = 2, 
	GUARDRAIL = 3, 
	DITCH = 4, 
	WALL = 5,  
	NONE = 6
};

enum class LanemarkType {  
	UNKNOWN=0,
	SINGLE_DOTTED_LINE=1,
	SHORT_DOTTED_LINE=2,
	SHORT_THICK_DOTTED_LINE=3,
	DOUBLE_DOTTED_LINE=4,
	SINGLE_SOLID_LINE=5,
	DOUBLE_SOLID_LINE=6,
	LEFT_DOTTED_RIGHT_SOLID=7,
	LEFT_SOLID_RIGHT_DOTTED=8,
  DIAMOND_LINE=9
};

struct Lane{
  float score;
  bool is_road_edge;
  bool is_stopline;
  std::vector<vision::Point> coords;
  std::vector<float> coord_scores;  // the score of every point in lane
  std::vector<int> ranges;
  LanemarkColor color;
  RoadEdgeType load_edge_type;
  LanemarkType lanemark_type;
  
  size_t length(){return coords.size();}
};
typedef std::shared_ptr<Lane> spLane;

}  // namespace road


namespace face_snap {
typedef std::shared_ptr<phigent::vision::FaceInfo> sp_FaceInfo;
typedef std::shared_ptr<phigent::vision::BodyInfo> sp_BodyInfo;

struct FaceSnapData {
  int track_id;
  long long frame_id;
  time_t time_stamp;
  float score;
  sp_FaceInfo face_info;
  sp_FaceInfo rel_face_info;
  phigent::vision::BBox crop_box;
  cv::Mat whole_img;
  cv::Mat snap_img;

  sp_BodyInfo matched_body_info;
};
typedef std::shared_ptr<FaceSnapData> sp_FaceSnapData;
}  // namespace face_snap

// the format of whole_img and snap_img is rgb or bgr

namespace person_snap {
typedef std::shared_ptr<phigent::vision::FaceInfo> sp_FaceInfo;
typedef std::shared_ptr<phigent::vision::BodyInfo> sp_BodyInfo;
struct PersonSnapData {
  int track_id;
  long long frame_id;
  time_t time_stamp;
  int orientation;
  float quality;
  float person_score;

  phigent::vision::BBox crop_box;
  sp_BodyInfo body_info;
  sp_BodyInfo rel_body_info;
  cv::Mat snap_img;
  cv::Mat whole_img;

  sp_FaceInfo matched_face_info;
};
typedef std::shared_ptr<PersonSnapData> sp_PersonInfo;
}  // namespace person_snap

// the format of whole_img and snap_img is rgb or bgr

namespace one_person_one_record {

enum SnapRank {
  A_CLASS = 1,
  B_CLASS = 2,
  C_CLASS = 3
};

// data type
typedef std::vector<float> Feature;
typedef std::shared_ptr<Feature> sp_Feature;

struct FaceSnapData {
  int track_id;
  long long frame_id;
  time_t time_stamp;
  std::string cam_info;  // camera info
  phigent::face_snap::sp_FaceInfo face_info;
  SnapRank rank;  // A / B / C
  float quality_score;
  cv::Mat loose_crop_img;
  sp_Feature feature;

  // for debug info
  int _debug_gt_id = -1;
};
typedef std::shared_ptr<FaceSnapData> sp_FaceSnapData;
}  // namespace one_person_one_record

namespace traffic_snap {

struct MotorVehicle {
  int track_id;
  phigent::vision::spBBox sp_bbox;
  phigent::vision::spBBox sp_front_window_bbox;
  phigent::vision::spBBox sp_plate_bbox;
};
typedef std::shared_ptr<MotorVehicle> spMotorVehicle;


struct NonMotorVehicle {
  int track_id;
  phigent::vision::spBBox sp_bbox;
  std::vector<phigent::face_snap::sp_FaceInfo> face_info;
  std::vector<phigent::person_snap::sp_BodyInfo> body_info;

};
typedef std::shared_ptr<NonMotorVehicle> spNonMotorVehicle;


struct NonMotorSnapData {
    int track_id;
    long long frame_id;
    time_t time_stamp;
    float score;
    spNonMotorVehicle abs_snap_box_info;
    spNonMotorVehicle rel_snap_box_info;
    phigent::vision::BBox crop_box;
    cv::Mat snap_img;
    cv::Mat whole_img;

};
typedef std::shared_ptr<NonMotorSnapData> spNonMotorSnapData;

struct MotorSnapData {
    int track_id;
    long long frame_id;
    time_t time_stamp;
    float score;
    spMotorVehicle abs_snap_box_info;
    spMotorVehicle rel_snap_box_info;
    phigent::vision::BBox crop_box;
    cv::Mat snap_img;
    cv::Mat whole_img;
};
typedef std::shared_ptr<MotorSnapData> spMotorSnapData;

}  // namespace traffic_snap
}  // namespace phigent
#endif  // VISION_TYPE_VISION_TYPE_HPP_
