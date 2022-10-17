#include <chrono>
#include <cstdio>
#include <memory>
#include <string>

#include "glog/logging.h"
#include "opencv2/opencv.hpp"
#include "pg/utils/opencv_helper.hpp"
#include "pg/vidar_interface.hpp"

static std::string ReadTextFile(const std::string &file_name) {
  FILE *fd = fopen(file_name.c_str(), "r");
  fseek(fd, 0, SEEK_END);
  int cfg_file_size = ftell(fd);
  rewind(fd);
  std::string cfg_str;
  cfg_str.resize(cfg_file_size + 1);
  cfg_str[cfg_file_size] = '\0';
  fread(&cfg_str[0], sizeof(char), cfg_file_size, fd);
  fclose(fd);
  return cfg_str;
}

namespace {

class FpsCounter {
 public:
  FpsCounter() { start_time_ = std::chrono::high_resolution_clock::now(); }
  void AddOne() { frame_count_++; };
  float Compute() {
    const auto time_now = std::chrono::high_resolution_clock::now();
    auto dur_ms = std::chrono::duration_cast<std::chrono::milliseconds>(
                      time_now - start_time_)
                      .count();
    if (0 == dur_ms) {
      return 0;
    }
    return static_cast<float>(frame_count_) / dur_ms * 1000;
  }
  size_t FrameCount() const { return frame_count_; }
  void Update(size_t frame_count,
              std::chrono::high_resolution_clock::time_point start_time) {
    frame_count_ = frame_count;
    start_time_ = start_time;
  }

 private:
  std::chrono::high_resolution_clock::time_point start_time_;
  size_t frame_count_ = 0;
};

}  // namespace

int main(int argc, char const *argv[]) {
  if (argc < 2) {
    printf("usage: %s <path to config file>", argv[0]);
    return -1;
  }
  const char *cfg_file_name = argv[1];
  const char *flow_file_name = argv[2];
  using namespace pg::vidar;
  std::unique_ptr<VidarInterface> vidar(VidarInterface::Create());
  auto conf_json = ReadTextFile(cfg_file_name);
  auto flow_json = ReadTextFile(flow_file_name);
  int Init_Status = vidar->Init(conf_json);
  // Check if VidarInterface is successful
  if (Init_Status < 0) {
    LOG(ERROR) <<
        "VidarInterface Init failed, Please check availablilty of Host Device "
        "and connectioned of USB";
    return 0;
  }
  FpsCounter fps_counter;
  FpsCounter imu_counter;
  VidarData vidar_data;
  std::string ret_str;
  int GetConfig_Status = -1;
  while(GetConfig_Status < 0){
    GetConfig_Status = vidar->GetConfig(flow_json, ret_str);
  }
  LOG(INFO) << "GetConfig_Status: " << GetConfig_Status;
  LOG(INFO) << "test config receive: " << ret_str;
  
  while (true) {
    int ret = vidar->RecvData(&vidar_data, 1000);
    if (ret < 0 || vidar_data.images.empty()) {
      LOG(ERROR) << "RecvData failed " << ret;
      continue;
    }
    fps_counter.AddOne();
    auto counter = fps_counter.FrameCount();
    if (counter % 100 == 0) {
      LOG(INFO) << "Output fps=" << fps_counter.Compute();
      fps_counter.Update(0, std::chrono::high_resolution_clock::now());
    }
    
    for (auto &img : vidar_data.images) {
     cv::Mat raw_mat, bgr_mat;
      pg::utils::helper::ImageFrame2CvMat(*img, raw_mat, false, false);
      if (kPGPixelFormatRawI420 == img->pixel_format) {
        cv::cvtColor(raw_mat, bgr_mat, cv::COLOR_YUV2BGR_I420);
      } else if (kPGPixelFormatRawGRAY == img->pixel_format) {
        cv::cvtColor(raw_mat, bgr_mat, cv::COLOR_GRAY2BGR);
      }
      // cv::imshow(std::to_string(img->channel_id), bgr_mat);
      cv::imwrite(std::to_string(img->time_stamp) + "_ns_" +
                     std::to_string(img->channel_id) + ".png",
                 bgr_mat);
      LOG(INFO) << "recv channel_id=" << img->channel_id;
    }
    
    for (auto &imu : vidar_data.imu) {
      LOG(INFO) << "recv imu" << imu;
      imu_counter.AddOne();
      auto icounter = imu_counter.FrameCount();
      if (icounter % 100 == 0) {
        LOG(INFO) << "Imu Output fps=" << imu_counter.Compute();
        imu_counter.Update(0, std::chrono::high_resolution_clock::now());
      }
    }
    auto key = cv::waitKey(5);
    if (key == 'x' || counter > 1000) {
      break;
    }
  }
  vidar->Deinit();

  return 0;
}
