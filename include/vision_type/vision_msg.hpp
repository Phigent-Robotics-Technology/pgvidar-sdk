/**
 * @copyright Copyright (c) 2021 PhiGent
 *
 */

#ifndef VISION_TYPE_VISION_MESSAGES_HPP_
#define VISION_TYPE_VISION_MESSAGES_HPP_

#ifdef __ANDROID__
#include "opencv2/core/core_c.h"
#include "opencv2/core/core.hpp"
#else
#include "opencv2/core/core.hpp"
#endif

#include <functional>
#include <string>
#include <vector>
#include <cstdint>
#include "json/json.h"
#include "flow/flow.h"
#include "vision_type/base_type.hpp"
#include "vision_type/vision_type.hpp"

namespace phigent {
namespace vision {

struct Header {
  /* data */
};

template<typename T, typename ExT = std::vector<int>>
class FlowMessage : public Message {
 public:
  FlowMessage() = default;
  FlowMessage(T in_data, ExT ext, std::function<std::string()> func)
          : data(in_data), ext_data(ext), dump_json_func(func) {}
  T data;
  ExT ext_data;
  std::string DumpJson() override {
    if (dump_json_func)
      return dump_json_func();
    return "";
  }
  void SetDumpJsonFunc(std::function<std::string()> func) {
    dump_json_func = func;
  }
 private:
  std::function<std::string()> dump_json_func;
};


/**
 * @brief Define message used for CV Mat object
 */
class CvMatMessage : public Message {
 public:
  CvMatMessage() = default;

  ~CvMatMessage() override = default;

  cv::Mat &GetImage() { return image_; }
  phigent::vision::ImageFramePtr& GetImageFrame() {
    CvMatToImageFramePtr();
    return imageFrame_;
  }
  std::string DumpJson() override {
    Json::Value jv;
    jv["name"] = typeid(*this).name();
    auto &data_jv = jv["data"];
    data_jv["col"] = image_.cols;
    data_jv["row"] = image_.rows;
    data_jv["dims"] = image_.dims;
    data_jv["channels"] = image_.channels();
    data_jv["depth"] = image_.depth();
    data_jv["type"] = image_.type();
    data_jv["flags"] = image_.flags;
    data_jv["size"]["height"] = image_.size().height;
    data_jv["size"]["width"] = image_.size().width;
    data_jv["step"] = size_t(image_.step);
    // dump to string
    Json::StreamWriterBuilder wbuilder;
    wbuilder["indentation"] = "";
    return Json::writeString(wbuilder, jv);
  }

 private:
  cv::Mat image_;
  phigent::vision::ImageFramePtr imageFrame_ = std::make_shared<phigent::vision::ImageFrameImpl>();
  void CvMatToImageFramePtr() {
    if (!image_.data || image_.empty()) {
      return;
    } else{
      phigent::vision::DataBufferPtr data_buff = std::make_shared<phigent::vision::DataBuffer>();
      data_buff->data = std::shared_ptr<char>(reinterpret_cast<char*>(image_.data), [=](char *p) {});
      data_buff->data_size = image_.rows * image_.step;
      imageFrame_ = std::make_shared<phigent::vision::ImageFrameImpl>(data_buff, image_.cols, image_.rows, image_.channels(), image_.step.buf[0]);
    }
  }
};
using spCvMatMessage = std::shared_ptr<CvMatMessage>;

/**
 * @brief Define message used for a BBox vector
 */
class RectsMessage : public Message {
 public:
  RectsMessage() = default;

  ~RectsMessage() override = default;

  std::vector<BBox> &GetRects() { return rects_; }

  std::vector<int> &GetIdxList() { return idx_list_; }

  std::string DumpJson() override {
    Json::Value jv;
    jv["name"] = typeid(*this).name();
    auto &data_jv = jv["data"];
    auto &rects_jv = data_jv["rects"];
    for (Json::Value::ArrayIndex i = 0; i < rects_.size(); i++) {
      rects_jv[i]["x"] = rects_[i].x1;
      rects_jv[i]["y"] = rects_[i].y1;
      rects_jv[i]["width"] = rects_[i].width();
      rects_jv[i]["height"] = rects_[i].height();
    }
    auto &idx_list_jv = data_jv["index_list"];
    for (Json::Value::ArrayIndex i = 0; i < idx_list_.size(); i++) {
      idx_list_jv[i] = idx_list_[i];
    }
    // dump to string
    Json::StreamWriterBuilder wbuilder;
    wbuilder["indentation"] = "";
    return Json::writeString(wbuilder, jv);
  }

 private:
  std::vector<BBox> rects_;

  std::vector<int> idx_list_;
};
using spRectsMessage = std::shared_ptr<RectsMessage>;

/**
 * @brief Define message used for Landmarks vector
 */
class LandmarksMessage : public Message {
 public:
  LandmarksMessage() = default;

  ~LandmarksMessage() override = default;

  std::vector<Landmarks> &GetLandmarks() { return landmarks_; }

  std::vector<int> &GetIdxList() { return idx_list_; }

  float &GetPose() { return pose_; }

 private:
  std::vector<Landmarks> landmarks_;

  float pose_;

  std::vector<int> idx_list_;
};
using spLandmarksMessage = std::shared_ptr<LandmarksMessage>;

/**
 * @brief Define message used for a attribute vector
 */
class AttributesMessage : public Message {
 public:
  AttributesMessage() = default;

  ~AttributesMessage() override = default;

  std::vector<std::vector<FaceAttributes>> &GetAttributes() {
    return attributes_;
  }

  std::vector<int> &GetIdxList() { return idx_list_; }

 private:
  std::vector<std::vector<FaceAttributes>> attributes_;

  std::vector<int> idx_list_;
};
using spAttributesMessage = std::shared_ptr<AttributesMessage>;

/**
 * @brief Define message used for face metric features
 */
class FaceMetricFeaturesMessage : public Message {
 public:
  FaceMetricFeaturesMessage() = default;

  ~FaceMetricFeaturesMessage() override = default;

  std::vector<std::vector<float>> &GetFeatures() { return metric_feature_; }

  std::vector<bool> &GetHasFeature() { return has_metric_feature_; }

  std::vector<int> &GetIdxList() { return idx_list_; }

 private:
  std::vector<std::vector<float>> metric_feature_;

  std::vector<bool> has_metric_feature_;

  std::vector<int> idx_list_;
};
using spFaceMetricFeaturesMessage = std::shared_ptr<FaceMetricFeaturesMessage>;
using ReidFeatureMessage = FaceMetricFeaturesMessage;
using spReidFeatureMessage = std::shared_ptr<ReidFeatureMessage>;

/**
 * @brief Message used to transform Face Recognition result
 */
class RecogMessage : public Message {
 public:
  RecogMessage() = default;
  ~RecogMessage() override = default;

  RecogResultList recog_result_;
};
using spRecogMessage = std::shared_ptr<RecogMessage>;

/**
 * @brief Message that represents a group of persons
 */
class PersonInfoMessage : public Message {
 public:
  PersonInfoMessage() = default;

  ~PersonInfoMessage() override = default;

  std::vector<PersonInfo> persons_;
};
using spPersonInfoMessage = std::shared_ptr<PersonInfoMessage>;

/**
 * @brief Define message used for human skeleton vectors
 */
class HumanSkeletonMessage : public Message {
 public:
  HumanSkeletonMessage() = default;

  ~HumanSkeletonMessage() override = default;

  std::vector<HumanSkeleton> &GetHumanSkeletons() { return human_skeletons_; }

  std::vector<int> &GetIdxList() { return idx_list_; }

 private:
  std::vector<HumanSkeleton> human_skeletons_;

  std::vector<int> idx_list_;
};
using spHumanSkeletonMessage = std::shared_ptr<HumanSkeletonMessage>;

class MessageListMessage : public Message {
 public:
  MessageListMessage() = default;

  ~MessageListMessage() override = default;

  const std::vector<spMessage> &GetMessageList() { return message_list_; }
  const int GetMessageListLength() { return message_list_.size(); }
  void AddMessage(spMessage message) { this->message_list_.push_back(message); }
  void DeleteMessageFromBack() { this->message_list_.pop_back(); }
  void ClearMessageList() { message_list_.clear(); }

 private:
  std::vector<spMessage> message_list_;
};
using spMessageListMessage = std::shared_ptr<MessageListMessage>;

class FlagMessage : public Message {
public:
  FlagMessage() = default;
  ~FlagMessage() override = default;
  int flag = -1;
};
using spFlagMessage = std::shared_ptr<FlagMessage>;


class PointsMessage : public Message {
  public:
    PointsMessage() = default;
    ~PointsMessage() override = default;
    std::vector<Point>& GetPoints(){ return points_;}
    std::vector<int>& GetCategories(){ return point_categories_; }
    std::string DumpJson() override {
      Json::Value jv;
      jv["name"] = typeid(*this).name();
      auto &data_jv = jv["data"];
      for (Json::Value::ArrayIndex i = 0; i < points_.size(); i++) {
        auto& point = data_jv[i]["point"];
        point["x"] = points_[i].x;
        point["y"] = points_[i].y;
        point["category"] = point_categories_[i];
      }

    }
  private:
    std::vector<Point> points_;
    std::vector<int> point_categories_;
};
using spPointsMessage = std::shared_ptr<PointsMessage>;

class ClassifyMessage : public Message {
  public:
    ClassifyMessage() = default;
    ~ClassifyMessage() override = default;
    size_t& GetArgmaxIdx(){return argmax_idx;}
    std::vector<float>& GetClassifyResVec(){return classify_result_vec;}
    std::vector<float>& GetLmksResVec(){return lmks_result_vec;}
    std::string DumpJson() override {
      Json::Value jv;
      jv["name"] = typeid(*this).name();
      auto &data_jv = jv["data"];
      auto &classify_result_jv = data_jv["classify_result"];
      for(int i = 0; i < classify_result_vec.size(); ++i){
        classify_result_jv[i] = classify_result_vec[i];
      }
      auto &lmks_result_jv = data_jv["landmarks_result"];
      for(int i = 0; i < lmks_result_vec.size(); ++i){
        lmks_result_jv[i] = lmks_result_vec[i];
      }
      data_jv["argmax_idx"] = argmax_idx;

      Json::StreamWriterBuilder wbuilder;
      wbuilder["indentation"] = "";
      return Json::writeString(wbuilder, jv);

    }
  private:
    std::vector<float> classify_result_vec;
    std::vector<float> lmks_result_vec;
    size_t argmax_idx = 0;
};
using spClassifyMessage = std::shared_ptr<ClassifyMessage>;

class ComboBoxMessage : public Message {
  public:
    ComboBoxMessage() = default;
    ~ComboBoxMessage() override = default;
    std::vector<spComboBox> &GetComboBoxes() { return combo_boxes_; }
    std::string DumpJson() override {
    Json::Value jv;
    jv["name"] = typeid(*this).name();
    auto &data_jv = jv["data"];
    auto &combo_box_jv = data_jv["combo_boxes"];
    for (Json::Value::ArrayIndex i = 0; i < combo_boxes_.size(); i++) {
      auto& main_box = combo_box_jv[i]["main_bbox"];
      auto& sub_box = combo_box_jv[i]["sub_bbox"];
      auto& pts = combo_box_jv[i]["points"];
      main_box["x1"] = combo_boxes_[i]->main_bbox.x1;
      main_box["y1"] = combo_boxes_[i]->main_bbox.y1;
      main_box["x2"] = combo_boxes_[i]->main_bbox.x2;
      main_box["y2"] = combo_boxes_[i]->main_bbox.y2;
      main_box["score"] = combo_boxes_[i]->main_bbox.score;
      main_box["label"] = combo_boxes_[i]->main_label;
      // main_box["attr_score"] = combo_boxes_[i]->attr_score;
      // main_box["attr_label"] = combo_boxes_[i]->attr_label;
      auto& attr_scores_jv = main_box["attr_scores"];
      for (Json::Value::ArrayIndex j = 0; j < combo_boxes_[i]->attr_scores.size(); j++){
        attr_scores_jv[j] = combo_boxes_[i]->attr_scores[j];
      }
      sub_box["x1"] = combo_boxes_[i]->sub_bbox.x1;
      sub_box["y1"] = combo_boxes_[i]->sub_bbox.y1;
      sub_box["x2"] = combo_boxes_[i]->sub_bbox.x2;
      sub_box["y2"] = combo_boxes_[i]->sub_bbox.y2;
      sub_box["score"] = combo_boxes_[i]->sub_bbox.score;
      sub_box["label"] = combo_boxes_[i]->sub_label;


      for(Json::Value::ArrayIndex j = 0; j < 2; j++){
        auto& pt_jv = pts[j];
        pt_jv["x"] = combo_boxes_[i]->pts.points[j].x;
        pt_jv["y"] = combo_boxes_[i]->pts.points[j].y;
        pt_jv["score"] = combo_boxes_[i]->pts.scores[j];
        pt_jv["label"] = combo_boxes_[i]->pt_visible_labels[j];
      }

    } 
    // auto &idx_list_jv = data_jv["index_list"];
    // for (Json::Value::ArrayIndex i = 0; i < idx_list_.size(); i++) {
    //   idx_list_jv[i] = idx_list_[i];
    // }
    // dump to string
    Json::StreamWriterBuilder wbuilder;
    wbuilder["indentation"] = "";
    return Json::writeString(wbuilder, jv);
  }

  private:
    std::vector<spComboBox> combo_boxes_;
};
using spComboBoxMessage = std::shared_ptr<ComboBoxMessage>;

/**
 * @brief Define message used for multi label class
 */
class MultiLabelMessage : public phigent::Message {
public:
  MultiLabelMessage() = default;

  ~MultiLabelMessage() override = default;

  std::vector<bool>& GetAttrs() { return attrs_; }
  std::vector<float>& GetAttrScores() { return attrs_scores_; }
  std::string DumpJson() override {
    Json::Value jv;
    jv["name"] = typeid(*this).name();
    auto &data_jv = jv["data"];
    auto &attrs_result_jv = data_jv["attrs_"];
    for(int i = 0; i < attrs_.size(); ++i){
        if(attrs_[i]){
          attrs_result_jv[i] = 1;
        }else{
          attrs_result_jv[i] = 0;
        }
    }
    auto &attrs_scores__result_jv = data_jv["attrs_scores_"];
    for(int i = 0; i < attrs_scores_.size(); ++i){
      attrs_scores__result_jv[i] = attrs_scores_[i];
    }
    Json::StreamWriterBuilder wbuilder;
    wbuilder["indentation"] = "";
    return Json::writeString(wbuilder, jv);
  }

private:
  // 多标签分类的指示向量 如[1, 0, 0, 1, 1]
  std::vector<bool> attrs_;
  // 多标签分类的类别得分 如[0.5, 0.1, 0.6, 0.4]
  std::vector<float> attrs_scores_;
};

using spMultiLabelMessage = std::shared_ptr<MultiLabelMessage>;

}  // namespace vision


namespace road {
  class LanesMessage : public phigent::vision::FlowMessage<std::string>{
  public:
      LanesMessage() {}
      std::vector<spLane> lane_list;
      std::string DumpJson() override {
        Json::Value jv;
        jv["name"] = typeid(*this).name();
        Json::Value arrayObj;
        for(const auto lane : lane_list){
          Json::Value item;
          item["score"] = lane->score;
          item["is_road_edge"] = lane->is_road_edge;
          item["is_stopline"] = lane->is_stopline;
          item["color"] = static_cast<int>(lane->color);
          item["load_edge_type"] = static_cast<int>(lane->load_edge_type);
          item["lanemark_type"] = static_cast<int>(lane->lanemark_type);
          Json::Value coords_json;
          for(size_t coord_idx = 0; coord_idx < lane->coords.size(); ++coord_idx){
            Json::Value pt_item;
            const auto& pt = lane->coords[coord_idx];
            pt_item["x"] = pt.x;
            pt_item["y"] = pt.y;
            pt_item["score"] = lane->coord_scores[coord_idx];
            coords_json.append(pt_item);
          }
          item["coords"] = coords_json;
          arrayObj.append(item);

        }
        jv["lanes"] = arrayObj;
        // dump to string
        Json::StreamWriterBuilder wbuilder;
        wbuilder["indentation"] = "";
        return Json::writeString(wbuilder, jv);
    }
  };
  typedef std::shared_ptr<LanesMessage> spLanesMessage;
}  // namespace road


namespace face_snap {
class FaceInfoMessage : public Message {
 public:
  FaceInfoMessage() {}
  std::vector<std::shared_ptr<phigent::vision::FaceInfo>> face_info_list_;
};
typedef std::shared_ptr<FaceInfoMessage> sp_FaceInfoMessage;

class TimeStampMessage : public Message {
 public:
  TimeStampMessage() {}
  time_t time_stamp_;
};
typedef std::shared_ptr<TimeStampMessage> sp_TimeStampMessage;

class ImgInfoMessage : public Message {
 public:
  ImgInfoMessage() {}
  int img_width_;
  int img_height_;
};
typedef std::shared_ptr<ImgInfoMessage> sp_ImgInfoMessage;

class SnapInfoMessage : public Message {
 public:
  SnapInfoMessage() {}
  std::vector<phigent::face_snap::FaceSnapData> face_snap_result_;
};
typedef std::shared_ptr<SnapInfoMessage> sp_SnapInfoMessage;
}  // namespace face_snap

namespace person_snap {
class PersonInfoMessage : public Message {
 public:
  PersonInfoMessage() {}
  std::vector<std::shared_ptr<phigent::person_snap::PersonSnapData>>
      person_info_list_;
};
typedef std::shared_ptr<PersonInfoMessage> sp_PersonInfoMessage;

class TimeStampMessage : public Message {
 public:
  TimeStampMessage() {}
  time_t time_stamp_;
};
typedef std::shared_ptr<TimeStampMessage> sp_TimeStampMessage;

class ImgInfoMessage : public Message {
 public:
  ImgInfoMessage() {}
  int img_width_;
  int img_height_;
};
typedef std::shared_ptr<ImgInfoMessage> sp_ImgInfoMessage;

class TailFlagMessage : public Message {
 public:
  TailFlagMessage() {}
  bool is_tail_;
};
typedef std::shared_ptr<TailFlagMessage> sp_TailFlagMessage;

class SnapInfoMessage : public Message {
 public:
  SnapInfoMessage() {};
  std::vector<phigent::person_snap::PersonSnapData> person_snap_result_;
};
typedef std::shared_ptr<SnapInfoMessage> sp_SnapInfoMessage;
}  // namespace person_snap

namespace one_person_one_record {

class FaceSnapListMessage : public Message {
 public:
  FaceSnapListMessage() {}
  std::vector<sp_FaceSnapData> face_snap_datas;
};
typedef std::shared_ptr<FaceSnapListMessage> spFaceSnapListMessage;
}  // namespace one_person_one_record

namespace iou_mot {
class RemovedTrackIDsMessage : public Message {
 public:
  RemovedTrackIDsMessage() {}
  std::vector<int> removed_track_ids;
};
typedef std::shared_ptr<RemovedTrackIDsMessage> spRemovedTrackIDsMessage;

}  // namespace iou_mot
}  // namespace phigent


namespace phigent {
namespace traffic_snap {


class NonMotorInfoMessage : public Message {
  public:
    NonMotorInfoMessage() {}
    std::vector<spNonMotorVehicle> non_motor_infos;
};
typedef std::shared_ptr<NonMotorInfoMessage> spNonMotorInfoMessage;


class NonMotorSnapsMessage : public Message{
public:
    NonMotorSnapsMessage() {}
    std::vector<spNonMotorSnapData> snap_list;
};
typedef std::shared_ptr<NonMotorSnapsMessage> spNonMotorSnapsMessage;


class MotorInfoMessage : public Message {
  public:
    MotorInfoMessage() {}
    std::vector<spMotorVehicle> motor_infos;
};
typedef std::shared_ptr<MotorInfoMessage> spMotorInfoMessage;


class MotorSnapsMessage : public Message{
public:
    MotorSnapsMessage() {}
    std::vector<spMotorSnapData> snap_list;
};
typedef std::shared_ptr<MotorSnapsMessage> spMotorSnapsMessage;

}  // namespace traffic_snap
}  // namespace phigent


#endif  // VISION_TYPE_VISION_MESSAGES_HPP_
