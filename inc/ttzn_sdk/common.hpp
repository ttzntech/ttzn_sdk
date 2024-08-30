/*
 * Created on Sat Aug 10 2024
 *
 * Author: Joonchen Liau
 * Email: liaojuncheng123@foxmail.com
 *
 * Copyright (c) 2024 Tiante Intelligent Technology
 */

#ifndef COMMON_H
#define COMMON_H

#include <stdint.h>
#include <string>
#include <thread>
#include <atomic>

#include "utils.hpp"


/**
 * @brief Device type that transmit CAN frame
 * 
 */
enum class DevType: uint8_t {
    USB_TTL_CAN,        /* USB to TTL to CAN */
    CANable,            /* USB to CAN */
    ORIGIN              /* Original CAN */
};

/* USB to TTL to CAN message */
struct PREPACK UTCMsg {
    uint8_t head;
    uint8_t ext;
    uint8_t remt;
    uint8_t can_dlc;
    /* Which part of frame need to reverse */
    union {
        struct PREPACK {
            uint8_t data[8];
            uint32_t can_id;
        } POSTPACK;
        uint8_t reverse[12];
    };
    // uint8_t tail; 
} POSTPACK;

/* Socket CAN message */
struct PREPACK SCMsg {
    uint32_t can_id;
    uint8_t can_dlc;
    RESERVE(24);
    union {
        uint8_t data[8];
        uint8_t reverse[8];
    };
} POSTPACK;

union CANMsg {
    UTCMsg utc; /* utc stand for usb ttl can */
    SCMsg sc;   /* sc stand for socket can */
};

class CANInterface {
public:
    CANInterface(const std::string& ifname, DevType dev_type);
    CANInterface(const CANInterface&) = delete;
    CANInterface& operator=(const CANInterface&) = delete;
    virtual ~CANInterface();

    virtual bool send(uint32_t idx) = 0;
    virtual bool recv(uint32_t idx) = 0;

    virtual void async_recv();
    virtual void stop_async_recv();

protected:
    int fd;
    std::string ifname;
    DevType dev_type;

    CANMsg send_; 
    CANMsg recv_;

    /* async recv */
    std::thread recv_thread_;
    std::atomic<bool> running_;
    
    virtual void async_recv_() = 0;
};

#endif /* END COMMON_H */