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

CANTran::CANTran(const std::string& ifname, DevType dev_type) : 
CANInterface(ifname, dev_type) {}

/**
 * Send CAN data function
 * You must fill the 'data' with corresponding idx first
 */
bool CANTran::send(uint32_t idx) {
    int nbytes;
    pack(idx, data_, data, dev_type, send_);
    switch (dev_type)
    {
    case DevType::USB_TTL_CAN:
        buf_.utc = send_.utc;
        reverse_byte(&buf_.utc, sizeof(buf_.utc));
        nbytes = write(fd, &buf_.utc, sizeof(buf_.utc));
        break;
    case DevType::CANable:
    case DevType::ORIGIN:
        buf_.sc = send_.sc;
        reverse_byte(&buf_.sc, sizeof(buf_.sc));
        nbytes = write(fd, &buf_.sc, sizeof(buf_.sc));
        break;
    }

    if (nbytes != sizeof(buf_.utc)) {
        #ifdef LOG
        perror("Error in sending CAN frame");
        #endif
        return false;
    }

    return true;
}

/**
 * Recv CAN data function
 * recv specific one package CAN data to correspondind idx of 'data'
 */
bool CANTran::recv(uint32_t idx) {
    int nbytes;
    while (!check_buf(idx, data)) { /* recv idx equal to desire or have buf data */
        switch (dev_type)
        {
        case DevType::USB_TTL_CAN:
            nbytes = read(fd, &recv_.utc, sizeof(recv_.utc));
            if (nbytes < 0) {
                #ifdef LOG
                perror("Error in reading");
                #endif
                return false;
            }
            reverse_byte(&recv_.utc, sizeof(recv_.utc));
            break;
        case DevType::CANable:
        case DevType::ORIGIN:
            nbytes = read(fd, &recv_.sc, sizeof(recv_.sc));
            if (nbytes < 0) {
                #ifdef LOG
                perror("Error in reading");
                #endif
                return false;
            }
            reverse_byte(&recv_.sc, sizeof(recv_.sc));
            break;
        }
        unpack(data_, data, dev_type, recv_);
    }

    return true;
}

} /* END namespace cody */