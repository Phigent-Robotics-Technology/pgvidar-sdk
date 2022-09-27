/**
 * @author Jet.Sun (shuhuan.sun)
 * @date 2021-09-15
 *
 * @copyright Copyright (c) 2021 PhiGent
 *
 */

#ifndef VISION_TYPE_C_VISION_TYPE_COMMON_H_
#define VISION_TYPE_C_VISION_TYPE_COMMON_H_
#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include <stdint.h>
/**
 * \~Chinese @brief 图片编码方式
 */
typedef enum PGPixelFormat_ {
  /// \~Chinese 普通图片文件的二进制流，默认BGR,
  /// 需要与二进制流文件长度一起配套使用
  kPGPixelFormatNone = 0,
  /// RGB
  kPGPixelFormatRawRGB,
  /// RGB565
  kPGPixelFormatRawRGB565,
  /// BGR
  kPGPixelFormatRawBGR,
  /// \~Chinese 单通道灰度图
  kPGPixelFormatRawGRAY,
  /// YUV420SP:NV21
  kPGPixelFormatRawNV21,
  /// YUV420SP:NV12
  kPGPixelFormatRawNV12,
  /// YUV420P:I420
  kPGPixelFormatRawI420,
  /// YUV420P:YV12
  kPGPixelFormatRawYV12,
  /// \~Chinese 图片标准格式 ，比如jpeg
  kPGPixelFormatImageContainer,
  /// RGBA
  kPGPixelFormatRawRGBA,
  /// BGRA
  kPGPixelFormatRawBGRA,
  /// ARGB
  kPGPixelFormatRawARGB,
  /// ABGR
  kPGPixelFormatRawABGR,
  // Multichannel picture
  kPGPixelFormatUint8,
  kPGPixelFormatFloat32,
  kPGPixelFormatYUYV,
  kPGPixelFormatUYVY,
  kPGPixelFormatYUV444,
  kPGPixelFormatH264,
  kPGPixelFormatH265,
  kPGPixelFormatInt8,
  kPGPixelFormatInt16,
  kPGPixelFormatInt32,
  kPGPixelFormatInt64,
} PGPixelFormat;

/**
 * \~Chinese @brief 朝向
 */
typedef enum PGOrientation_ {
  /// \~Chinese 未知
  Unknown = 0,
  /// \~Chinese 前
  Front = 1,
  /// \~Chinese 后
  Back = 2,
  /// \~Chinese 左
  Left = 3,
  /// \~Chinese 右
  Right = 4
} PGOrientation;

#ifdef __cplusplus
}
#endif
#endif  // VISION_TYPE_C_VISION_TYPE_COMMON_H_
