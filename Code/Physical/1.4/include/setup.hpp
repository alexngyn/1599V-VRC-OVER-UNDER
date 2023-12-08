#pragma once

#include "api.hpp"

#define DT_WHEEL_LF_PORT 8
#define DT_WHEEL_LM_PORT 9
#define DT_WHEEL_LB_PORT 10
#define DT_WHEEL_RF_PORT 1
#define DT_WHEEL_RM_PORT 2
#define DT_WHEEL_RB_PORT 3

#define WING_SOLENOID_PORT 'D'

#define PUNCHER_PORT 7

#define INTAKE_LEFT_PORT 14
#define INTAKE_RIGHT_PORT 20

#define INERTIAL_SENSOR_PORT 21

extern pros::Controller master;

extern pros::Motor dt_motor_lf;
extern pros::Motor dt_motor_lm;
extern pros::Motor dt_motor_lb;

extern pros::Motor dt_motor_rf;
extern pros::Motor dt_motor_rm;
extern pros::Motor dt_motor_rb;

extern pros::Motor_Group dt_left;
extern pros::Motor_Group dt_right;

extern pros::Motor puncher_motor;

extern pros::ADIDigitalOut wing_solinoid;

// extern pros::ADIDigitalIn auton_jumper;

extern pros::Motor intake_left;
extern pros::Motor intake_right;

extern pros::Motor_Group intake_motors;

extern pros::Imu inertial_sensor;

extern lemlib::Chassis chassis;