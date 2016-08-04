/**
******************************************************************************
* @file    navipack_api.h
* @author  Jalon
* @date    2016.07.01
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

typedef struct
{
    u8 *rxBuffer;
    u8 *txBuffer;
    u16 rxDataLen;
    u16 txDataLen;
    u16 rxSize;
    u16 txSize;
    TransportFrame_Type rxFrame;
    TransportFrame_Type txFrame;
}NavipackComm_Type;

#pragma pack(push, 1)

typedef struct{
    NaviPack_CtrlType control;
    NaviPack_StatusType status;
    NaviPack_ConfigType config;
}NaviPack_GlobalType;

#pragma pack(pop)

extern NaviPack_GlobalType NaviPack_Global;

bool NaviPack_Init(void);
bool NaviPack_TxProcessor(NavipackComm_Type *comm, NaviPack_HeadType *head);
bool NaviPack_RxProcessor(NavipackComm_Type *comm, u8 data);
bool Navipack_LockReg(u8 reg_id);
void Navipack_UnlockReg(u8 reg_id);
void Navipack_SendData(u8* pbuf, u16 len);

#endif
