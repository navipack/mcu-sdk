/**
******************************************************************************
* @file    navipack_transport_layer.c
* @author  Jalon
* @date    2016.02.01
* @brief   通讯协议传输层解包封包等相关函数
* @attention Copyright (C) 2016 Inmotion
******************************************************************************
*/
#include "navipack_transport_layer.h"

//传输层协议标志
#define FRAMECTRL 0xA5
#define FRAMEHEAD 0xAA
#define FRAMETAIL 0x55

/**
* @brief  解包函数
* @param  pframe : 帧对象
* @param  data   : 接收的数据，单 byte
* @retval 是否成功解包
*/
bool Navipack_TransportUnpacking(TransportFrame_Type *pframe, u8 data)
{
    if( (pframe->offset >= pframe->size) //当接收的数据长度超过接收SIZE
        || ((data == FRAMEHEAD) && (pframe->lastByte == FRAMEHEAD)) )
    {
        //复位
        pframe->offset = 0;
        pframe->recvFlag = true;
        pframe->checkSum = 0;
        return false;
    }

    if( (data == FRAMETAIL) && (pframe->lastByte == FRAMETAIL) && (pframe->recvFlag) )
    { 
        //收到结束符
        pframe->offset -= 2;
        pframe->checkSum -= (FRAMETAIL + pframe->buffer[pframe->offset]);

        if(pframe->checkSum == pframe->buffer[pframe->offset])
        {                                       
            pframe->recvFlag = false;
            return true;
        }
        else
        {
            pframe->offset = 0;
            pframe->checkSum = 0;
            pframe->recvFlag = false;
            return false;
        }
    }

    if(pframe->recvFlag)
    {
        if(pframe->ctrlFlag)
        {
            if( (data == FRAMEHEAD) || (data == FRAMETAIL) || (data == FRAMECTRL) )
            {
                pframe->buffer[pframe->offset++] = data;
                pframe->ctrlFlag = false;
                pframe->checkSum += data;
                data = FRAMECTRL;
            }
            else
            {
                pframe->offset = 0;
                pframe->checkSum = 0;
                pframe->recvFlag = false;
            }
        }
        else
        {
            if(data == FRAMECTRL)
            {
                pframe->ctrlFlag = true;
            }
            else
            {
                pframe->buffer[pframe->offset++] = data;
                pframe->checkSum += data;
            }
        }
    }

    pframe->lastByte = data;

    return false;
}

/**
* @brief  打包函数
* @param  pframe    : 帧对象
* @param  in_buf    : 打包数据指针
* @param  len       : 打包数据长度
* @param  pack_flag : 打包模式 @ref PACK_FLAG_define 按 bit 设置
* @retval 打包错误则返回 false
*/
bool Navipack_TransportPacking(TransportFrame_Type *pframe, u8 *in_buf, u16 len, u8 pack_flag)
{
    u16 i;
    
    if((pack_flag & PACK_FLAG_BEGIN) != 0)
    {
        pframe->offset = 0;
        pframe->checkSum = 0;
        
        if(len > pframe->size - 5)      //当发送的数据长度超过发送SIZE - 包头字节数 - 包尾字节数 - 校验位字节数
            return false;
        
        pframe->buffer[pframe->offset++] = FRAMEHEAD;      //加头
        pframe->buffer[pframe->offset++] = FRAMEHEAD;
        
    }
    else
    {
        if(len + pframe->offset > pframe->size - 3)      //当发送的数据长度超过发送SIZE - 包尾字节数 - 校验位字节数
            return false;
    }

    for (i = 0; i < len; i++)
    {
        if( (*in_buf == FRAMECTRL) || (*in_buf == FRAMEHEAD)|| (*in_buf == FRAMETAIL) )
        {
            pframe->buffer[pframe->offset++] = FRAMECTRL;
            if((len - i) + pframe->offset > pframe->size - 3)    //溢出判断
                return false;
        }
        pframe->buffer[pframe->offset++] = *in_buf;
        pframe->checkSum += *in_buf;
        in_buf++;
    }

    if((pack_flag & PACK_FLAG_END) != 0)
    {
        //校验和
        if ( (pframe->checkSum == FRAMECTRL) || (pframe->checkSum == FRAMEHEAD) || (pframe->checkSum == FRAMETAIL) )
        {
            pframe->buffer[pframe->offset++] = FRAMECTRL;
            if(pframe->offset > pframe->size - 3)    //溢出判断
                return false;
        }
        pframe->buffer[pframe->offset++] = pframe->checkSum;

        pframe->buffer[pframe->offset++] = FRAMETAIL;      //加尾
        pframe->buffer[pframe->offset++] = FRAMETAIL;
    }

    return true;
}
