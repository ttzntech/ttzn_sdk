/*
 * Created on Sat Aug 10 2024
 *
 * Author: Joonchen Liau
 * Email: liaojuncheng123@foxmail.com
 *
 * Copyright (c) 2024 Tiante Intelligent Technology
 */

#include "ttzn_sdk/cody/parser.hpp"


namespace cody {

/**
 * @brief pack data with specific 'idx' in 'act_data' to 'out'
 * 
 * @param idx CAN id
 * @param data param to store origin data 
 * @param act_data param to store actual data
 * @param out output CAN message
 * @return int 0 mean success other mean false
 */
int pack(uint32_t idx, Data& data, const ActualData& act_data, CanMsg* out) {
    out->idx = idx;
    switch (idx)
    {
    case ID_MoveCtrl:
        data.i111MoveCtrl.speed = bound<int16_t>(static_cast<int16_t>(act_data.i111MoveCtrl.speed / 0.00066), -2000, 2000);
        data.i111MoveCtrl.corner = bound<int16_t>(static_cast<int16_t>(act_data.i111MoveCtrl.corner / 0.06), -500, 500);
        memcpy(out->data, data.i111MoveCtrl.data, sizeof(out->data));
        break;
    case ID_ModeCtrl:
        data.i421ModeCtrl.mode = static_cast<uint8_t>(act_data.i421ModeCtrl.mode);
        memcpy(out->data, data.i421ModeCtrl.data, sizeof(out->data));
        break;
    case ID_LightCtrl:
        data.i121LightCtrl.front = static_cast<uint8_t>(act_data.i121LightCtrl.front);
        data.i121LightCtrl.rear = static_cast<uint8_t>(act_data.i121LightCtrl.rear);
        data.i121LightCtrl.parity = act_data.i121LightCtrl.parity;
        memcpy(out->data, data.i121LightCtrl.data, sizeof(out->data));
        break;
    default:
        return -1;
    }

    return 0;
}

/**
 * @brief unpack one package data in 'in' to 'act_data'
 * 
 * @param data param to store origin data 
 * @param act_data param to store actual data
 * @param in input CAN message
 * @return int 0 means success otherwise means error
 */
int unpack(Data& data, ActualData& act_data, CanMsg* in) {
    switch (in->idx)
    {
    case ID_SysStatus:
        memcpy(data.i211SysStatus.data, in->data, sizeof(in->data));
        act_data.i211SysStatus.recv_ = 1;
        act_data.i211SysStatus.cur_status = data.i211SysStatus.cur_status;
        act_data.i211SysStatus.ctrl_mode = data.i211SysStatus.ctrl_mode;
        act_data.i211SysStatus.bat_vol = static_cast<double>(data.i211SysStatus.bat_vol / 100); /* V */
        act_data.i211SysStatus.error_info = data.i211SysStatus.error_info;
        act_data.i211SysStatus.parity = data.i211SysStatus.parity;
        break;
    case ID_MoveCtrlFb:
        memcpy(data.i221MoveCtrlFb.data, in->data, sizeof(in->data));
        act_data.i221MoveCtrlFb.recv_ = 1;
        act_data.i221MoveCtrlFb.speed = static_cast<double>(data.i221MoveCtrlFb.speed * 0.00066); /* m/s */
        act_data.i221MoveCtrlFb.corner = static_cast<double>(data.i221MoveCtrlFb.corner * 0.06); /* degree */
        break;
    case ID_Motor1InfoFb:
        memcpy(data.i251Motor1InfoFb.data, in->data, sizeof(in->data));
        act_data.i251Motor1InfoFb.recv_ = 1;
        act_data.i251Motor1InfoFb.rpm = data.i251Motor1InfoFb.rpm;
        act_data.i251Motor1InfoFb.pos = data.i251Motor1InfoFb.pos;
        break;
    case ID_Motor2InfoFb:
        memcpy(data.i252Motor2InfoFb.data, in->data, sizeof(in->data));
        act_data.i252Motor2InfoFb.recv_ = 1;
        act_data.i252Motor2InfoFb.rpm = data.i252Motor2InfoFb.rpm;
        act_data.i252Motor2InfoFb.pos = data.i252Motor2InfoFb.pos;
        break;
    case ID_WarnFb:
        memcpy(data.i261WarnFb.data, in->data, sizeof(in->data));
        act_data.i261WarnFb.recv_ = 1;
        act_data.i261WarnFb.steer_motor_warn = data.i261WarnFb.steer_motor_warn;
        act_data.i261WarnFb.motor1_warn = data.i261WarnFb.motor1_warn;
        act_data.i261WarnFb.motor2_warn = data.i261WarnFb.motor2_warn;
        act_data.i261WarnFb.bat_warn = data.i261WarnFb.bat_warn;
        act_data.i261WarnFb.cabin1_temp = data.i261WarnFb.cabin1_temp;
        act_data.i261WarnFb.cabin2_temp = data.i261WarnFb.cabin2_temp;
        act_data.i261WarnFb.emer_stop = data.i261WarnFb.emer_stop;
        break;
    case ID_OdomFb:
        memcpy(data.i311OdomFb.data, in->data, sizeof(in->data));
        act_data.i311OdomFb.recv_ = 1;
        act_data.i311OdomFb.left = static_cast<double>(data.i311OdomFb.left / 1000); /* m */
        act_data.i311OdomFb.right = static_cast<double>(data.i311OdomFb.right / 1000); /* m */
        break;
    case ID_BMSFb:
        memcpy(data.i361BMSFb.data, in->data, sizeof(in->data));
        act_data.i361BMSFb.recv_ = 1;
        act_data.i361BMSFb.bat_SOC = bound<uint8_t>(data.i361BMSFb.bat_SOC, 0, 100);
        act_data.i361BMSFb.vol = data.i361BMSFb.vol; /* voltage */
        act_data.i361BMSFb.cur = data.i361BMSFb.cur; /* current */
        act_data.i361BMSFb.temp = data.i361BMSFb.temp; /* degree */
        break;
    default:
        return 0;
    }

    return in->idx;
}

/**
 * @brief check whether have buf data at first
 * 
 * @param idx 
 * @param act_data 
 * @return int 1 means have buf data 0 means haven't buf data
 */
int check_buf(uint32_t idx, ActualData& act_data) {
    switch (idx)
    {
    case ID_SysStatus:
        if (act_data.i211SysStatus.recv_) {
            act_data.i211SysStatus.recv_ = 0;
            return 1;
        }
        break;
    case ID_MoveCtrlFb:
        if (act_data.i221MoveCtrlFb.recv_) {
            act_data.i221MoveCtrlFb.recv_ = 0;
            return 1;
        }
        break;
    case ID_Motor1InfoFb:
        if (act_data.i251Motor1InfoFb.recv_) {
            act_data.i251Motor1InfoFb.recv_ = 0;
            return 1;
        }
        break;
    case ID_Motor2InfoFb:
        if (act_data.i252Motor2InfoFb.recv_) {
            act_data.i252Motor2InfoFb.recv_ = 0;
            return 1;
        }
        break;
    case ID_WarnFb:
        if (act_data.i261WarnFb.recv_) {
            act_data.i261WarnFb.recv_ = 0;
            return 1;
        }
        break;
    case ID_OdomFb:
        if (act_data.i311OdomFb.recv_) {
            act_data.i311OdomFb.recv_ = 0;
            return 1;
        }
        break;
    case ID_BMSFb:
        if (act_data.i361BMSFb.recv_) {
            act_data.i361BMSFb.recv_ = 0;
            return 1;
        }
        break;
    }

    return 0;
}

} /* END cody namespace */