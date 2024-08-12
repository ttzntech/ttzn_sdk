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

#include "utils.hpp"


struct PREPACK CanMsg {
    // uint8_t tail; 
    uint8_t data[8];
    uint32_t idx;
    uint8_t len;
    uint8_t remt;
    uint8_t ext;
    uint8_t head;
} POSTPACK;

#endif /* END COMMON_H */