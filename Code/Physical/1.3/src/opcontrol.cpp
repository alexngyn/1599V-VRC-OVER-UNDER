#include "api.hpp"
#include "setup.hpp"

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
	    
		auto [left, right] = arcade_drive(power, turn);

	    dt_left.move_voltage(left);
	    dt_right.move_voltage(right);

		pros::delay(5);
	}
}