#include "main.hpp"

pros::Controller master (pros::E_CONTROLLER_MASTER);

pros::Motor dt_motor_lf (DT_WHEEL_LF_PORT, pros::E_MOTOR_GEARSET_06, false, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor dt_motor_lm (DT_WHEEL_LM_PORT, pros::E_MOTOR_GEARSET_06, false, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor dt_motor_lb (DT_WHEEL_LB_PORT, pros::E_MOTOR_GEARSET_06, false, pros::E_MOTOR_ENCODER_DEGREES);

pros::Motor dt_motor_rf (DT_WHEEL_RF_PORT, pros::E_MOTOR_GEARSET_06, true, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor dt_motor_rm (DT_WHEEL_RM_PORT, pros::E_MOTOR_GEARSET_06, true, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor dt_motor_rb (DT_WHEEL_RB_PORT, pros::E_MOTOR_GEARSET_06, true, pros::E_MOTOR_ENCODER_DEGREES);

pros::Motor_Group dt_left ({dt_motor_lf, dt_motor_lm, dt_motor_lb});
pros::Motor_Group dt_right ({dt_motor_rf, dt_motor_rm, dt_motor_rb});

pros::Motor puncher_motor (PUNCHER_PORT, pros::E_MOTOR_GEARSET_18, false, pros::E_MOTOR_ENCODER_DEGREES);

pros::ADIDigitalOut wing_solinoid (WING_SOLINOID_PORT);

pros::Motor intake_left (INTAKE_LEFT_PORT, pros::E_MOTOR_GEARSET_06, false, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor intake_right (INTAKE_RIGHT_PORT, pros::E_MOTOR_GEARSET_06, true, pros::E_MOTOR_ENCODER_DEGREES);

pros::Motor_Group intake_motors ({intake_left, intake_left});

//pros::ADIDigitalIn auton_jumper (AUTON_PORT);

pros::Imu inertial_sensor (INERTIAL_SENSOR_PORT);

lemlib::Drivetrain_t drivetrain {
    &dt_left, // left drivetrain motors
    &dt_right, // right drivetrain motors
    10.625, // track width
    3.25, // wheel diameter
    360 // wheel rpm
};

lemlib::OdomSensors_t sensors {
    nullptr, // vertical tracking wheel 1
    nullptr, // vertical tracking wheel 2
    nullptr, // horizontal tracking wheel 1
    nullptr, // we don't have a second tracking wheel, so we set it to nullptr
    &inertial_sensor // inertial sensor
};

// forward/backward PID
lemlib::ChassisController_t lateralController {
    8, // kP
    30, // kD
    1, // smallErrorRange
    100, // smallErrorTimeout
    3, // largeErrorRange
    500, // largeErrorTimeout
    5 // slew rate
};
	
// turning PID
lemlib::ChassisController_t angularController {
    4, // kP
    40, // kD
    1, // smallErrorRange
    100, // smallErrorTimeout
    3, // largeErrorRange
    500, // largeErrorTimeout
    0 // slew rate
};

lemlib::Chassis chassis(drivetrain, lateralController, angularController, sensors);