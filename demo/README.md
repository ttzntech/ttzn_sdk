# TTZN SDK DEMO

[English Version](./README-en.md)

## 编译 demo 子目录示例程序
首先，确保 cd 到 demo 目录。
```bash
# 编译程序
mkdir -p build && cd build
cmake ..
make -j8

# 运行程序
./cody_demo
```

## 如何使用该项目

### 1. 在 `CMakeLists.txt` 
```cmake
...
# 查找 ttzn_sdk package
find_package(ttzn_sdk REQUIRED)

add_executable(<your_target> <your_source_files>)

# 在需要使用的 target 中 link ttzn_sdk
target_link_libraries(<your_target> ttzn_sdk)
...
```
详细细节请查阅 [CMakeLists.txt](./CMakeLists.txt)
### 2. 在相应的 c++ 程序中
以下为 cody 底盘对应的程序，其他底盘类似。

```cpp
#include <stdio.h>

#include "ttzn_sdk/cody/tran.hpp"


int main() {
    /* 对底盘的控制和接收都封装在该类内部 */
    cody::CanTran can_tran("/dev/ttyUSB0");

    /* 发送数据 */
    can_tran.data.i111MoveCtrl.speed = 0.3;
    can_tran.data.i111MoveCtrl.corner = 12;
    can_tran.send(ID_MoveCtrl);

    /* 接受数据 */
    can_tran.recv(ID_MoveCtrlFb);
    printf("0x221 MoveCtrlFb speed: %.2lf corner: %.2lf \n",
        can_tran.data.i221MoveCtrlFb.speed,
        can_tran.data.i221MoveCtrlFb.corner
    );

    return 0;
}
```
只需要实例化出 `cody::CanTran` 对象，即可对底盘进行控制。在 ROS 中也可参考该程序。

详细细节请查阅 [cody_demo.cpp](./cody_demo.cpp)