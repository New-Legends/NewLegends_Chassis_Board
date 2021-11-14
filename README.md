# NewLegends_Chassis_Board
<p>
用于宁波工程学院NewLegends战队RoboMaster2022赛季比赛车辆的底盘控制
</p>

## <div align="center">Documentation</div>

See the [Chassis_Board Docs](https://www.baidu.com).

## <div align="center">Quick Start</div>

<details open>
<summary>Environment</summary>
环境配置参考稚晖君的B站博客:(https://www.bilibili.com/read/cv6308000/)

仓库里的下载器配置文件.cfg为正点原子cmsis-dap无线下载器 后续会添加ST-Link的下载器配置文件
</details>

## <div align="center">Notes</div>
1.有文件更改(包括加入和删除)时记得重新载入Cmake配置
<details open>
<summary>2.C++文件调用C文件时在#include前后包括上extern "C"</summary>

例如:
```bash
#include "remote_data.h" //这是一个c语言程序
```
我们转化为
```bash
#ifdef __cplusplus             //告诉编译器，这部分代码按C语言的格式进行编译，而不是C++的
extern "C"{
#endif
#include "remote_data.h" //这是一个c语言程序
#ifdef __cplusplus
}
#endif
```
</details>



</details>
