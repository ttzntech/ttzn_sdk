/*
 * Created on Tue Aug 20 2024
 *
 * Author: Joonchen Liau
 * Email: liaojuncheng123@foxmail.com
 *
 * Copyright (c) 2024 Tiante Intelligent Technology
 */

#ifndef COROCO_TRAN_H
#define COROCO_TRAN_H 

#include <iostream>

#include "ttzn_sdk/common.hpp"
#include "ttzn_sdk/coroco/parser.hpp"


namespace coroco {

class CANTran : public CANInterface {
public:
    CANTran(const std::string& ifname = "/dev/ttyUSB0", DevType dev_type = DevType::USB_TTL_CAN);
    
    bool send(uint32_t idx);
    bool recv(uint32_t idx);

    ActualData data;

private:
    Data data_;
};

} /* END namespace coroco */

#endif /* END COROCO_TRAN_H */