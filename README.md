# TTZN SDK

[English Version](./README-en.md)

## 简介
该软件包提供了一个 C++ 接口，用于与移动平台通信，向机器人发送命令并接收最新的机器人状态。该仓库是威海天特智能科技有限公司开发团队的共同努力成果。


如果在使用该仓库当中出现问题，请发布 issue @ ["https://github.com/ttzntech/ttzn_sdk/issues"](https://github.com/ttzntech/ttzn_sdk/issues) 。

## 支持的平台

### 软件环境
- 架构：x86_64/arm64
- 操作系统：Ubuntu 20.04/22.04
- ROS 版本：Noetic（ROS1）/Humble（ROS2）

该项目也应该可以在其他类似的 Linux 环境中工作，但只有上面列出的环境做过测试。

### 支持的底盘及通讯方式

|      底盘型号      |  USB串口转CAN  |  CANable（USB转CAN）  | 是否维护 |
| :---------------: |  :----------: | :------------------: | :-----: |
|     Cody          |    &#10004;   |       &#10008;       | &#10004;|

## 安装底盘驱动

### 1. 直接安装编译的安装包
在 [Releases](https://github.com/ttzntech/ttzn_sdk/releases) 中下载最新的 deb 安装包。
```bash
sudo dpkg -i ttzn_sdk_<version>_<arch>.deb
```

> NOTE：该方式目前仅编译了 x86_64 架构的安装包。若你的机器是其他架构（如 arm 等）的请参考从[项目源码编译](#2-从项目源码编译)并安装。

### 2. 从项目源码编译

#### 安装依赖
```bash
sudo apt-get update
sudo apt-get install build-essential git cmake
```
#### 编译
```bash
git clone https://github.com/ttzntech/ttzn_sdk.git
cd ttzn_sdk
mkdir -p build && cd build

# 下面语句选择一条执行即可
# 编译 Demo 程序
cmake .. -DCODY_DEMO=ON
# 不编译 Demo 程序
cmake ..

make -j8
```

#### 安装
```bash
sudo make install
```

默认安装文件和对应路径
```bash
/usr/local/bin/ttzn_setup_dev
/usr/local/include/ttzn_sdk/
/usr/local/lib/libttzn_sdk.a
/usr/local/share/cmake/ttzn_sdk/
```
如果你需要删除该驱动，把上述文件删去即可。

## CAN 设备初始化

运行脚本 `ttzn_setup_dev` 根据提示即可初始化设备。 

目前支持 USB-TTL-CAN 设备的初始化。
```bash
ttzn_setup_dev

> Please select setup device type (default: 1)
> 1: USB TTY CAN
> To Be Done
...
```

## 如何使用该驱动

该项目会输出一个 CMake package，使用CMake `find_package(ttzn_sdk)` 即可引入该项目。

具体细节请参考 [demo](./demo) 子目录。

---
- Copyright &copy; 2023 [威海天特智能科技有限公司](http://ttzntech.com/)