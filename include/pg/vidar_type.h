/**
 * @file vidar_api.h
 * @author Jet.SUN (shuhuan.sun@phigent.ai)
 * @brief
 * @version 0.1
 * @date 2021-11-09
 *
 * @copyright Copyright (c) 2021
 *
 */

#ifndef PG_VIDAR_TYPE_API_H_
#define PG_VIDAR_TYPE_API_H_

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define PG_MAX_COLOR_CHANNEL 4

#define PG_ERROR_CODE_OK 0
#define PG_ERROR_CODE_ERROR -1
#define PG_ERROR_CODE_INVALID_PARAM -2
#define PG_ERROR_CODE_ALREADY_INIT -3
#define PG_ERROR_CODE_NOT_INIT -4
#define PG_ERROR_CODE_DISCONNECTED -5
#define PG_ERROR_CODE_NO_IMPL -6

/**
 * @brief  张量数据
 * @note
 */
typedef struct {
  /// 0-int8, 1-int16, 2-float32, 3-float64, 4-int32, 5-int64
  int type;
  /// 数据指针，目前仅支持hwc格式
  void *data;
  /// 数据数组长度，数组类型根据上面type来决定
  size_t data_size;
  /// 数据buffer的大小，单位bytes
  size_t buffer_size;
  /// 宽
  int width;
  /// 高
  int height;
  /// 通道数量
  int channel;
  /// 每个元素需乘以scale才是真正的元素数值
  float scale;
} pgVidarTensor;

/**
 * @brief  视频帧
 * @note
 */
typedef struct {
  /// 0-h264, 1-h265, 2-jpg, 10-yuv444, 11-rgb
  int type;
  /// 数据指针
  uint8_t *data;
  /// 数据指针指向的数组长度，当前仅支持的这几种格式单位都是bytes
  size_t data_size;
  /// 数据buffer大小，单位bytes
  size_t buffer_size;
  /// 宽
  int width;
  /// 高
  int height;
  /// 通道数
  int channel;
  /// 数据帧同步的车身信息
  uint8_t *chassis_content;
} pgVidarVideoPacket;

/**
 * @brief  雷达点
 * @note
 */
typedef struct {
  /// 点x坐标
  float x;
  /// 点y坐标
  float y;
  /// 点z坐标
  float z;
  /// 彩色图像像素值RGBA，或者YUV444，
  uint8_t color[PG_MAX_COLOR_CHANNEL];
} pgVidarPoint;

/**
 * @brief 雷达点云数据结构
 * @note
 */
typedef struct {
  /// 彩色图像的像素通道数目
  int color_channel;
  /// 彩色图像像素格式，0-RGB，1-YUV444, 2-Y
  int color_type;
  /// 点云数据
  pgVidarPoint *points;
  /// 点云数据中点的数量
  size_t points_num;
} pgVidarPointCloud;

// IMU related
typedef struct {
  float x;
  float y;
  float z;
} pgXYZ;
typedef struct {
  float x;
  float y;
  float z;
  float w;
} pgOrientation;
typedef struct {
  pgOrientation orientation;  //< 姿态
  float orientation_covariance[9];  //< 姿态协方差
  pgXYZ angular_velocity;  //< 角速度
  float angular_velocity_covariance[9];  //< 姿态协方差
  pgXYZ linear_acceleration;  //< 加速度
  float linear_acceleration_covariance[9];  //< 加速度协方差
  uint64_t timestamp;
} pgVidarImuData;

/**
 * @brief  Vidar原始数据
 * @note
 */
typedef struct {
  /// 原始数据的时间戳
  uint64_t timestamp;
  /** 状态码，用来指示当前帧的处理状态
  - 0, video-deep_pic synced
  - 1, video waits deep_pic timeout
  - 2, video without deep_pic
  - 3, deep_pic without matching video frame
  - 4, video+deep_pic droped
  - 5, video droped
  - 6, deep_pic droped
  */
  int state_code;
  /// 视频帧
  pgVidarVideoPacket **video_packet_array;
  /// 视频帧通道ID的array，用以区分不同路视频流
  int *channel_id_array;
  /// 视频帧通道的总数
  int channel_sum;
  /// 深度图
  pgVidarTensor *deep_pic;
  /// 点云
  pgVidarPointCloud *point_cloud;
  /// 视差图
  pgVidarTensor *disp_pic;
  pgVidarImuData imu_data;
} pgVidarRawData;

#ifdef __cplusplus
}
#endif

#endif  // PG_VIDAR_TYPE_API_H_
