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
    CANTran(const std::string& ifname, DevType dev_type);
    
    bool send(uint32_t idx) override;
    bool recv(uint32_t idx) override;

    ActualData data;

private:
    void async_recv_() override;
    Data data_;
};

} /* END namespace cody */

#endif /* END CODY_TRAN_H */