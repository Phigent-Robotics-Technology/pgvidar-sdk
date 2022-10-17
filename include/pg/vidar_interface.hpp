
/**
 * @file vidar_interface.hpp
 * @author Jet.SUN (shuhuan.sun@phigent.ai)
 * @brief
 * @version 0.1
 * @date 2022-08-20
 *
 * @copyright Copyright (c) 2020
 *
 */

#ifndef VIDAR_INTERFACE_HPP_
#define VIDAR_INTERFACE_HPP_

#include <string>
#include <vector>

#include "vision_type/base_type.hpp"

namespace pg {
namespace vidar {
using std::string;
/***
共享指针的数据结构，用来存放获取到的图片
***/
struct VidarData {
  std::vector<phigent::vision::ImageFramePtr> images;
  std::vector<std::string> imu;
};

class VidarInterface {
 public:
  /**
   * @brief  创建双目视觉雷达数据界面
   * @note
   * @param
   * @retval
   */
  VidarInterface() = default;
  virtual ~VidarInterface() = default;
  /**
   * @brief  创建双目视觉雷达数据界面
   * @note
   * @param  type
   * @retval
   */
  static VidarInterface *Create(const string &type = "default");
  /**
   * @brief  初始化双目视觉雷达数据界面
   * @note
   * @param  conf_json 用于配置句柄的json字符串
   * {
   *    "IComConfig": {
   *        "type": 0,
   *        "logic_channel_id": 0,
   *        "usb_host": {
   *            "dev_vendor_id": 7531,
   *            "dev_product_id": 261
   *        }
   *    },
   *    "port": 50051,
   *    "SourceVideoStream":{
   *        "channel_id" : [0, 1]
   *    }
   * }
   * @retval
   */
  virtual int Init(const string &conf_json) = 0;
  /**
   * @brief  反初始化
   * @note
   * @param
   * @retval 0 成功，否则为错误码
   */
  virtual int Deinit() = 0;
  /**
   * @brief  接收视觉雷达结果
   * @note
   * @param  *data: 雷达结果
   * @param  timeout_ms: block until timeout_ms ms 阻塞流程等待时间
   * @retval 0 成功，否则为错误码
   */
  virtual int RecvData(VidarData *data, int timeout_ms) = 0;
  /**
   * @brief 使用json字符串更新vidar config
   * @note
   * @param conf_json: [in] 配置json字符串
   * @return 0 成功，否则为错误码
   */
  virtual int UpdateConfig(const std::string &conf_json) = 0;
  /**
   * @brief
   * 根据conf_json内容获取配置信息并且跟进配置文件激活输出未校正或者矫正后图片的板端服务流程
   * @note   * @param conf_json: [in] 请求json字符串，里面包含请求config的key
   * @param return_conf_json: [out]
   * 返回的config字符串,需要在该接口调用前初始化，格式模板如下
   * {
   *    "camera" :
   *    {
   *            "calib" : "%YAML:1.0",
   *            "product_number" : "P06002M160-B0X",
   *            "serial_number" : "P03002360000X",
   *            "shift" : -1,
   *            "version" : "0.0.X"
   *    }
   * }
   * @return int  激活板端服务成功并且获取相机信息时返回 0，
   * 激活板端服务成功但是没有获取相机信息时返回 1，
   * 激活板端服务成功但是没有获取相机信息时返回 2，完全失败返回-1
   * 返回码为1时，板端服务激活，但是相机信息获取失败，此时重新调用GetConfig即可获取相机信息
   * 返回码为2时，板端服务已经激活，相机信息获取成功，此时如果想重新选择板端服务类型需要重启板端程序
   * 返回码为-1时，请检查usb链接
   */
  virtual int GetConfig(const std::string &conf_json,
                        std::string &return_conf_json) = 0;
};
}  // namespace vidar

}  // namespace pg

#endif  // VIDAR_INTERFACE_HPP_
