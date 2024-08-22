/*
 * Created on Wed Aug 14 2024
 *
 * Author: Joonchen Liau
 * Email: liaojuncheng123@foxmail.com
 *
 * Copyright (c) 2024 Tiante Intelligent Technology
 */

#include "ttzn_sdk/common.hpp"


CANInterface::CANInterface(const std::string& ifname, DevType dev_type) : 
ifname(ifname), dev_type(dev_type) {
    int baud;
    switch (dev_type)
    {
    case DevType::USB_TTL_CAN:
        send_.utc.head    = 0xaa;
        send_.utc.ext     = 0x00;
        send_.utc.remt    = 0x00;
        send_.utc.can_dlc = 0x08;
        // send.utc.tail    = 0x7a;
        fd = open(ifname.c_str(), O_RDWR | O_NOCTTY | O_NONBLOCK);
        if (fd == -1) {
            perror("Error in open UART\n");
            exit(-1);
        }

        baud = 115200;
        if (uart_set(fd, baud, 0, 8, 'N', 1) != 0) {
            perror("Error on set UART config\n");
            exit(-1);
        }
        break;
    case DevType::CANable:
    case DevType::ORIGIN:
        send_.sc.can_dlc = 0x08;
        fd = socket_can_set(ifname.c_str());
        if (fd == -1) {
            perror("Error in open CAN\n");
            exit(-1);
        }
        break;
    default:
        perror("Error Unknow ModeType\n");
        exit(-1);
        break;
    }
}

CANInterface::~CANInterface() {
    close(fd);
    stop_async_recv();
}

/**
 * @brief asynchronous recv function
 * 
 */
void CANInterface::async_recv() {
    running_ = true;
    recv_thread_ = std::thread([this]() {
        while (running_) {
            this->async_recv_();
        }
    });
}

/**
 * @brief stop async recv thread 
 * 
 */
void CANInterface::stop_async_recv() {
    running_ = false;
    if (recv_thread_.joinable()) {
        recv_thread_.join();
    }
}