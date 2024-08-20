/*
 * Created on Tue Aug 20 2024
 *
 * Author: Joonchen Liau
 * Email: liaojuncheng123@foxmail.com
 *
 * Copyright (c) 2024 Tiante Intelligent Technology
 */

#ifndef COROCO_PARSER_H 
#define COROCO_PARSER_H 

#include <stdint.h>
#include <string.h>

#include "ttzn_sdk/utils.hpp"
#include "ttzn_sdk/common.hpp"


namespace coroco {

enum ID : uint32_t {
    ID_SysStatus            = 0x211,
    ID_MoveCtrlFb           = 0x221,
    ID_MoveCtrl             = 0x111,
    ID_Motor1InfoFb         = 0x251,
    ID_Motor2InfoFb         = 0x252,
    ID_Motor3InfoFb         = 0x253,
    ID_Motor4InfoFb         = 0x254,
    ID_WarnFb               = 0x261,
    ID_ModeCtrl             = 0x421,
    ID_LightCtrl            = 0x121,
    ID_OdomFb               = 0x311,
    ID_BMSFb                = 0x361,
    ID_TurnTweak            = 0x42f
};

/**
 * note: t prefix means type 
 *       i prefix means instance
 *       E prefix means Enum
 */

typedef union {
    struct PREPACK {
        uint8_t parity;
        RESERVE(8);
        uint8_t error_info;
        RESERVE(8);
        uint16_t bat_vol;       /* battery voltage */
        uint8_t ctrl_mode;      /* mode control */
        uint8_t cur_status;     /* current status */
    } POSTPACK;
    uint8_t data[8];
} t211SysStatus;

typedef union {
    struct PREPACK {
        RESERVE(32);
        int16_t angular;
        int16_t speed;
    } POSTPACK;
    uint8_t data[8];
} t221MoveCtrlFb;

typedef t221MoveCtrlFb t111MoveCtrl;

typedef union {
    struct PREPACK {
        int32_t pos;
        RESERVE(16);
        int16_t rpm;
    } POSTPACK;
    uint8_t data[8];
} t251Motor1InfoFb;

typedef t251Motor1InfoFb t252Motor2InfoFb;
typedef t251Motor1InfoFb t253Motor3InfoFb;
typedef t251Motor1InfoFb t254Motor4InfoFb;


typedef union {
    struct PREPACK {
        RESERVE(8);
        uint8_t emer_stop;      /* emergency stop */
        uint8_t cabin2_temp;
        uint8_t cabin1_temp;
        uint8_t bat_warn;       /* battery warn */
        uint8_t motor2_warn;
        uint8_t motor1_warn;
        uint8_t steer_motor_warn;
    } POSTPACK;
    uint8_t data[8];
} t261WarnFb;

typedef union {
    struct PREPACK {
        RESERVE(56);
        uint8_t mode; 
    } POSTPACK;
    uint8_t data[8];
} t421ModeCtrl;

typedef union {
    struct PREPACK {
        uint8_t parity;
        RESERVE(40);
        uint8_t rear;       /* rear light */
        uint8_t front;      /* front light */
    } POSTPACK;
    uint8_t data[8];
} t121LightCtrl;

typedef union {
    struct PREPACK {
        uint32_t right;         /* right wheel odom */
        uint32_t left;          /* left wheel odom */
    } POSTPACK;
    uint8_t data[8];
} t311OdomFb;

typedef union {
    struct PREPACK {
        uint16_t temp;   /* temperature */
        uint16_t cur;   /* current */
        uint16_t vol;   /* voltage */
        RESERVE(8);
        uint8_t bat_SOC;
    } POSTPACK;
    uint8_t data[8];
} t361BMSFb;

typedef union {
    struct PREPACK {
        uint8_t tweak;
        RESERVE(56);
    } POSTPACK;
    uint8_t data[8];
} t42fTurnTweak;

struct Data {
    t211SysStatus           i211SysStatus;
    t221MoveCtrlFb          i221MoveCtrlFb;
    t111MoveCtrl            i111MoveCtrl;
    t251Motor1InfoFb        i251Motor1InfoFb;
    t252Motor2InfoFb        i252Motor2InfoFb;
    t253Motor3InfoFb        i253Motor3InfoFb;
    t254Motor4InfoFb        i254Motor4InfoFb;
    t261WarnFb              i261WarnFb;
    t421ModeCtrl            i421ModeCtrl;
    t121LightCtrl           i121LightCtrl;
    t311OdomFb              i311OdomFb;
    t361BMSFb               i361BMSFb;
    t42fTurnTweak           i42fTurnTweak;
};

enum class E421Mode: uint8_t {
    SPEED,      /* speed mode */
    TORQUE      /* torque mode */
};

enum class E121Light: uint8_t {
    OFF,        /* light off */
    ON          /* light on */
};

struct ActualData {
    struct {
        uint8_t recv_;
        uint8_t cur_status;
        uint8_t ctrl_mode;
        double bat_vol;
        uint8_t error_info;
        uint8_t parity;
    } i211SysStatus;

    struct {
        uint8_t recv_;
        double speed;
        double angular;
    } i221MoveCtrlFb, i111MoveCtrl;

    struct {
        uint8_t recv_;
        int16_t rpm;
        int32_t pos;
    } i251Motor1InfoFb, i252Motor2InfoFb, i253Motor3InfoFb, i254Motor4InfoFb;

    struct {
        uint8_t recv_;
        uint8_t steer_motor_warn;
        uint8_t motor1_warn;
        uint8_t motor2_warn;
        uint8_t bat_warn;       /* battery warn */
        uint8_t cabin1_temp;
        uint8_t cabin2_temp;
        uint8_t emer_stop;      /* emergency stop */
    } i261WarnFb;

    struct {
        uint8_t recv_;
        E421Mode mode;
    } i421ModeCtrl;

    struct {
        uint8_t recv_;
        E121Light front;
        E121Light rear;
        uint8_t parity;
    } i121LightCtrl;

    struct {
        uint8_t recv_;
        double left;
        double right;
    } i311OdomFb;

    struct {
        uint8_t recv_;
        uint8_t bat_SOC;
        uint16_t vol;       /* voltage */
        uint16_t cur;       /* current */
        uint16_t temp;      /* temperature */
    } i361BMSFb;

    struct {
        double tweak;
    } i42fTurnTweak;
};

int pack(uint32_t idx, Data& data, const ActualData& act_data, DevType dev_type, CANMsg& out);

static int _pack(Data& data, const ActualData& act_data, uint32_t idx, uint8_t* data_, size_t size);

uint32_t unpack(Data& data, ActualData& act_data, DevType dev_type, CANMsg& in);

static void _unpack(Data& data, ActualData& act_data, uint32_t idx, uint8_t* data_, size_t size);

int check_buf(uint32_t idx, ActualData& act_data);

} /* END coroco namespace */
#endif /* END COROCO_PARSER_H */