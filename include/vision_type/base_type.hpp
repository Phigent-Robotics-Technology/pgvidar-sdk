/**
 * @author Jet.Sun (shuhuan.suni)
 * @date 2021-09-15
 *
 * @copyright Copyright (c) 2021 PhiGent
 *
 */

#ifndef VISION_TYPE_BASE_TYPE_HPP_
#define VISION_TYPE_BASE_TYPE_HPP_
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "vision_type/c_vision_type_common.h"

namespace phigent {
namespace vision {

/**
 * \~Chinese @brief 基础数据缓存实现
 */
struct DataBuffer {
  DataBuffer(){};
  virtual ~DataBuffer() {}
  /* data */
  std::shared_ptr<char> data = nullptr;
  size_t data_size = 0;
  size_t elemSize = 1;
};

using DataBufferPtr = std::shared_ptr<DataBuffer>;
/**
 * \~Chinese @brief 基础图像帧
 */
struct ImageFrame {
  virtual ~ImageFrame() {}
  /// \~Chinese 图片编码方式
  PGPixelFormat pixel_format = PGPixelFormat::kPGPixelFormatNone;
  uint32_t channel_id = 0;
  uint64_t time_stamp = 0;
  uint64_t frame_id = 0;
  std::string type = "";
  // 像素点数值的scale系数
  float float_scale = 1.0f;
  /// \~Chinese 自定义数据
  virtual uint8_t *CustomData() { return nullptr; }
  /**
   * @brief 数据拷贝到给定指针地址
   *
   * @param dst_addr [in] 目标存放数据的地址
   * @param size [in] 目标地址内存大小，用于安全性检查
   * @return long 返回值<0表明发生错误，>0则为拷贝数据的大小
   */
  virtual long CopyTo(uint8_t *dst_addr, uint32_t size) { return -1; }
  /// \~Chinese 使物理地址和虚拟地址都可用（一般用于嵌入式场景）
  virtual int MakePhyVirtDataAllValid() { return -1; }
  /// \~Chinese 图像数据
  virtual uint8_t *Data() { return nullptr; }
  virtual DataBufferPtr GetDataBuffer() { return nullptr; }
  /// \~Chinese UV分量数据
  virtual uint8_t *DataUV() { return nullptr; }
  /// \~Chinese 图像数据物理地址
  virtual uint8_t *PhyData() { return nullptr; }
  /// \~Chinese UV分量数据物理地址
  virtual uint8_t *PhyDataUV() { return nullptr; }
  /// \~Chinese 图片大小
  virtual uint32_t DataSize() { return 0; }
  /// \~Chinese UV分量大小
  virtual uint32_t DataUVSize() { return 0; }
  /// \~Chinese 宽度
  virtual uint32_t Width() { return 0; }
  /// \~Chinese 高度
  virtual uint32_t Height() { return 0; }
  /// \~Chinese 长度
  virtual uint32_t Stride() { return 0; }
  /// \~Chinese 长度
  virtual uint32_t Channel() { return 0; }
  /// \~Chinese uv长度
  virtual uint32_t StrideUV() { return 0; }
  /// resize this frame
  virtual std::shared_ptr<ImageFrame> Resize(
      uint32_t target_w, uint32_t target_h, uint32_t roi_x = 0,
      uint32_t roi_y = 0, uint32_t roi_w = 0, uint32_t roi_ = 0) {
    return nullptr;
  }
  /// convert color
  virtual std::shared_ptr<ImageFrame> CvtColor(PGPixelFormat target_format) {
    return nullptr;
  }
};

using ImageFramePtr = std::shared_ptr<ImageFrame>;

/**
 * \~Chinese @brief 基础图像帧默认实现
 */
struct ImageFrameImpl : public ImageFrame {
  /// \~Chinese 自定义数据
  ImageFrameImpl() = default;
  ImageFrameImpl(DataBufferPtr _DataBuffer, uint32_t _width, uint32_t _height,
                 uint32_t _channel, uint32_t _stride) {
    DataBuffer_ = _DataBuffer;
    width = _width;
    height = _height;
    channel = _channel;
    stride = _stride;
  }
  ImageFrameImpl(ImageFrameImpl &src) {
    DataBuffer_ = src.DataBuffer_;
    width = src.width;
    height = src.height;
    stride = src.stride;
  }
  uint8_t *CustomData() override { return custom_data_addr; }
  /// \~Chinese 图像数据
  uint8_t *Data() override { return virt_data_addr; }
  /// \~Chinese 图像缓存数据
  DataBufferPtr GetDataBuffer() override { return DataBuffer_; }
  /// \~Chinese UV分量数据
  uint8_t *DataUV() override { return virt_uv_data_addr; }
  /// \~Chinese device侧地址
  uint8_t *PhyData() override { return phy_data_addr; }
  /// \~Chinese UV分量数据device侧地址
  uint8_t *PhyDataUV() override { return phy_uv_data_addr; }
  /// \~Chinese 图片大小
  uint32_t DataSize() override { return data_size; }
  /// \~Chinese UV分量大小
  uint32_t DataUVSize() override { return data_uv_size; }
  /// \~Chinese 宽度
  uint32_t Width() override { return width; }
  /// \~Chinese 高度
  uint32_t Height() override { return height; }
  /// \~Chinese 长度
  uint32_t Stride() override { return stride; }
  /// \~Chinese 维度
  uint32_t Channel() override { return channel; }
  /// \~Chinese uv长度
  uint32_t StrideUV() override { return stride_uv; }

  uint8_t *custom_data_addr = nullptr;
  uint8_t *virt_data_addr = nullptr;
  uint8_t *virt_uv_data_addr = nullptr;
  uint8_t *phy_data_addr = nullptr;
  uint8_t *phy_uv_data_addr = nullptr;
  uint32_t data_size = 0;
  uint32_t data_uv_size = 0;
  uint32_t width = 0;
  uint32_t height = 0;
  uint32_t stride = 0;
  uint32_t channel = 0;
  uint32_t stride_uv = 0;
  DataBufferPtr DataBuffer_ = nullptr;
};

/**
 * @brief pixel format conversion to/from string name
 *
 * @param format [in/out]
 * @param name [out/in]
 * @param to_str [in] true: convert to string; false convert to format from
 * string
 * @return int 0 when success
 */
inline int StrCvtPixelFormat(PGPixelFormat &format, std::string &name,
                             bool to_str) {
  static std::unordered_map<std::string, PGPixelFormat> name_pixel_format_map =
      {
          {"none", kPGPixelFormatNone},        {"rgb", kPGPixelFormatRawRGB},
          {"rgb565", kPGPixelFormatRawRGB565}, {"bgr", kPGPixelFormatRawBGR},
          {"gray", kPGPixelFormatRawGRAY},     {"nv21", kPGPixelFormatRawNV21},
          {"nv12", kPGPixelFormatRawNV12},     {"i420", kPGPixelFormatRawI420},
          {"yv12", kPGPixelFormatRawYV12},     {"rgba", kPGPixelFormatRawRGBA},
          {"bgra", kPGPixelFormatRawBGRA},     {"abgr", kPGPixelFormatRawARGB},
          {"argb", kPGPixelFormatRawABGR},     {"uint8", kPGPixelFormatUint8},
          {"fp32", kPGPixelFormatFloat32},     {"yuyv", kPGPixelFormatYUYV},
          {"uyvy", kPGPixelFormatUYVY},        {"yuv444", kPGPixelFormatYUV444},
          {"h264", kPGPixelFormatH264},        {"h265", kPGPixelFormatH265},
          {"int8", kPGPixelFormatInt8},        {"int16", kPGPixelFormatInt16},
          {"int32", kPGPixelFormatInt32},      {"int64", kPGPixelFormatInt64},
      };
  if (to_str) {
    name = "";
    for (const auto &it : name_pixel_format_map) {
      if (it.second == format) {
        name = it.first;
      }
    }
  } else {
    auto it = name_pixel_format_map.find(name);
    if (it != name_pixel_format_map.end()) {
      format = it->second;
    } else {
      return -1;
    }
  }
  return 0;
}

}  // namespace vision
}  // namespace phigent

#endif  // VISION_TYPE_BASE_TYPE_HPP_
