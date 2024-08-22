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
    ID_ReMoveCtrlFb         = 0x241,
    ID_Motor1InfoFb         = 0x250,
    ID_Motor2InfoFb         = 0x251,
    ID_Motor3InfoFb         = 0x252,
    ID_Motor4InfoFb         = 0x253,
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
        int16_t bat_cur;        /* battery current */
        uint16_t bat_vol;       /* battery voltage */
        uint8_t ctrl_mode;      /* control mode */
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
typedef t221MoveCtrlFb t241ReMoveCtrlFb;

typedef union {
    struct PREPACK {
        uint32_t pos;
        RESERVE(16);
        int16_t rpm;
    } POSTPACK;
    uint8_t data[8];
} t250Motor1InfoFb;

typedef t250Motor1InfoFb t251Motor2InfoFb;
typedef t250Motor1InfoFb t252Motor3InfoFb;
typedef t250Motor1InfoFb t253Motor4InfoFb;


typedef union {
    struct PREPACK {
        RESERVE(8);
        uint8_t warn;           /* highest priority warning */
        int8_t temp2;
        int8_t temp1;
        uint8_t bat_warn;       /* battery warn */
        uint8_t motor2_warn;
        uint8_t motor1_warn;
        uint8_t steer_motor_warn;
    } POSTPACK;
    uint8_t data[8];
} t261WarnFb;

/* pin */
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
        RESERVE(48);
        uint8_t front;      /* front light */
    } POSTPACK;
    uint8_t data[8];
} t121LightCtrl;

typedef union {
    struct PREPACK {
        RESERVE(32);
        uint32_t odom;          /* left wheel odom */
    } POSTPACK;
    uint8_t data[8];
} t311OdomFb;

typedef union {
    struct PREPACK {
        int16_t temp;   /* temperature */
        int16_t cur;    /* current */
        uint16_t vol;   /* voltage */
        RESERVE(8);
        uint8_t bat_soc;
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
    t241ReMoveCtrlFb        i241ReMoveCtrlFb;
    t250Motor1InfoFb        i250Motor1InfoFb;
    t251Motor2InfoFb        i251Motor2InfoFb;
    t252Motor3InfoFb        i252Motor3InfoFb;
    t253Motor4InfoFb        i253Motor4InfoFb;
    t261WarnFb              i261WarnFb;
    t421ModeCtrl            i421ModeCtrl;
    t121LightCtrl           i121LightCtrl;
    t311OdomFb              i311OdomFb;
    t361BMSFb               i361BMSFb;
    t42fTurnTweak           i42fTurnTweak;
};

enum class E421Mode: uint8_t {
    IDEL,       /* idel mode */
    REMOTE,     /* remote control mode */
    CAN         /* CAN control mode */
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
        double bat_cur;
        uint8_t parity;
    } i211SysStatus;

    struct {
        uint8_t recv_;
        double speed;
        double angular;
    } i221MoveCtrlFb, i111MoveCtrl, i241ReMoveCtrlFb;

    struct {
        uint8_t recv_;
        int16_t rpm;
        uint32_t pos;
    } i250Motor1InfoFb, i251Motor2InfoFb, i252Motor3InfoFb, i253Motor4InfoFb;

    struct {
        uint8_t recv_;
        uint8_t steer_motor_warn;
        uint8_t motor1_warn;
        uint8_t motor2_warn;
        uint8_t bat_warn;       /* battery warn */
        int8_t temp1;
        int8_t temp2;
        uint8_t warn;           /* highest prioirty warning */
    } i261WarnFb;

    struct {
        uint8_t recv_;
        E421Mode mode;
    } i421ModeCtrl;

    struct {
        uint8_t recv_;
        E121Light front;
        uint8_t parity;
    } i121LightCtrl;

    struct {
        uint8_t recv_;
        double odom;
    } i311OdomFb;

    struct {
        uint8_t recv_;
        uint8_t bat_soc;
        uint16_t vol;       /* voltage */
        int16_t cur;        /* current */
        int16_t temp;       /* temperature */
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