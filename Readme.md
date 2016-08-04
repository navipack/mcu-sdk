# 使用说明
## 移植
1. 将源码加入工程，将源码文件夹的路径添加到 include 头文件的路径。
2. 在 `navipack_type.h` 中添加缺少的数据类型，如 `bool`,`s32` 等，保证编译能够顺利通过。
3. 接收数据的移植需要在 `navipack_api.c` 中的 `RxProcessor()` 函数下的 `switch` 内添加用户自己需要的处理。
4. 发送数据的移植需要在 `navipack_api.c` 中的 `Navipack_SendData()` 函数下添加实际将数据发送出去的处理。

完成以上步骤并编译通过，则移植成功。

其它说明在 `navipack_api.c` 中用 `// TODO:` 注释给出了提示

## 调用
参考 `example.c` 中的调用例程。该文件中的代码只是范例，并不是可直接编译运行的代码，需要移植完成才能看到运行效果。
1. 为发送和接收分别准备 Buffer。
2. 定义 `NavipackComm_Type` 类型的变量，把 Buffer 指针分别赋值给该变量的 `rxBuffer` 和 `txBuffer` 成员。
3. 同时把两个 Buffer 的尺寸赋值给以上变量的 `rxSize` 和 `txSize` 成员。
4. 接收要逐个 byte 的调用 `NaviPack_RxProcessor()` 接口。
5. 发送要先定义一个 `NaviPack_HeadType` 类型的变量，填入对应的值，其中 `startAddr` 代表要发送的内容在寄存器中的偏移。
6. 然后向 `NaviPack_Global` 全局变量中填入对应的值，再调用 `NaviPack_TxProcessor()` 接口即可。