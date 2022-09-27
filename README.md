# pgvidar-sdk

## 简介

pgvidar-sdk是由鉴智机器人设计并开发的一款配合视觉雷达板端程序使用的软件开发工具包，主要包括，支持获取鉴智海东青和星尘等模组校准后的彩色/黑白图像数据，视差数据等算法结果，和双目相机模组标定数据等硬件信息。

## 安装使用步骤

### 准备代码

`git clone https://github.com/Phigent-Robotics-Technology/pgvidar-sdk.git`

`cd pgvidar-sdk`

`tar -xf lib.tgz`

### 编译example

`cd pgvidar2-example`

`mkdir build && cd build && cmake .. && make && cd ..`

### 运行example

`sudo LD_LIBRARY_PATH=$LD_LIBRARY_PATH:../lib ./build/vidar2_test config/vidar2_cfg.json`