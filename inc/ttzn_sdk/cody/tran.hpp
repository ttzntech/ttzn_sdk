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

class CanTran {
public:
    CanTran(const std::string& ifname);
    ~CanTran();
    
    bool send(uint32_t idx);
    bool recv(uint32_t idx);

    ActualData data;

private:
    int fd;
    std::string filename;

    Data data_;

    CanMsg send_data_;
    CanMsg send_buf_;

    CanMsg recv_data_;
};

} /* END namespace cody */

#endif