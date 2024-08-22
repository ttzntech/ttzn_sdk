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
    CANTran(const std::string& ifname, DevType dev_type);
    
    bool send(uint32_t idx) override;
    bool recv(uint32_t idx) override;

    ActualData data;

private:
    void async_recv_() override;
    Data data_;
};

} /* END namespace coroco */

#endif /* END COROCO_TRAN_H */