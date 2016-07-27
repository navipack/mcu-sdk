/**
******************************************************************************
* @file    navipack_transport_layer.h
* @author  Jalon
* @date    2016.02.01
* @brief   通讯协议解包封包等相关声明
* @attention Copyright (C) 2016 Inmotion
******************************************************************************
*/
#ifndef  __NAVIPACK_TRANSPORT_LAYER_H__
#define  __NAVIPACK_TRANSPORT_LAYER_H__

#include "navipack_type.h"

/** @defgroup PACK_FLAG_define
  * @{
  */
#define PACK_FLAG_BEGIN     0x01
#define PACK_FLAG_END       0x02
/**
  * @}
  */ 

typedef struct
{
    bool recvFlag;		///< 接收标志
	bool ctrlFlag;		///< 转义符标志
	u8 offset;		    ///< 当前buf位置
	u8 lastByte;		///< 上次数据
	u8 checkSum;		///< 校验和
    u8* buffer;
    u16 size;
}TransportFrame_Type;

bool Navipack_TransportUnpacking(TransportFrame_Type *pframe, u8 data);
bool Navipack_TransportPacking(TransportFrame_Type *pframe, u8 *in_buf, u16 len, u8 pack_flag);

#endif
