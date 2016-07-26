// 调用例程
// 必须在工程中定义宏 CHASSIS_PLATFORM

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
        Recv();
        
        // 给状态寄存器赋值
        NaviPack_Global.status.lineVelocity = v;        
        // 10 ms 调用一次，发送状态寄存器的值
        Boost();
    }
}
