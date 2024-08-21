/*
 * Created on Tue Aug 20 2024
 *
 * Author: Joonchen Liau
 * Email: liaojuncheng123@foxmail.com
 *
 * Copyright (c) 2024 Tiante Intelligent Technology
 */

#include <iostream>
#include <unistd.h>

#include "ttzn_sdk/coroco/tran.hpp"

using namespace coroco;

#define DELAY 20000


int main() {
    CANTran can_tran("/dev/ttyUSB0", DevType::USB_TTL_CAN);

    while (true) {
        // can_tran.data.i111MoveCtrl.speed = 0.3;
        // can_tran.data.i111MoveCtrl.angular = 0.4;
        // can_tran.send(ID_MoveCtrl);

        // can_tran.data.i421ModeCtrl.mode = E421Mode::IDEL;
        // can_tran.send(ID_ModeCtrl);

        // can_tran.data.i121LightCtrl.front = E121Light::ON;
        // can_tran.send(ID_LightCtrl);

        printf("######################## BEGIN ############################## \n");
        can_tran.recv(ID_SysStatus);
        printf("0x211 SysStatus cur_status: %hhu ctrl_mode: %hhu bat_vol: %.2lf \n \
                bat_cur: %.2lf parity: %hhd \n",
            can_tran.data.i211SysStatus.cur_status,
            can_tran.data.i211SysStatus.ctrl_mode,
            can_tran.data.i211SysStatus.bat_vol,
            can_tran.data.i211SysStatus.bat_cur,
            can_tran.data.i211SysStatus.parity
        );

        can_tran.recv(ID_MoveCtrlFb);
        printf("0x221 MoveCtrlFb speed: %.2lf angular: %.2lf \n",
            can_tran.data.i221MoveCtrlFb.speed,
            can_tran.data.i221MoveCtrlFb.angular
        );
        
        can_tran.recv(ID_ReMoveCtrlFb);
        printf("0x241 ReMoveCtrlFb speed: %.2lf angular: %.2lf \n",
            can_tran.data.i241ReMoveCtrlFb.speed,
            can_tran.data.i241ReMoveCtrlFb.angular
        );

        can_tran.recv(ID_Motor1InfoFb);
        printf("0x250 Motor1InfoFb rpm: %hd pos: %hu \n",
            can_tran.data.i250Motor1InfoFb.rpm,
            can_tran.data.i250Motor1InfoFb.pos
        );
        
        can_tran.recv(ID_Motor2InfoFb);
        printf("0x251 Motor2InfoFb rpm: %hd pos: %hu \n",
            can_tran.data.i251Motor2InfoFb.rpm,
            can_tran.data.i251Motor2InfoFb.pos
        );

        can_tran.recv(ID_Motor3InfoFb);
        printf("0x252 Motor3InfoFb rpm: %hd pos: %hu \n",
            can_tran.data.i252Motor3InfoFb.rpm,
            can_tran.data.i252Motor3InfoFb.pos
        );

        can_tran.recv(ID_Motor4InfoFb);
        printf("0x253 Motor4InfoFb rpm: %hd pos: %hu \n",
            can_tran.data.i253Motor4InfoFb.rpm,
            can_tran.data.i253Motor4InfoFb.pos
        );

        can_tran.recv(ID_WarnFb);
        printf("0x261 WarnFb steer_motor_warn: %hhu motor1_warn: %hhu motor2_warn: %hhu \n \
                bat_warn: %hhu temp1: %hhd temp2: %hhd warn: %hhu \n",
            can_tran.data.i261WarnFb.steer_motor_warn,
            can_tran.data.i261WarnFb.motor1_warn,
            can_tran.data.i261WarnFb.motor2_warn,
            can_tran.data.i261WarnFb.bat_warn,
            can_tran.data.i261WarnFb.temp1,
            can_tran.data.i261WarnFb.temp2,
            can_tran.data.i261WarnFb.warn
        );

        can_tran.recv(ID_OdomFb);
        printf("0x311 OdomFb odom: %.4lf \n",
            can_tran.data.i311OdomFb.odom
        );

        can_tran.recv(ID_BMSFb);
        printf("0x361 BMSFb bat_SOC: %hhu vol: %hu cur: %hd temp: %hd \n",
            can_tran.data.i361BMSFb.bat_soc,
            can_tran.data.i361BMSFb.vol,
            can_tran.data.i361BMSFb.cur,
            can_tran.data.i361BMSFb.temp
        );
        printf("######################### END ############################### \n");

        usleep(DELAY);
    }

    return 0;
}
