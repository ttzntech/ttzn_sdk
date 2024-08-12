/*
 * Created on Sat Aug 10 2024
 *
 * Author: Joonchen Liau
 * Email: liaojuncheng123@foxmail.com
 *
 * Copyright (c) 2024 Tiante Intelligent Technology
 */

#ifndef UTILS_H
#define UTILS_H

/* BEGIN Macro for data pack */
#define RESERVE(n) uint64_t :(n)

/* Define PREPACK */
#ifndef PREPACK
#define PREPACK
#endif

/* Define POSTPACK */
#ifndef POSTPACK
    #ifdef __GNUC__
    #define POSTPACK __attribute__((packed))
    #elif
    #define POSTPACK
    #endif
#endif
/* END macro for data pack */

#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <sys/types.h>

/**
 * @brief revsese_byte order at `ptr` with size `n`
 * 
 * @param ptr pointer to where start to reverse
 * @param n reverse number
 */
inline void reverse_byte(void* ptr, size_t n) {
  char *l = (char*)ptr, *r = l + n - 1;
  char tmp;
  while (l < r) {
    tmp = *l;
    *l = *r;
    *r = tmp;
    l++;
    r--;
  }
}

/**
 * @brief set a value to a bound
 * 
 * @tparam T 
 * @param x 
 * @param low 
 * @param upper 
 * @return T 
 */
template<typename T>
inline T bound(T x, T low, T upper) {
  if (x <= low) return low;
  else if(x >= upper) return upper;
  else return x;
}

int uart_set(int fd, uint64_t baude, int c_flow, int bits, char parity, int stop);

#endif 