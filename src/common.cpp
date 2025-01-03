/*
 * Created on Wed Aug 14 2024
 *
 * Author: Joonchen Liau
 * Email: liaojuncheng123@foxmail.com
 *
 * Copyright (c) 2024 Tiante Intelligent Technology
 */

#include "ttzn_sdk/common.hpp"

/**
 * @brief Construct a new CANInterface::CANInterface object
 * 
 * @param ifname 
 * @param dev_type 
 */
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

/**
 * @brief send a CAN frame
 * before invoke this function, should fill the corresponding data at `idx`
 * 
 * @param idx 
 * @return true 
 * @return false 
 */
bool CANInterface::send(uint32_t idx) {
    int nbytes;
    switch (dev_type)
    {
    case DevType::USB_TTL_CAN:
        send_.utc.can_id = idx;
        if (!pack(idx, send_.utc.data, sizeof(send_.utc.data))) {
            perror("Error in packing CAN frame");
            return false;
        }
        reverse_byte(send_.utc.reverse, sizeof(send_.utc.reverse));
        nbytes = write(fd, &send_.utc, sizeof(send_.utc));
        if (nbytes != sizeof(send_.utc)) {
            perror("Error in sending CAN frame");
            return false;
        }
        break;
    case DevType::CANable:
    case DevType::ORIGIN:
        send_.sc.can_id = idx;
        if (!pack(idx, send_.sc.data, sizeof(send_.sc.data))) {
            perror("Error in packing CAN frame");
            return false;
        }
        reverse_byte(send_.sc.reverse, sizeof(send_.sc.reverse));
        nbytes = write(fd, &send_.sc, sizeof(send_.sc));
        if (nbytes != sizeof(send_.sc)) {
            perror("Error in sending CAN frame");
            return false;
        }
        break;
    }

    return true;
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
        int nbytes;
        uint32_t idx;
        while (running_) {
            switch (dev_type)
            {
            case DevType::USB_TTL_CAN:
                nbytes = read(fd, &recv_.utc, sizeof(recv_.utc));
                if (nbytes < 0) continue;
                reverse_byte(recv_.utc.reverse, sizeof(recv_.utc.reverse));
                idx = recv_.utc.can_id;
                unpack(idx, recv_.utc.data, sizeof(recv_.utc.data));
                break;
            case DevType::CANable:
            case DevType::ORIGIN:
                nbytes = read(fd, &recv_.sc, sizeof(recv_.sc));
                if (nbytes < 0) continue;
                reverse_byte(recv_.sc.reverse, sizeof(recv_.sc.reverse));
                idx = recv_.sc.can_id;
                unpack(idx, recv_.sc.data, sizeof(recv_.sc.data));
                break;
            }
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