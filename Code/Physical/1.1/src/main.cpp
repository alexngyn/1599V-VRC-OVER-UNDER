#include "main.hpp"

void screen() {
    while (true) {
        lemlib::Pose pose = chassis.getPose(); // get the current position of the robot
        pros::lcd::print(0, "x: %f", pose.x); // print the x position
        pros::lcd::print(1, "y: %f", pose.y); // print the y position
        pros::lcd::print(2, "heading: %f", pose.theta); // print the heading
        pros::delay(20);
    }
}

void initialize() {
	cata_motors.set_brake_modes(pros::E_MOTOR_BRAKE_HOLD);
	chassis.setPose(0, 0, 0); // X: 0, Y: 0, Heading: 0
    chassis.calibrate(); // calibrate the chassis
    pros::lcd::initialize(); // initialize brain screen
    pros::Task screenTask(screen); // create a task to print the position to the screen
	pros::Task lights(addrled); // task for leds
	printf("int");
}

void disabled() {}
void competition_initialize() {}

void autonomous() {
	cata_motors.move_relative(1080, 100);

	if (auton_jumper.get_value()) {
		auton_1();
	} else {
		auton_2();
	}
}

void opcontrol() {
	pros::Task drive_thread(drive);
	pros::Task cata_thread(cata);
	pros::Task wings_thread(wings);
	pros::Task intake_thread(intake);
}
