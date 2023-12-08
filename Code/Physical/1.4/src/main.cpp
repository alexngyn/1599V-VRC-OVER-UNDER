#include "main.hpp"

// void screen() {
//     while (true) {
//         lemlib::Pose pose = chassis.getPose(); // get the current position of the robot
//         pros::lcd::print(0, "x: %f", pose.x); // print the x position
//         pros::lcd::print(1, "y: %f", pose.y); // print the y position
//         pros::lcd::print(2, "heading: %f", pose.theta); // print the heading
//         pros::delay(20);
//     }
// }

void initialize() {
	puncher_motor.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
    chassis.calibrate(); // calibrate the chassis
    chassis.setPose(-60, -52, 20); // X: 0, Y: 0, Heading: 0
    //chassis.setPose(-36, -30, 315);
    pros::lcd::initialize(); // initialize brain screen
    pros::Task screen([&]() {
        while (true) {
            lemlib::Pose pose = chassis.getPose(); // get chassis position
            pros::lcd::print(0, "X: %f", pose.x);
            pros::lcd::print(1, "Y: %f", pose.y);
            pros::lcd::print(2, "Theta: %f", pose.theta);
            pros::delay(20);
        }
    });
	printf("init");
}

void disabled() {}
void competition_initialize() {}

void autonomous() {
	auton_1();
}

void opcontrol() {
	pros::Task drive_thread(drive);
	// pros::Task cata_thread(cata);
	// pros::Task wings_thread(wings);
	// pros::Task intake_thread(intake);
}
