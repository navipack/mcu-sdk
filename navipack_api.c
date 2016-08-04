/**
******************************************************************************
* @file    navipack_api.c
* @author  Jalon
* @date    2016.07.01
* @brief   通讯协议接口，该文件在移植过程中需要根据实际情况更改，
           部分函数需要用户实现
* @attention Copyright (C) 2016 Inmotion
******************************************************************************
*/
#include "navipack_api.h"
#include "navipack_session_layer.h"

NaviPack_GlobalType NaviPack_Global;

/**
* @brief  初始化
* @param  None
* @retval None
*/
bool NaviPack_Init(void)
{
    // TODO: 用户可添加初始化处理
    return true;
}

/**
* @brief  通讯接收数据解析函数
* @param  head : 数据指针
* @param  len  : 数据长度
* @retval None
*/
static void RxProcessor(NavipackComm_Type *comm, NaviPack_HeadType *head, u16 len)
{
    if(head->deviceAddr == NAVIPACK_SLAVE_ID)
    {
        if(head->len != len - sizeof(NaviPack_HeadType))
        {
            return;
        }

        // TODO: 用户可添加收到寄存器操作时的自定义处理
        switch(head->functionCode)
        {
        case FUNC_ID_READ_STATUS:
        case FUNC_ID_READ_CONTROL:
        case FUNC_ID_READ_CONFIG:
            // TODO: 如果发送和接收不是同一个线程，则应改成通知发送线程调用该发送函数
            NaviPack_TxProcessor(comm, head);
            break;
        case FUNC_ID_WRITE_CONTROL:
            if(RegisterWrite(head, (u8*)&NaviPack_Global.control, sizeof(NaviPack_Global.control), REG_ID_COTROL))
            {
                // TODO: 获得接收到的新值
                // NaviPack_Global.control.lineVelocity
                // NaviPack_Global.control.angularVelocity
            }
            break;
        }
    }
}

/**
* @brief  通讯接收数据处理函数
* @param  comm : 通讯对象
* @param  data : 接收数据，单 byte
* @retval 是否成功处理了数据包
*/
bool NaviPack_RxProcessor(NavipackComm_Type *comm, u8 data)
{
    comm->rxFrame.buffer = comm->rxBuffer;
    comm->rxFrame.size = comm->rxSize;
    if(Navipack_TransportUnpacking(&comm->rxFrame, data))
    {
        //解包处理成功
        comm->rxDataLen = comm->rxFrame.offset;
        
        RxProcessor(comm, (NaviPack_HeadType*)comm->rxBuffer, comm->rxDataLen);
        true;
    }
    return false;
}

/**
* @brief  通讯发送数据处理函数
* @param  comm : 通讯对象
* @param  head : 接收数据，单 byte
* @retval 是否成功处理了数据包
*/
bool NaviPack_TxProcessor(NavipackComm_Type *comm, NaviPack_HeadType *head)
{
    // TODO: 用户可添加寄存器发送时的自定义处理
    switch(head->functionCode)
    {
    case FUNC_ID_READ_STATUS:
        RegisterRead(comm, head, 0, (u8*)&NaviPack_Global.status, sizeof(NaviPack_Global.status), REG_ID_STATUS);
        break;
    case FUNC_ID_READ_CONTROL:
        RegisterRead(comm, head, 0, (u8*)&NaviPack_Global.control, sizeof(NaviPack_Global.control), REG_ID_COTROL);
        break;
    case FUNC_ID_READ_CONFIG:
        RegisterRead(comm, head, 0, (u8*)&NaviPack_Global.config, sizeof(NaviPack_Global.config), REG_ID_CONFIG);
        break;
    case FUNC_ID_WRITE_CONTROL:
        // ACK 暂无
        break;
    }
    
    return true;
}

/**
* @brief  实际发送数据的函数
* @param  pbuf : 数据指针
* @param  len  : 数据长度
* @retval None
*/
void Navipack_SendData(u8* pbuf, u16 len)
{
    // TODO: 用户添加数据发送函数
}

/**
* @brief  寄存器操作上锁
* @param  reg_id : 寄存器 id，指示需要上锁的寄存器
* @retval 是否成功上锁
*/
bool Navipack_LockReg(u8 reg_id)
{
    // TODO: 上锁处理，若无多线程操作可留空
    switch(reg_id)
    {
    case REG_ID_STATUS:
        break;
    case REG_ID_COTROL:
        break;
    case REG_ID_CONFIG:
        break;
    default:
        return false;
    }
    
    return true;
}

/**
* @brief  寄存器操作解锁
* @param  reg_id : 寄存器 id，指示需要解锁的寄存器
* @retval None
*/
void Navipack_UnlockReg(u8 reg_id)
{
    // TODO: 解锁处理，若无多线程操作可留空
    switch(reg_id)
    {
    case REG_ID_STATUS:
        break;
    case REG_ID_COTROL:
        break;
    case REG_ID_CONFIG:
        break;
    default:
        break;
    }
}
