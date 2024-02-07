#include "main.hpp"
#include "pros/misc.h"
#include "pros/motors.h"
#include "pros/rtos.hpp"
#include "setup.hpp"

void initialize() {
	puncher_motor.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
    chassis.calibrate(); // calibrate the chassis
    chassis.setPose(0, 0, 0); // X: 0, Y: 0, Heading: 0
    pros::lcd::initialize(); // initialize brain screen
    pros::Task screen([&]() {
        while (true) {
            lemlib::Pose pose = chassis.getPose(); // get chassis position
            pros::lcd::print(0, "X: %f", pose.x);
            pros::lcd::print(1, "Y: %f", pose.y);
            pros::lcd::print(2, "Theta: %f", pose.theta);
            pros::delay(20 ? CONTROLLER_MODE : 50);
        }
    });
    pros::delay(1000);
	printf("init");
}

void disabled() {}
void competition_initialize() {}

void autonomous() {
    //pros::Task logger_thread(logginator);

    dt_left.set_brake_modes(pros::E_MOTOR_BRAKE_HOLD);
    dt_right.set_brake_modes(pros::E_MOTOR_BRAKE_HOLD);
    
	auton_2();
}

void opcontrol() {
    dt_left.set_brake_modes(pros::E_MOTOR_BRAKE_COAST);
    dt_right.set_brake_modes(pros::E_MOTOR_BRAKE_COAST);

	pros::Task drive_thread(drive);
    pros::Task puncher_thread(puncher);
	pros::Task wings_thread(wings);
	pros::Task intake_thread(intake);
}
