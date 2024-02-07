#include "main.hpp"
#include "pros/gps.hpp"

pros::Controller master (pros::E_CONTROLLER_MASTER);

pros::Motor dt_motor_lf (DT_WHEEL_LF_PORT, pros::E_MOTOR_GEARSET_06, true, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor dt_motor_lm (DT_WHEEL_LM_PORT, pros::E_MOTOR_GEARSET_06, true, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor dt_motor_lb (DT_WHEEL_LB_PORT, pros::E_MOTOR_GEARSET_06, true, pros::E_MOTOR_ENCODER_DEGREES);

pros::Motor dt_motor_rf (DT_WHEEL_RF_PORT, pros::E_MOTOR_GEARSET_06, false, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor dt_motor_rm (DT_WHEEL_RM_PORT, pros::E_MOTOR_GEARSET_06, false, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor dt_motor_rb (DT_WHEEL_RB_PORT, pros::E_MOTOR_GEARSET_06, false, pros::E_MOTOR_ENCODER_DEGREES);

pros::Motor_Group dt_left ({dt_motor_lb});
pros::Motor_Group dt_right ({dt_motor_rb});

pros::Motor puncher_motor (PUNCHER_PORT, pros::E_MOTOR_GEARSET_36, false, pros::E_MOTOR_ENCODER_DEGREES);

pros::ADIDigitalOut wing_solenoid (WING_SOLENOID_PORT);

pros::ADIDigitalOut blocker_solenoid (BLOCKER_SOLENOID_PORT);

pros::Motor intake_motor (INTAKE_PORT, pros::E_MOTOR_GEARSET_06, true, pros::E_MOTOR_ENCODER_DEGREES);

pros::Gps gps_main (GPS_MAIN_PORT, -0.05, 0.2);
//pros::Gps gps_secondary (GPS_SECONDARY_PORT, 0.05, 0.18);

pros::c::gps_status_s_t status_main;

//pros::ADIDigitalIn auton_jumper (AUTON_PORT);

pros::Imu inertial_sensor (INERTIAL_SENSOR_PORT);

// drivetrain settings
lemlib::Drivetrain drivetrain {
    &dt_left, // left drivetrain motors
    &dt_right, // right drivetrain motors
    13, // track width
    lemlib::Omniwheel::NEW_325, // using new 3.25" omnis
    360, // drivetrain rpm is 360
    2 // chase power is 2. If we had traction wheels, it would have been 8
};

// lateral motion controller
lemlib::ControllerSettings linearController {
    40, // proportional gain (kP)
    0, // derivative gain (kD)
    1, // small error range, in inches
    100, // small error range timeout, in milliseconds
    3, // large error range, in inches
    500, // large error range timeout, in milliseconds
    400 // maximum acceleration (slew)
};

// angular motion controller
lemlib::ControllerSettings angularController {
    5, // proportional gain (kP)
    55, // derivative gain (kD)
    1, // small error range, in degrees
    100, // small error range timeout, in milliseconds
    3, // large error range, in degrees
    500, // large error range timeout, in milliseconds
    80 // maximum acceleration (slew)
};

// sensors for odometry
// note that in this example we use internal motor encoders, so we don't pass vertical tracking wheels
lemlib::OdomSensors sensors {
    nullptr, // vertical tracking wheel 1, set to nullptr as we don't have one
    nullptr, // vertical tracking wheel 2, set to nullptr as we don't have one
    nullptr, // horizontal tracking wheel 1
    nullptr, // horizontal tracking wheel 2, set to nullptr as we don't have a second one
    &inertial_sensor // inertial sensor
};

// create the chassis
lemlib::Chassis chassis(drivetrain, linearController, angularController, sensors);