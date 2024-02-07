#include "api.hpp"
#include "pros/rtos.hpp"
#include "setup.hpp"
#include <algorithm>

ASSET(path_txt);
ASSET(path1_txt);
ASSET(path2_txt);
ASSET(path3_txt);
ASSET(path4_txt);

//double meter_to_in (double meter) { return meter * 39.37008; }

void logginator() {
    while (true) {
        lemlib::telemetrySink()->debug("{},{},{}", 0, (dt_motor_lb.get_actual_velocity() + dt_motor_rb.get_actual_velocity()) / 2, (dt_motor_lb.get_target_velocity() + dt_motor_rb.get_target_velocity()) / 2);
        lemlib::telemetrySink()->debug("{},{}", 1, (dt_motor_lb.get_position() + dt_motor_rb.get_position()) / 2);
        lemlib::Pose posel = chassis.getPose();
        lemlib::telemetrySink()->debug("{},{}", 2, posel.theta);
        pros::delay(100 ? CONTROLLER_MODE : 100); 
    }
};

enum side { left, right, both }; 

void wings(bool state, side side=both) {
    if (side == left or side == both) {
        wing_right_solenoid.set_value(state); }
    if (side == right or side == both) {
        wing_left_solenoid.set_value(state); }
}

void drive(float rotation, int velocity, int wait) {
    dt_left.move_relative(rotation, velocity);
    dt_right.move_relative(rotation, velocity);
    pros::delay(wait);
    // puncher_motor.move_relative(360*44, 128);
    // pros::delay(30000);
}

void auton_1() {
    chassis.setPose(-36,-60,0);

    //go to match load
    chassis.moveToPose(-60, -44, 250, 4000, {.forwards = true, .maxSpeed = 100}, false);

    puncher_motor.move_relative(48*360, 128);
    pros::delay(30000);

    //chassis.setPose(12,-60,-90);

    //movement 1
    chassis.moveToPose(-24, -60, -90, 4000, {.forwards = false, .maxSpeed = 100, .minSpeed = 48}, false);
    chassis.moveToPose(38, -60, -90, 4000, {.forwards = false, .maxSpeed = 100, .minSpeed = 48}, true);
    chassis.waitUntil(48);
    wings(true, right);
    chassis.moveToPose(60, -30, 180, 4000, {.forwards = false, .maxSpeed = 128, .minSpeed = 64}, false);
    pros::delay(200);
    drive(1,200,500);
    //chassis.moveToPose(60, -40, 180, 8000, {.forwards = true, .maxSpeed = 96, .minSpeed = 36}, false);
    //pros::delay(200);
    drive(-1.5,600,500);
    //drive(500,-12);
    //chassis.moveToPose(60, -30, 180, 8000, {.forwards = false, .maxSpeed = 128, .minSpeed = 120}, false);
    //pros::delay(200);
    // chassis.moveToPose(60, -32, 180, 8000, {.forwards = true, .maxSpeed = 96, .minSpeed = 48}, false);
    wings(false, right);

    // //movement 2
    chassis.follow(path1_txt, 10, 4000, true, false);
    chassis.turnTo(24, -24, 1000, false);
    wings(true);
    chassis.follow(path2_txt, 10, 4000, false, false);
    wings(false, left);
    chassis.moveToPose(50, -10, -90, 4000, {.forwards = false, .maxSpeed = 128, .minSpeed = 64}, false);
    pros::delay(200);
    //drive(500,8);
    drive(1,200,500);
    //chassis.moveToPose(40, -10, -90, 4000, {.forwards = true, .maxSpeed = 96, .minSpeed = 36}, false);
    //pros::delay(200);
    //drive(500,-12);
    //chassis.moveToPose(55, -10, -90, 4000, {.forwards = false, .maxSpeed = 128, .minSpeed = 120}, false);
    //pros::delay(200);
    drive(-1.5,600,500);
    //chassis.moveToPose(42, -10, -90, 4000, {.forwards = false, .maxSpeed = 128, .minSpeed = 120}, false);
    wings(false, right);

    chassis.follow(path3_txt, 10, 1000, true, false);
    wings(true);
    chassis.moveToPose(55, 4, -90, 4000, {.forwards = false, .maxSpeed = 128, .minSpeed = 64}, false);
    //pros::delay(200);
    drive(1,200,500);
    //chassis.moveToPose(40, 4, -90, 4000, {.forwards = true, .maxSpeed = 96, .minSpeed = 36}, false);
    //pros::delay(200);
    //drive(500,-12);
    drive(-1.5,600,500);
    //chassis.moveToPose(42, 4, -90, 4000, {.forwards = false, .maxSpeed = 128, .minSpeed = 120}, false);
    //pros::delay(200);
    wings(false);

    //movement 4
    chassis.moveToPose(30, -4, 160, 1000, {.forwards = true, .maxSpeed = 64}, false);
    wings(true);
    chassis.follow(path4_txt, 10, 4000, false, true);
    chassis.waitUntil(55);
    wings(false, left);
    chassis.waitUntilDone();
    drive(1,200,500);
    drive(-1.5,600,500);
    //chassis.moveToPose(60, 30, -180, 8000, {.forwards = false, .maxSpeed = 128, .minSpeed = 100}, false);
} 