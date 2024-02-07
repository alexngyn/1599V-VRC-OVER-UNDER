#pragma once
using namespace vex;

extern brain Brain;

extern controller Controller1;

extern motor front_right_motor;
extern motor middle_right_motor;
extern motor back_right_motor;
extern motor front_left_motor;
extern motor middle_left_motor;
extern motor back_left_motor;
extern motor puncher;
extern motor intake_right;
extern motor intake_left;

extern motor_group left_motor_group;
extern motor_group right_motor_group;

extern motor intake;

extern digital_out piston_wings;
extern digital_out piston_blocker;

extern limit cata_switch;
extern inertial Inertial;

void vexcodeInit(void);
