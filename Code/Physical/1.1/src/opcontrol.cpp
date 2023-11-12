#include "api.hpp"
#include "setup.hpp"

static bool intake_solinoid_toggle { false };
static bool wing_solinoid_toggle { false };
static bool cata_countinous_toggle { false };
static bool reverse { false };

static std::pair<double, double> arcade_drive(double ithrottle, double iturn) {
	double left, right;

	right = left = ithrottle * 10;
	right += iturn * 10;
	left -= iturn * 10;

	return std::make_pair(left, right);
}

void drive() {
	while (true) {
		double power = master.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
		double turn = master.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X);

		if (master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_X)) {
			reverse = !reverse;
    	}
		if (reverse) {power = -power;}
	    
		auto [left, right] = arcade_drive(power, turn);

	    dt_left.move_voltage(left);
	    dt_right.move_voltage(right);

		pros::delay(5);
	}
}

void cata() {
	while (true) {
		if (cata_countinous_toggle) {cata_motors.move_velocity(200);} else {cata_motors.move_velocity(0);}

		while (master.get_digital(pros::E_CONTROLLER_DIGITAL_L2) & !cata_countinous_toggle) {
			cata_motors.move_velocity(100);
		}

		if (master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_A)) {
			cata_countinous_toggle = !cata_countinous_toggle;
    	}

		while (master.get_digital(pros::E_CONTROLLER_DIGITAL_B) & !cata_countinous_toggle) {
			cata_motors.move_velocity(-100);
    	}
		pros::delay(20);
	}
}

void wings() {
	while (true) {
		while (master.get_digital(pros::E_CONTROLLER_DIGITAL_R2) & !wing_solinoid_toggle) {
			wing_solinoid.set_value(true);
    	}
		if (!wing_solinoid_toggle) {wing_solinoid.set_value(false);};
		if (master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_R1)) {
			wing_solinoid.set_value(!wing_solinoid_toggle);
			wing_solinoid_toggle = !wing_solinoid_toggle;
    	}
		pros::delay(20);
	}
}

void intake() {
	while (true) {
		if (master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_L1)) {
			intake_solinoid.set_value(!intake_solinoid_toggle);
			intake_solinoid_toggle = !intake_solinoid_toggle;
    	}
		pros::delay(20);
	}
}