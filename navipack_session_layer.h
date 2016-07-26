/**
******************************************************************************
* @file    navipack_session_layer.h
* @author  Jalon
* @date    2016.06.16
* @brief   通讯协议会话层解析相关声明
* @attention Copyright (C) 2016 Inmotion
******************************************************************************
*/
#ifndef __NAVIPACK_SESSION_LAYER_H__
#define __NAVIPACK_SESSION_LAYER_H__

#include "stm32_lib.h"
#include "navipack_api.h"

bool RegisterWrite(NaviPack_HeadType *head, u8 *reg, u32 size, u8 reg_id);
bool RegisterRead(NavipackComm_Type *comm, NaviPack_HeadType *head, u8 err_id, u8 *reg, u32 reg_size, u8 reg_id);

#endif