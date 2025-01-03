/*
 * Created on Tue Aug 20 2024
 *
 * Author: Joonchen Liau
 * Email: liaojuncheng123@foxmail.com
 *
 * Copyright (c) 2024 Tiante Intelligent Technology
 */

#include "ttzn_sdk/coroco/tran.hpp"

#include "ttzn_sdk/utils.hpp"


namespace coroco {

/**
 * @brief Construct a new CANTran::CANTran object
 * 
 * @param ifname 
 * @param dev_type 
 */
CANTran::CANTran(const std::string& ifname, DevType dev_type) : 
CANInterface(ifname, dev_type) {}

/**
 * @brief pack correct format of data to `pack_data`
 * 
 * @param idx 
 * @param pack_data 
 * @param size 
 * @return true 
 * @return false 
 */
bool CANTran::pack(uint32_t idx, uint8_t* pack_data, size_t size) {
    switch (idx)
    {
    case ID_MoveCtrl:
        data_.i111MoveCtrl.speed = bound<int16_t>(static_cast<int16_t>(data.i111MoveCtrl.speed / 0.001), -2000, 2000);
        data_.i111MoveCtrl.angular = bound<int16_t>(static_cast<int16_t>(data.i111MoveCtrl.angular / 0.004), -500, 500);
        memcpy(pack_data, data_.i111MoveCtrl.data, size);
        break;
    case ID_ModeCtrl:
        data_.i421ModeCtrl.mode = static_cast<uint8_t>(data.i421ModeCtrl.mode);
        memcpy(pack_data, data_.i421ModeCtrl.data, size);
        break;
    case ID_LightCtrl:
        data_.i121LightCtrl.front = static_cast<uint8_t>(data.i121LightCtrl.front);
        data_.i121LightCtrl.parity = data.i121LightCtrl.parity;
        memcpy(pack_data, data_.i121LightCtrl.data, size);
        break;
    case ID_TurnTweak:
        data_.i42fTurnTweak.tweak = static_cast<uint8_t>(data.i42fTurnTweak.tweak / 0.01);
        memcpy(pack_data, data_.i42fTurnTweak.data, size);
    default:
        return false;
    }

    return true;
}

/**
 * @brief unpack correct format data from `unpack_data`
 * 
 * @param idx 
 * @param unpack_data 
 * @param size 
 */
void CANTran::unpack(uint32_t idx, uint8_t* unpack_data, size_t size) {
    switch (idx)
    {
    case ID_SysStatus:
        memcpy(data_.i211SysStatus.data, unpack_data, size);
        data.i211SysStatus.recv_ = 1;
        data.i211SysStatus.cur_status = data_.i211SysStatus.cur_status;
        data.i211SysStatus.ctrl_mode = data_.i211SysStatus.ctrl_mode;
        data.i211SysStatus.bat_vol = static_cast<double>(data_.i211SysStatus.bat_vol * 0.01); /* V */
        data.i211SysStatus.bat_cur = static_cast<double>(data_.i211SysStatus.bat_cur * 0.01); /* A */
        data.i211SysStatus.parity = data_.i211SysStatus.parity;
        break;
    case ID_MoveCtrlFb:
        memcpy(data_.i221MoveCtrlFb.data, unpack_data, size);
        data.i221MoveCtrlFb.recv_ = 1;
        data.i221MoveCtrlFb.speed = static_cast<double>(data_.i221MoveCtrlFb.speed * 0.001); /* m/s */
        data.i221MoveCtrlFb.angular = static_cast<double>(data_.i221MoveCtrlFb.angular * 0.004); /* rad/s */
        break;
    case ID_ReMoveCtrlFb:
        memcpy(data_.i241ReMoveCtrlFb.data, unpack_data, size);
        data.i241ReMoveCtrlFb.recv_ = 1;
        data.i241ReMoveCtrlFb.speed = static_cast<double>(data_.i241ReMoveCtrlFb.speed * 0.001); /* m/s */
        data.i241ReMoveCtrlFb.angular = static_cast<double>(data_.i241ReMoveCtrlFb.angular * 0.004); /* rad/s */
        break;
    case ID_Motor1InfoFb:
        memcpy(data_.i250Motor1InfoFb.data, unpack_data, size);
        data.i250Motor1InfoFb.recv_ = 1;
        data.i250Motor1InfoFb.rpm = data_.i250Motor1InfoFb.rpm;
        data.i250Motor1InfoFb.pos = data_.i250Motor1InfoFb.pos;
        break;
    case ID_Motor2InfoFb:
        memcpy(data_.i251Motor2InfoFb.data, unpack_data, size);
        data.i251Motor2InfoFb.recv_ = 1;
        data.i251Motor2InfoFb.rpm = data_.i251Motor2InfoFb.rpm;
        data.i251Motor2InfoFb.pos = data_.i251Motor2InfoFb.pos;
        break;
    case ID_Motor3InfoFb:
        memcpy(data_.i252Motor3InfoFb.data, unpack_data, size);
        data.i252Motor3InfoFb.recv_ = 1;
        data.i252Motor3InfoFb.rpm = data_.i252Motor3InfoFb.rpm;
        data.i252Motor3InfoFb.pos = data_.i252Motor3InfoFb.pos;
        break;
    case ID_Motor4InfoFb:
        memcpy(data_.i253Motor4InfoFb.data, unpack_data, size);
        data.i253Motor4InfoFb.recv_ = 1;
        data.i253Motor4InfoFb.rpm = data_.i253Motor4InfoFb.rpm;
        data.i253Motor4InfoFb.pos = data_.i253Motor4InfoFb.pos;
        break;
    case ID_WarnFb:
        memcpy(data_.i261WarnFb.data, unpack_data, size);
        data.i261WarnFb.recv_ = 1;
        data.i261WarnFb.steer_motor_warn = data_.i261WarnFb.steer_motor_warn;
        data.i261WarnFb.motor1_warn = data_.i261WarnFb.motor1_warn;
        data.i261WarnFb.motor2_warn = data_.i261WarnFb.motor2_warn;
        data.i261WarnFb.bat_warn = data_.i261WarnFb.bat_warn;
        data.i261WarnFb.temp1 = data_.i261WarnFb.temp1;
        data.i261WarnFb.temp2 = data_.i261WarnFb.temp2;
        data.i261WarnFb.warn = data_.i261WarnFb.warn;
        break;
    case ID_OdomFb:
        memcpy(data_.i311OdomFb.data, unpack_data, size);
        data.i311OdomFb.recv_ = 1;
        data.i311OdomFb.odom = static_cast<double>(data_.i311OdomFb.odom * 0.01); /* m */
        break;
    case ID_BMSFb:
        memcpy(data_.i361BMSFb.data, unpack_data, size);
        data.i361BMSFb.recv_ = 1;
        data.i361BMSFb.bat_soc = bound<uint8_t>(data_.i361BMSFb.bat_soc, 0, 100);
        data.i361BMSFb.vol = data_.i361BMSFb.vol; /* voltage mV */
        data.i361BMSFb.cur = data_.i361BMSFb.cur; /* current A */
        data.i361BMSFb.temp = data_.i361BMSFb.temp; /* degree */
        break;
    }
}

} /* END namespace coroco */