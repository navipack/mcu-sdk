/**
******************************************************************************
* @file    example.c
* @author  Jalon
* @date    2016.07.01
* @brief   该代码只是调用通讯接口范例，并非直接可运行的程序
* @attention Copyright (C) 2016 Inmotion
******************************************************************************
*/

#include "navipack_api.h"

#define NAVI_MAX(a, b) (a>b?a:b)
#define NAVI_MAX_BLOCK (u32)(NAVI_MAX(NAVI_MAX( sizeof(NaviPack_CtrlType), sizeof(NaviPack_StatusType)), sizeof(NaviPack_ConfigType)))
#define NAVIPACK_COMM_SIZE (NAVI_MAX_BLOCK + sizeof(NaviPack_HeadType) + 1)

NavipackComm_Type Comm;

u8 RecvBuf[NAVIPACK_COMM_SIZE];
u8 SendBuf[NAVIPACK_COMM_SIZE*2+6];

void Boost(void)
{
    NaviPack_HeadType head = {
            NAVIPACK_SLAVE_ID,
            FUNC_ID_READ_STATUS,
            0,
            sizeof(NaviPack_StatusType),
        };
    
    if(NaviPack_TxProcessor(&Comm, head))
    {
        // 成功发送了指定的寄存器内容
    }
}

void Recv(void)
{
    u8* data;
    u32 len, i;
    
    len = RecvData(data); // 用户程序，从通讯接口接收数据
        
    for(i=0; i<len; i++)
    {
        // 逐个 byte 的调用该接口
        if(NaviPack_RxProcessor(&comm, data[i]))
        {
            //成功处理了一个寄存器操作
        }
    }
}

int main(void)
{
    // 初始化
    Comm.rxBuffer = RecvBuf;
    Comm.rxSize = sizeof(RecvBuf);    
    Comm.txBuffer = SendBuf;
    Comm.txSize = sizeof(SendBuf);
    
    NaviPack_Init();
    
    while(1)
    {
        // 接收处理根据用户的程序架构来决定调用位置，比如中断里调用
        Recv();
        
        // 调用发送函数前必须将新的值填入对应的变量中
        NaviPack_Global.status.lineVelocity = v;
        NaviPack_Global.status.angularVelocity = w;
        
        Boost(); // 推荐 10 ms 调用一次，持续向上位机发送状态寄存器的值
    }
}
