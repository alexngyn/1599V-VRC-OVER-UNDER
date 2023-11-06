#include "main.h"
#define DT_WHEEL_LF_PORT 1
#define DT_WHEEL_LM_PORT 2
#define DT_WHEEL_LB_PORT 3
#define DT_WHEEL_RF_PORT 4
#define DT_WHEEL_RM_PORT 5
#define DT_WHEEL_RB_PORT 6

void initialize() {
	pros::Controller master (pros::E_CONTROLLER_MASTER);

	pros::Motor dt_motor_lf (DT_WHEEL_LF_PORT, E_MOTOR_GEARSET_06, false, E_MOTOR_ENCODER_DEGREES);
	pros::Motor dt_motor_lm (DT_WHEEL_LM_PORT, E_MOTOR_GEARSET_06, false, E_MOTOR_ENCODER_DEGREES);
	pros::Motor dt_motor_lb (DT_WHEEL_LB_PORT, E_MOTOR_GEARSET_06, true, E_MOTOR_ENCODER_DEGREES);

	pros::Motor dt_motor_rf (DT_WHEEL_RF_PORT, E_MOTOR_GEARSET_06, true, E_MOTOR_ENCODER_DEGREES);
	pros::Motor dt_motor_rm (DT_WHEEL_RM_PORT, E_MOTOR_GEARSET_06, false, E_MOTOR_ENCODER_DEGREES);
	pros::Motor dt_motor_rb (DT_WHEEL_RB_PORT, E_MOTOR_GEARSET_06, false, E_MOTOR_ENCODER_DEGREES);

	pros::Motor_Group dt_left ({motor_lf, motor_2});
	pros::Motor_Group dt_right ({motor_1, motor_2});
}

void disabled() {}
void competition_initialize() {}

void autonomous() {
	right_wheels.move_relative(1000, 600);
	left_wheels.move_relative(1000, 600);
}

void opcontrol() {
	while (true) {
		int power = master.get_analog(ANALOG_LEFT_Y);
	    int turn = master.get_analog(ANALOG_RIGHT_X);
	    int left = power + turn;
	    int right = power - turn;
	    dt_left.move(left);
	    dt_right.move(right);

	    pros::delay(5);
	}
}
