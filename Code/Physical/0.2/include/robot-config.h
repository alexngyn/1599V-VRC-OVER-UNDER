#pragma once
using namespace vex;

extern brain Brain;

extern controller con1;

extern motor front_right_motor;
extern motor middle_right_motor;
extern motor back_right_motor;
extern motor front_left_motor;
extern motor middle_left_motor;
extern motor back_left_motor;

extern motor_group left_motor_group;
extern motor_group right_motor_group;

extern digital_out wing;

void vexcodeInit(void);
