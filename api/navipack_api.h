/**
******************************************************************************
* @file    navipack_api.h
* @author  Jalon
* @date    2016.02.01
* @brief   通讯协议相关声明
* @attention Copyright (C) 2016 Inmotion
******************************************************************************
*/
#ifndef  __NAVIPACK_API_H__
#define  __NAVIPACK_API_H__

#include "navipack_type.h"

#define CHASSIS_PLATFORM
#include "navipack_protocol.h"
#include "navipack_transport_layer.h"

#define REG_ID_STATUS   0
#define REG_ID_COTROL   1
#define REG_ID_CONFIG   2
#define REG_ID_USER     3

typedef struct
{
    // 需要初始化
    u8 *rxBuffer;   ///< 接收 Buffer 指针
    u8 *txBuffer;   ///< 发送 Buffer 指针
    u16 rxSize;     ///< 接收 Buffer 尺寸
    u16 txSize;     ///< 发送 Buffer 尺寸
    
    u16 rxDataLen;
    u16 txDataLen;
    TransportFrame_Type rxFrame;
    TransportFrame_Type txFrame;
    
    NaviPack_CtrlType control;  ///< 控制寄存器
    NaviPack_StatusType status; ///< 状态寄存器
    NaviPack_ConfigType config; ///< 参数寄存器
}NavipackComm_Type;

bool NaviPack_Init(void);
bool NaviPack_TxProcessor(NavipackComm_Type *comm, NaviPack_HeadType *head);
bool NaviPack_RxProcessor(NavipackComm_Type *comm, u8 data);
bool Navipack_LockReg(u8 reg_id);
void Navipack_UnlockReg(u8 reg_id);
void Navipack_SendData(u8* pbuf, u16 len);

#endif
