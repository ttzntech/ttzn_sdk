/*
 * Created on Tue Aug 20 2024
 *
 * Author: Joonchen Liau
 * Email: liaojuncheng123@foxmail.com
 *
 * Copyright (c) 2024 Tiante Intelligent Technology
 */

#include "ttzn_sdk/coroco/parser.hpp"


namespace coroco {

/**
 * @brief pack data with specific 'idx' in 'act_data' to 'out'
 * 
 * @param idx CAN id
 * @param data param to store origin data 
 * @param act_data param to store actual data
 * @param dev_type param to specific CAN dev type 
 * @param out output CAN message
 * @return int 0 mean success other mean false
 */
int pack(uint32_t idx, Data& data, const ActualData& act_data, DevType dev_type, CANMsg& out) {
    int ret = 0;
    switch (dev_type)
    {
    case DevType::USB_TTL_CAN:
        out.utc.can_id = idx;
        ret = _pack(data, act_data, idx, out.utc.data, sizeof(out.utc.data));
        reverse_byte(out.utc.reverse, sizeof(out.utc.reverse));
        break;
    case DevType::CANable:
    case DevType::ORIGIN:
        out.sc.can_id = idx;
        ret = _pack(data, act_data, idx, out.sc.data, sizeof(out.sc.data));
        reverse_byte(out.sc.reverse, sizeof(out.sc.reverse));
        break;
    }

    return ret;
}

static int _pack(Data& data, const ActualData& act_data, uint32_t idx, uint8_t* data_, size_t size) {
    switch (idx)
    {
    case ID_MoveCtrl:
        data.i111MoveCtrl.speed = bound<int16_t>(static_cast<int16_t>(act_data.i111MoveCtrl.speed / 0.001), -2000, 2000);
        data.i111MoveCtrl.angular = bound<int16_t>(static_cast<int16_t>(act_data.i111MoveCtrl.angular / 0.001), -500, 500);
        memcpy(data_, data.i111MoveCtrl.data, size);
        break;
    case ID_ModeCtrl:
        data.i421ModeCtrl.mode = static_cast<uint8_t>(act_data.i421ModeCtrl.mode);
        memcpy(data_, data.i421ModeCtrl.data, size);
        break;
    case ID_LightCtrl:
        data.i121LightCtrl.front = static_cast<uint8_t>(act_data.i121LightCtrl.front);
        data.i121LightCtrl.rear = static_cast<uint8_t>(act_data.i121LightCtrl.rear);
        data.i121LightCtrl.parity = act_data.i121LightCtrl.parity;
        memcpy(data_, data.i121LightCtrl.data, size);
        break;
    case ID_TurnTweak:
        data.i42fTurnTweak.tweak = static_cast<uint8_t>(act_data.i42fTurnTweak.tweak / 0.01);
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
 * @param dev_type param to specific CAN dev type 
 * @param in input CAN message
 * @return int 0 means success otherwise means error
 */
uint32_t unpack(Data& data, ActualData& act_data, DevType dev_type, CANMsg& in) {
    uint32_t idx = 0;
    switch (dev_type)
    {
    case DevType::USB_TTL_CAN:
        reverse_byte(in.utc.reverse, sizeof(in.utc.reverse));
        idx = in.utc.can_id;
        _unpack(data, act_data, idx, in.utc.data, sizeof(in.utc.data));
        break;
    case DevType::CANable:
    case DevType::ORIGIN:
        reverse_byte(in.sc.reverse, sizeof(in.sc.reverse));
        idx = in.sc.can_id;
        _unpack(data, act_data, idx, in.sc.data, sizeof(in.sc.data));
        break;
    }

    return idx;
}

static void _unpack(Data& data, ActualData& act_data, uint32_t idx, uint8_t* data_, size_t size) {
    switch (idx)
    {
    case ID_SysStatus:
        memcpy(data.i211SysStatus.data, data_, size);
        act_data.i211SysStatus.recv_ = 1;
        act_data.i211SysStatus.cur_status = data.i211SysStatus.cur_status;
        act_data.i211SysStatus.ctrl_mode = data.i211SysStatus.ctrl_mode;
        act_data.i211SysStatus.bat_vol = static_cast<double>(data.i211SysStatus.bat_vol / 100); /* V */
        act_data.i211SysStatus.error_info = data.i211SysStatus.error_info;
        act_data.i211SysStatus.parity = data.i211SysStatus.parity;
        break;
    case ID_MoveCtrlFb:
        memcpy(data.i221MoveCtrlFb.data, data_, size);
        act_data.i221MoveCtrlFb.recv_ = 1;
        act_data.i221MoveCtrlFb.speed = static_cast<double>(data.i221MoveCtrlFb.speed * 0.001); /* m/s */
        act_data.i221MoveCtrlFb.angular = static_cast<double>(data.i221MoveCtrlFb.angular * 0.001); /* rad/s */
        break;
    case ID_Motor1InfoFb:
        memcpy(data.i251Motor1InfoFb.data, data_, size);
        act_data.i251Motor1InfoFb.recv_ = 1;
        act_data.i251Motor1InfoFb.rpm = data.i251Motor1InfoFb.rpm;
        act_data.i251Motor1InfoFb.pos = data.i251Motor1InfoFb.pos;
        break;
    case ID_Motor2InfoFb:
        memcpy(data.i252Motor2InfoFb.data, data_, size);
        act_data.i252Motor2InfoFb.recv_ = 1;
        act_data.i252Motor2InfoFb.rpm = data.i252Motor2InfoFb.rpm;
        act_data.i252Motor2InfoFb.pos = data.i252Motor2InfoFb.pos;
        break;
    case ID_Motor3InfoFb:
        memcpy(data.i253Motor3InfoFb.data, data_, size);
        act_data.i253Motor3InfoFb.recv_ = 1;
        act_data.i253Motor3InfoFb.rpm = data.i253Motor3InfoFb.rpm;
        act_data.i253Motor3InfoFb.pos = data.i253Motor3InfoFb.pos;
        break;
    case ID_Motor4InfoFb:
        memcpy(data.i254Motor4InfoFb.data, data_, size);
        act_data.i254Motor4InfoFb.recv_ = 1;
        act_data.i254Motor4InfoFb.rpm = data.i254Motor4InfoFb.rpm;
        act_data.i254Motor4InfoFb.pos = data.i254Motor4InfoFb.pos;
        break;
    case ID_WarnFb:
        memcpy(data.i261WarnFb.data, data_, size);
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
        memcpy(data.i311OdomFb.data, data_, size);
        act_data.i311OdomFb.recv_ = 1;
        act_data.i311OdomFb.left = static_cast<double>(data.i311OdomFb.left / 1000); /* m */
        act_data.i311OdomFb.right = static_cast<double>(data.i311OdomFb.right / 1000); /* m */
        break;
    case ID_BMSFb:
        memcpy(data.i361BMSFb.data, data_, size);
        act_data.i361BMSFb.recv_ = 1;
        act_data.i361BMSFb.bat_SOC = bound<uint8_t>(data.i361BMSFb.bat_SOC, 0, 100);
        act_data.i361BMSFb.vol = data.i361BMSFb.vol; /* voltage */
        act_data.i361BMSFb.cur = data.i361BMSFb.cur; /* current */
        act_data.i361BMSFb.temp = data.i361BMSFb.temp; /* degree */
        break;
    }
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

} /* END coroco namespace */