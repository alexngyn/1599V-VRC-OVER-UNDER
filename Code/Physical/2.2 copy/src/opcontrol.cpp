#include "api.hpp"
#include "setup.hpp"

bool reversey = false;

float driveCurve(float input, float scale) {
    if (scale != 0) {
        return (powf(2.718, -(scale / 10)) + powf(2.718, (fabs(input) - 127) / 10) * (1 - powf(2.718, -(scale / 10)))) * input;
    }
    return input;
}

std::pair<float, float> arcade(int throttle, int turn, float curveGain = 0) {
    turn *= 0.8;
    float leftPower = driveCurve(throttle, curveGain) + (turn * 0.7);
    float rightPower = driveCurve(throttle, curveGain)  - (turn * 0.7);
    return std::make_pair(leftPower, rightPower);
}

std::pair<float, float> curvature(int throttle, int turn, float curveGain) {
    if (abs(throttle) < 4) {
        return arcade(throttle, turn, curveGain);;
    }

    float leftPower = throttle + (std::abs(throttle) * turn) / 127.0;
    float rightPower = throttle - (std::abs(throttle) * turn) / 127.0;

    leftPower = driveCurve(leftPower, curveGain);
    rightPower = driveCurve(rightPower, curveGain);

	return std::make_pair(leftPower, rightPower);
}

void drive() {
	while (true) {
        if (master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_X)) { reversey = !reversey; }

		double power = master.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
		double turn = master.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X);

        if (reversey){ power *= -1; }
	    
		auto [left, right] = arcade(power, turn, 7.2);
        // auto [left, right] = curvature(power, turn, 7.2);

	    dt_left.move(left);
	    dt_right.move(right);

		pros::delay(10 ? CONTROLLER_MODE == bluetooth : 50);
	}
}

void puncher() {
    while (true) {
        if (master.get_digital(pros::E_CONTROLLER_DIGITAL_R1)) {
            puncher_motor.move_velocity(100);
        } else if (master.get_digital(pros::E_CONTROLLER_DIGITAL_B)) {
            puncher_motor.move_velocity(-100);
        } else {
            puncher_motor.move_velocity(0);
        }
        pros::delay(30 ? CONTROLLER_MODE == bluetooth : 50);
    }
}

void intake () {
    while (true) {
        if (master.get_digital(pros::E_CONTROLLER_DIGITAL_L2)) {
            intake_motor.move_velocity(600);
        } else if (master.get_digital(pros::E_CONTROLLER_DIGITAL_L1)) {
            intake_motor.move_velocity(-600);
        } else {
            intake_motor.move_velocity(0);
        }
        pros::delay(30 ? CONTROLLER_MODE == bluetooth : 50);
    }
}

bool wings_out = false;

void wings() {
    while (true) {
        if (master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_R2)) {
            wings_out = !wings_out;
            wing_left_solenoid.set_value(wings_out);
            wing_right_solenoid.set_value(wings_out);
        }
        pros::delay(30 ? CONTROLLER_MODE == bluetooth : 50);
    }
}

bool blocker_out = false;

void blocker() {
    while (true) {
        if (master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_A)) {
            blocker_out = !blocker_out;
            blocker_solenoid.set_value(blocker_out);
        }
        pros::delay(30 ? CONTROLLER_MODE == bluetooth : 50);
    }
}