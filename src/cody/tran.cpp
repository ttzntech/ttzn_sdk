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

CanTran::CanTran(const std::string& filename) : filename(filename) {
    send_data_.head    = 0xaa;
    send_data_.ext     = 0x00;
    send_data_.remt    = 0x00;
    send_data_.len     = 0x08;
    // send_frame_.tail    = 0x7a;
    fd = open(filename.c_str(), O_RDWR | O_NOCTTY | O_NONBLOCK);
    if (fd == -1) {
        perror("Error in open UART\n");
        exit(-1);
    }

    int baud = 115200;
    if (uart_set(fd, baud, 0, 8, 'N', 1) != 0) {
        perror("Error on set UART config\n");
        exit(-1);
    }
}

CanTran::~CanTran() {
    close(fd);
}


/**
 * Send CAN data function
 * You must fill the 'data' with corresponding idx first
 */
bool CanTran::send(uint32_t idx) {
    pack(idx, data_, data, &send_data_);
    send_buf_ = send_data_;
    reverse_byte(&send_buf_, sizeof(send_buf_));
    int nbytes = write(fd, &send_buf_, sizeof(send_buf_));
    if (nbytes != sizeof(send_buf_)) {
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
bool CanTran::recv(uint32_t idx) {
    while (!check_buf(idx, data)) { /* recv idx equal to desire or have buf data */
        int nbytes = read(fd, &recv_data_, sizeof(recv_data_));
        if (nbytes < 0) {
            #ifdef LOG
            perror("Error in reading");
            #endif
            return false;
        }
        reverse_byte(&recv_data_, sizeof(recv_data_));
        unpack(data_, data, &recv_data_);
    }

    return true;
}

} /* END namespace cody */