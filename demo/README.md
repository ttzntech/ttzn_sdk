# TTZN SDK DEMO

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
### 2. 在相应的 c++ 程序中
以下为 cody 底盘对应的程序，其他底盘类似。

```cpp
#include "ttzn_sdk/cody/tran.hpp"

int main() {
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