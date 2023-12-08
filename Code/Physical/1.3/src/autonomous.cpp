#include "api.hpp"
#include "setup.hpp"

void auton_1() {
	chassis.moveTo(0, 5, 1000, 50);
	//chassis.follow("path.txt", 2000, 15);
}

// void auton_2() { }