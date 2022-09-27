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

#ifndef PG_VIDAR_API_API_H_
#define PG_VIDAR_API_API_H_

#include "pg/vidar_type.h"

#ifdef __cplusplus
extern "C" {
#endif

// 句柄
typedef void *pgVidarHandle;
/**
 * @brief  创建句柄
 * @note
 * @param  pHandle: 句柄指针，创建的句柄会被保存到改指针内
 * @retval 0 成功，否则为错误码
 */
int pgVidarCreate(pgVidarHandle *pHandle);
/**
 * @brief  初始化
 * @note
 * @param  pHandle: 要初始化的句柄
 * @param  *conf_json: 用于配置句柄的json字符串
 * {
 *  "server": "192.168.1.18",
 *  "port": 80800,
 *  "recv_buffer_num_max": 2,
 *  "rgb_sync_wait_depth_frame_max": 10,
 *  "output_type": "deep_pic", // string deep_pic/point_cloud
 *  "request_datas": [
 *     "left_origin", "right_origin", "left_remaped", "right_remaped", "disparity", "imu"
 *   ]
 * }
 * @retval 0 成功，否则为错误码
 */
int pgVidarInit(pgVidarHandle pHandle, const char *conf_json);
/**
 * @brief  反初始化
 * @note
 * @param  pHandle: 需要结束的句柄
 * @retval 0 成功，否则为错误码
 */
int pgVidarDeinit(pgVidarHandle pHandle);
/**
 * @brief  关闭句柄，释放相关资源
 * @note
 * @param  pHandle: 要释放的句柄
 * @retval 0 成功，否则为错误码
 */
int pgVidarRelease(pgVidarHandle pHandle);
/**
 * @brief  接收视觉雷达结果
 * @note
 * @param  pHandle: 操作句柄
 * @param  *data: 雷达结果
 * @param  timeout_ms: block until timeout_ms ms
 * @retval 0 成功，否则为错误码
 */
int pgVidarRecvData(pgVidarHandle pHandle, pgVidarRawData *data,
                     int timeout_ms);
/**
 * @brief  将雷达结果转换为点云
 * @note
 * @param pHandle: [in] 句柄，内部将会从该句柄获取转换矩阵
 * @param  *data: 原始雷达结果
 * @param  *pc_data:
 * 点云结构的指针，该结构体是用户申请内存，SDK会根据它内部的数据指针判断是否创建数据
 *                如果pc_data->points
 * 为null，那么SDK将申请需要的内存，否则SDK会根据pc_data->points_num判断小于需求大小才会释放并重新申请。
 * @retval
 */
int pgVidarConvert2PointCloud(pgVidarHandle pHandle, pgVidarRawData *data,
                               pgVidarPointCloud *pc_data);
/**
 * @brief 使用json字符串更新vidar config
 *
 * @param pHandle: [in] 句柄
 * @param conf_json: [in] 配置json字符串
 *
 * @return 0 成功，否则为错误码
 */
int pgVidarUpdateConfig(pgVidarHandle pHandle, const char *conf_json);

/**
 * @brief 根据req_json内容获取配置信息
 *
 * @param pHandle: [in] 句柄
 * @param req_json: [in] 请求json字符串，里面包含请求config的key
 * @param ret_conf_json: [out] 返回的config json字符串指针，释放请调用free(ret_conf_json)
 * @return int
 */
int pgVidarGetConfig(pgVidarHandle pHandle, const char *req_json, char **ret_conf_json);

int pgVidarClearRawData(pgVidarRawData *data);

int pgVidarClearPointCloud(pgVidarPointCloud *pc_data);

/**
 * @brief  获取字符串格式的矫正配置文件
 * @note
 *
 * @param pHandle: [in] 句柄
 * @param ret_calib_str: [out] 含有矫正矩阵的字符串，用完请调用free()释放
 */
int pgVidarGetCalibConfig(pgVidarHandle pHandle, char **ret_calib_str);

#ifdef __cplusplus
}
#endif

#endif  // PG_VIDAR_API_API_H_