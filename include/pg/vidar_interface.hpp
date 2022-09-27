
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

struct VidarData {
  std::vector<phigent::vision::ImageFramePtr> images;
};

class VidarInterface {
 public:
  VidarInterface() = default;
  virtual ~VidarInterface() = default;

  static VidarInterface *Create(const string &type = "default");

  virtual int Init(const string &conf_json) = 0;

  virtual int Deinit() = 0;

  virtual int RecvData(VidarData *data, int timeout_ms) = 0;

  virtual int UpdateConfig(const std::string &conf_json) = 0;

  virtual int GetConfig(const std::string &conf_json, char **ret_conf_json) = 0;
};
}  // namespace vidar

}  // namespace pg

#endif  // VIDAR_INTERFACE_HPP_