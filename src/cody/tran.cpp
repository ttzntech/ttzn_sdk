/*
 * Created on Mon Aug 12 2024
 *
 * Author: Joonchen Liau
 * Email: liaojuncheng123@foxmail.com
 *
 * Copyright (c) 2024 Tiante Intelligent Technology
 */

#include "ttzn_sdk/cody/tran.hpp"

#include "ttzn_sdk/utils.hpp"


namespace cody {

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
        data_.i111MoveCtrl.speed = bound<int16_t>(static_cast<int16_t>(data.i111MoveCtrl.speed / 0.00066), -2000, 2000);
        data_.i111MoveCtrl.corner = bound<int16_t>(static_cast<int16_t>(data.i111MoveCtrl.corner / 0.06), -500, 500);
        memcpy(pack_data, data_.i111MoveCtrl.data, size);
        break;
    case ID_ModeCtrl:
        data_.i421ModeCtrl.mode = static_cast<uint8_t>(data.i421ModeCtrl.mode);
        memcpy(pack_data, data_.i421ModeCtrl.data, size);
        break;
    case ID_LightCtrl:
        data_.i121LightCtrl.front = static_cast<uint8_t>(data.i121LightCtrl.front);
        data_.i121LightCtrl.rear = static_cast<uint8_t>(data.i121LightCtrl.rear);
        data_.i121LightCtrl.parity = data.i121LightCtrl.parity;
        memcpy(pack_data, data_.i121LightCtrl.data, size);
        break;
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
        data.i211SysStatus.error_info = data_.i211SysStatus.error_info;
        data.i211SysStatus.parity = data_.i211SysStatus.parity;
        break;
    case ID_MoveCtrlFb:
        memcpy(data_.i221MoveCtrlFb.data, unpack_data, size);
        data.i221MoveCtrlFb.recv_ = 1;
        data.i221MoveCtrlFb.speed = static_cast<double>(data_.i221MoveCtrlFb.speed * 0.00066); /* m/s */
        data.i221MoveCtrlFb.corner = static_cast<double>(data_.i221MoveCtrlFb.corner * 0.06); /* degree */
        break;
    case ID_Motor1InfoFb:
        memcpy(data_.i251Motor1InfoFb.data, unpack_data, size);
        data.i251Motor1InfoFb.recv_ = 1;
        data.i251Motor1InfoFb.rpm = data_.i251Motor1InfoFb.rpm;
        data.i251Motor1InfoFb.pos = data_.i251Motor1InfoFb.pos;
        break;
    case ID_Motor2InfoFb:
        memcpy(data_.i252Motor2InfoFb.data, unpack_data, size);
        data.i252Motor2InfoFb.recv_ = 1;
        data.i252Motor2InfoFb.rpm = data_.i252Motor2InfoFb.rpm;
        data.i252Motor2InfoFb.pos = data_.i252Motor2InfoFb.pos;
        break;
    case ID_WarnFb:
        memcpy(data_.i261WarnFb.data, unpack_data, size);
        data.i261WarnFb.recv_ = 1;
        data.i261WarnFb.steer_motor_warn = data_.i261WarnFb.steer_motor_warn;
        data.i261WarnFb.motor1_warn = data_.i261WarnFb.motor1_warn;
        data.i261WarnFb.motor2_warn = data_.i261WarnFb.motor2_warn;
        data.i261WarnFb.bat_warn = data_.i261WarnFb.bat_warn;
        data.i261WarnFb.cabin1_temp = data_.i261WarnFb.cabin1_temp;
        data.i261WarnFb.cabin2_temp = data_.i261WarnFb.cabin2_temp;
        data.i261WarnFb.emer_stop = data_.i261WarnFb.emer_stop;
        break;
    case ID_OdomFb:
        memcpy(data_.i311OdomFb.data, unpack_data, size);
        data.i311OdomFb.recv_ = 1;
        data.i311OdomFb.left = static_cast<double>(data_.i311OdomFb.left * 0.001); /* m */
        data.i311OdomFb.right = static_cast<double>(data_.i311OdomFb.right * 0.001); /* m */
        break;
    case ID_BMSFb:
        memcpy(data_.i361BMSFb.data, unpack_data, size);
        data.i361BMSFb.recv_ = 1;
        data.i361BMSFb.bat_soc = bound<uint8_t>(data_.i361BMSFb.bat_soc, 0, 100);
        data.i361BMSFb.vol = data_.i361BMSFb.vol; /* voltage */
        data.i361BMSFb.cur = data_.i361BMSFb.cur; /* current */
        data.i361BMSFb.temp = data_.i361BMSFb.temp; /* degree */
        break;
    }
}

} /* END namespace cody */