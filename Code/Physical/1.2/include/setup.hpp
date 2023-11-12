#pragma once

#include "api.hpp"

#define DT_WHEEL_LF_PORT 11
#define DT_WHEEL_LM_PORT 12
#define DT_WHEEL_LB_PORT 13
#define DT_WHEEL_RF_PORT 20
#define DT_WHEEL_RM_PORT 19
#define DT_WHEEL_RB_PORT 18

#define DRIVE_DEADBAND 16 // deadband
#define DRIVE_SLEW 0.01 //acceleration
#define CD_NEG_INERTIA_SCALAR 0.1 // acceleration
#define CD_SENSITIVITY 0.5  //chesy turn

#define WING_SOLINOID_PORT 'D'
#define INTAKE_SOLINOID_PORT 'C'

#define AUTON_PORT 'A'

#define CATA_LEFT_PORT 1
#define CATA_RIGHT_PORT 2

#define INERTIAL_SENSOR_PORT 14

#define LED_PORT 'E'
#define LED_LENGTH 60

extern pros::c::adi_led_t led_strip;

extern pros::Controller master;

extern pros::Motor dt_motor_lf;
extern pros::Motor dt_motor_lm;
extern pros::Motor dt_motor_lb;

extern pros::Motor dt_motor_rf;
extern pros::Motor dt_motor_rm;
extern pros::Motor dt_motor_rb;

extern pros::Motor_Group dt_left;
extern pros::Motor_Group dt_right;

extern pros::Motor cata_left_motor;
extern pros::Motor cata_right_motor;
extern pros::Motor_Group cata_motors;

extern pros::ADIDigitalOut wing_solinoid;
extern pros::ADIDigitalOut intake_solinoid;

extern pros::ADIDigitalIn auton_jumper;

extern pros::Imu inertial_sensor;

extern lemlib::Drivetrain_t drivetrain;

extern lemlib::OdomSensors_t sensors;

extern lemlib::ChassisController_t lateralController;
	
extern lemlib::ChassisController_t angularController;

extern lemlib::Chassis chassis;