# Navipack MCU SDK
MCU 使用的 Navipack 通讯 SDK。

## Copyright &copy; 2016 Inmotion Corporation
该仓库所有文件为**深圳乐行天下科技有限公司**所有，未经许可不得用于商业用途。

# 使用说明
## 移植
1. 将源码加入工程，将 api，core 文件夹的路径添加到头文件 include 的路径。
2. 在 `navipack_type.h` 中添加缺少的数据类型，如 `bool`,`s32` 等，保证编译能够顺利通过。
3. 接收数据的移植需要在 `navipack_api.c` 中的 `Navipack_RxCallback()` 函数下的 `switch` 内添加用户自己需要的处理。
4. 发送数据的移植需要在 `navipack_api.c` 中的 `Navipack_TxCallback()` 函数下添加实际将数据发送出去的处理。

完成以上步骤并编译通过，则移植成功。

其它说明在 `navipack_api.c` 中用 `// TODO:` 注释给出了提示

## 调用
1. 为发送和接收分别准备 Buffer。
2. 定义 `NavipackComm_Type` 类型的全局变量，把 Buffer 指针分别赋值给该变量的 `rxBuffer` 和 `txBuffer` 成员。
3. 同时把两个 Buffer 的尺寸赋值给以上变量的 `rxSize` 和 `txSize` 成员。
4. 接收要逐个 byte 的调用 `NaviPack_RxProcessor()` 接口。
5. 发送要先用 `NaviPack_HeadType` 定义一个会话层数据头，并填入相应的信息，其中 `startAddr` 代表要发送的内容在寄存器中的偏移。
6. 然后向第 2 步定义的全局变量中的对应寄存器填入新值，再调用 `NaviPack_TxProcessor()` 发送即可。

详细参考 `example.c` 中的调用例程。

## 用户寄存器
用于收发用户自定义的数据。SDK 不关心数据内容，只起到一个透传的作用，用户要自己保证上层发送的数据类型和底层使用的相同。

SDK 在接收到通过正确的接口发送来的用户数据后，会在 `Navipack_RxCallback()` 函数的 `case FUNC_ID_WRITE_USER:` 处通过 `RegisterWrite()` 函数将数据原封不动的写到参数给定的地址处（例中 `UserReg` 类型用户自定）。然后用户可直接读取 `UserReg` 获得新收到的数据。

而 `NaviPack_TxProcessor()` 的 `case FUNC_ID_READ_USER:` 处的 `RegisterRead()`，则是将 head 指定的用户数据打包并发送出去。

注意：初始化时 `rxBuffer` 和 `txBuffer` 的尺寸要满足 `UserReg` 的尺寸需求，否则通讯会失败。参考 `example.c`

# 文件结构简介
    mcu-sdk
    │  Readme.md  说明
    │  example.c  sdk 接口调用示例
    │
    ├─api
    │      navipack_api.c  sdk 接口函数实现，一部分代码需要用户补充
    │      navipack_api.h  接口函数及数据类型的申明
    │      navipack_type.h  用户在该文件中申明 sdk 需要使用到的数据类型
    │
    └─core
           navipack_protocol.h  定义了通讯使用的结构体，上位机与下位机公用
           navipack_session_layer.c  会话层，寄存器读写相关函数
           navipack_session_layer.h  会话层相关申明
           navipack_transport_layer.c  传输层，解包打包相关函数
           navipack_transport_layer.h  传输层相关申明