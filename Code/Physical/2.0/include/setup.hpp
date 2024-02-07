#pragma once

#include "api.hpp"

#define DT_WHEEL_LF_PORT 10
#define DT_WHEEL_LM_PORT 9
#define DT_WHEEL_LB_PORT 8
#define DT_WHEEL_RF_PORT 1
#define DT_WHEEL_RM_PORT 2
#define DT_WHEEL_RB_PORT 3

#define WING_SOLENOID_PORT 'H'
#define BLOCKER_SOLENOID_PORT 'G'

#define PUNCHER_PORT 5

#define INTAKE_PORT 6

#define GPS_MAIN_PORT 19
//#define GPS_SECONDARY_PORT 7
#define INERTIAL_SENSOR_PORT 20

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

extern pros::ADIDigitalOut wing_solenoid;
extern pros::ADIDigitalOut blocker_solenoid;

// extern pros::ADIDigitalIn auton_jumper;

extern pros::Motor intake_motor;

extern pros::Imu inertial_sensor;

extern lemlib::Chassis chassis;

extern pros::Gps gps_main;
//extern pros::Gps gps_secondary;

extern pros::c::gps_status_s_t status_main;
//extern pros::c::gps_status_s_t status_secondary;