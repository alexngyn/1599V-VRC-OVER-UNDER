#pragma once

#include "api.hpp"

#define DT_WHEEL_LF_PORT 18
#define DT_WHEEL_LM_PORT 19
#define DT_WHEEL_LB_PORT 20
#define DT_WHEEL_RF_PORT 11
#define DT_WHEEL_RM_PORT 12
#define DT_WHEEL_RB_PORT 13

#define WING_SOLINOID_PORT 'D'

#define AUTON_PORT 'A'

#define PUNCHER_PORT 1

#define INTAKE_LEFT_PORT 1
#define INTAKE_RIGHT_PORT 2

#define INERTIAL_SENSOR_PORT 14

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

extern lemlib::Drivetrain_t drivetrain;

extern lemlib::OdomSensors_t sensors;

extern lemlib::ChassisController_t lateralController;
	
extern lemlib::ChassisController_t angularController;

extern lemlib::Chassis chassis;