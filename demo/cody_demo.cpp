/*
 * Created on Sat Aug 10 2024
 *
 * Author: Joonchen Liau
 * Email: liaojuncheng123@foxmail.com
 *
 * Copyright (c) 2024 Tiante Intelligent Technology
 */

#include <iostream>
#include <unistd.h>

#include "ttzn_sdk/cody/tran.hpp"

#define DELAY 20000


int main() {
    cody::CanTran can_tran("/dev/ttyUSB0");

    while (true) {
        can_tran.data.i111MoveCtrl.speed = 0.3;
        can_tran.data.i111MoveCtrl.corner = 12;
        can_tran.send(ID_MoveCtrl);

        can_tran.data.i421ModeCtrl.mode = cody::E421Mode::SPEED;
        can_tran.send(ID_ModeCtrl);

        can_tran.data.i121LightCtrl.front = cody::E121Light::ON;
        can_tran.data.i121LightCtrl.rear = cody::E121Light::OFF;
        can_tran.send(ID_LightCtrl);

        printf("######################## BEGIN ############################## \n");
        can_tran.recv(ID_SysStatus);
        printf("0x211 SysStatus cur_status: %hhu ctrl_mode: %hhu \n \
                bat_vol: %.2lf error_info: %hhd parity: %hhd \n",
            can_tran.data.i211SysStatus.cur_status,
            can_tran.data.i211SysStatus.ctrl_mode,
            can_tran.data.i211SysStatus.bat_vol,
            can_tran.data.i211SysStatus.error_info,
            can_tran.data.i211SysStatus.parity
        );

        can_tran.recv(ID_MoveCtrlFb);
        printf("0x221 MoveCtrlFb speed: %.2lf corner: %.2lf \n",
            can_tran.data.i221MoveCtrlFb.speed,
            can_tran.data.i221MoveCtrlFb.corner
        );

        can_tran.recv(ID_Motor1InfoFb);
        printf("0x251 Motor1InfoFb rpm: %hd pos: %hd \n",
            can_tran.data.i251Motor1InfoFb.rpm,
            can_tran.data.i251Motor1InfoFb.pos
        );
        
        can_tran.recv(ID_Motor2InfoFb);
        printf("0x252 Motor2InfoFb rpm: %hd pos: %hd \n",
            can_tran.data.i252Motor2InfoFb.rpm,
            can_tran.data.i252Motor2InfoFb.pos
        );

        can_tran.recv(ID_WarnFb);
        printf("0x261 WarnFb steer_motor_warn: %hhu motor1_warn: %hhu motor2_warn: %hhu \n \
                bat_warn: %hhu cabin1_temp: %hhu cabin2_temp %hhu emer_stop: %hhu \n",
            can_tran.data.i261WarnFb.steer_motor_warn,
            can_tran.data.i261WarnFb.motor1_warn,
            can_tran.data.i261WarnFb.motor2_warn,
            can_tran.data.i261WarnFb.bat_warn,
            can_tran.data.i261WarnFb.cabin1_temp,
            can_tran.data.i261WarnFb.cabin2_temp,
            can_tran.data.i261WarnFb.emer_stop
        );

        can_tran.recv(ID_OdomFb);
        printf("0x311 OdomFb left: %.2lf right: %.2lf \n",
            can_tran.data.i311OdomFb.left,
            can_tran.data.i311OdomFb.right
        );

        can_tran.recv(ID_BMSFb);
        printf("0x361 BMSFb bat_SOC: %hhu vol: %hu cur: %hu tmp: %hu \n",
            can_tran.data.i361BMSFb.bat_SOC,
            can_tran.data.i361BMSFb.vol,
            can_tran.data.i361BMSFb.cur,
            can_tran.data.i361BMSFb.temp
        );
        printf("######################### END ############################### \n");

        usleep(DELAY);
    }

    return 0;
}