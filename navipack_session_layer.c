/**
******************************************************************************
* @file    navipack_session_layer.c
* @author  Jalon
* @date    2016.06.16
* @brief   通讯协议会话层解析相关函数
* @attention Copyright (C) 2016 Inmotion
******************************************************************************
*/
#include <string.h>
#include "navipack_session_layer.h"
#include "navipack_transport_layer.h"

/**
* @brief  执行写寄存器操作
* @param  head     : 数据头指针
* @param  reg      : 寄存器地址
* @param  reg_size : 寄存器尺寸
* @param  reg_id   : 寄存器 ID
* @retval 是否成功
*/
bool RegisterWrite(NaviPack_HeadType *head, u8 *reg, u32 reg_size, u8 reg_id)
{
    u8 err;
    
    if(head->startAddr >= reg_size || head->startAddr + head->len > reg_size)
    {
        return false;
    }
    
    if(Navipack_LockReg(reg_id))
    {
        memcpy(reg + head->startAddr, (u8*)(head + 1), head->len);
        Navipack_UnlockReg(reg_id);
        return true;
    }
    
    return false;
}

/**
* @brief  执行读寄存器操作
* @param  comm     : 通讯对象
* @param  head     : 数据头指针
* @param  reg      : 寄存器地址
* @param  reg_size : 寄存器尺寸
* @param  reg_id   : 寄存器 ID
* @retval 是否成功
*/
bool RegisterRead(NavipackComm_Type *comm, NaviPack_HeadType *head, u8 err_id, u8 *reg, u32 reg_size, u8 reg_id)
{
    bool ret;
    
    if(head->startAddr >= reg_size || head->startAddr + head->len > reg_size)
    {
        return false;
    }
    
    head->functionCode |= 0x80;
    
    if(Navipack_LockReg(reg_id))
    {
        comm->txFrame.buffer = comm->txBuffer;
        comm->txFrame.size = comm->txSize;
        ret = Navipack_TransportPacking(&comm->txFrame, (u8*)head, sizeof(NaviPack_HeadType), PACK_FLAG_BEGIN);
        if(!ret) return false;
        ret = Navipack_TransportPacking(&comm->txFrame, reg + head->startAddr, head->len, 0);
        if(!ret) return false;
        ret = Navipack_TransportPacking(&comm->txFrame, &err_id, 1, PACK_FLAG_END);
        if(!ret) return false;
        comm->txDataLen = comm->txFrame.offset;
        
        Navipack_UnlockReg(reg_id);
        
        Navipack_SendData(comm->txBuffer, comm->txDataLen);
        return true;
    }
    
    return false;
}
