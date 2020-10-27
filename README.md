# ModbusLCD1602

#### 介绍
把LCD1602做成Modbus模块

#### 软件架构
基于STM8S003，通过IIC驱动PCF8574控制LCD1602
通信通过485模块


#### 基本功能
1.  实现Modbus控制LCD显示内容，支持单字节0x06写入，多字节写入0x10，以及多字节0x03读取
2.  支持Modbus控制背光开关
3.  支持Modbus修改从机地址
#### 寄存器地址分配
1.   0x00-0x31 映射到LCD的32个字符
2.   0x32  : 0x00关背光，0x01开背光
3.   0x33  : Modbus地址


#### 特技

1.  使用 Readme\_XXX.md 来支持不同的语言，例如 Readme\_en.md, Readme\_zh.md
2.  Gitee 官方博客 [blog.gitee.com](https://blog.gitee.com)
3.  你可以 [https://gitee.com/explore](https://gitee.com/explore) 这个地址来了解 Gitee 上的优秀开源项目
4.  [GVP](https://gitee.com/gvp) 全称是 Gitee 最有价值开源项目，是综合评定出的优秀开源项目
5.  Gitee 官方提供的使用手册 [https://gitee.com/help](https://gitee.com/help)
6.  Gitee 封面人物是一档用来展示 Gitee 会员风采的栏目 [https://gitee.com/gitee-stars/](https://gitee.com/gitee-stars/)
