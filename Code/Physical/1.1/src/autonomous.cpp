#include "api.hpp"
#include "setup.hpp"

void auton_1() {
    cata_motors.move_relative(1360, 100);
	chassis.moveTo(0, 5, 1000, 50);
	chassis.follow("path.txt", 2000, 15);

	intake_solinoid.set_value(false);
	chassis.moveTo(0, 10, 1000, 50);

	intake_solinoid.set_value(true);
	chassis.moveTo(0, 14, 1000, 50);

	intake_solinoid.set_value(false);

	chassis.moveTo(0, 4, 1000, 50);
}

void auton_2() {

}