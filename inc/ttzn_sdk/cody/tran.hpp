/*
 * Created on Mon Aug 12 2024
 *
 * Author: Joonchen Liau
 * Email: liaojuncheng123@foxmail.com
 *
 * Copyright (c) 2024 Tiante Intelligent Technology
 */

#ifndef CODY_TRAN_H
#define CODY_TRAN_H

#include <iostream>

#include "ttzn_sdk/common.hpp"
#include "ttzn_sdk/cody/parser.hpp"


namespace cody {

class CANTran : public CANInterface {
public:
    CANTran(const std::string& ifname, DevType dev_type = DevType::USB_TTL_CAN);
    
    bool send(uint32_t idx);
    bool recv(uint32_t idx);

    ActualData data;

private:
    Data data_;
};

} /* END namespace cody */

#endif /* END CODY_TRAN_H */