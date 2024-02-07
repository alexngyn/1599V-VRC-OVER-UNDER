#include "api.hpp"
#include "pros/rtos.hpp"
#include "setup.hpp"
#include <algorithm>

ASSET(path_txt);
ASSET(path1_txt);
ASSET(path2_txt);
ASSET(path3_txt);
ASSET(path4_txt);

double meter_to_in (double meter) { return meter * 39.37008; }

void logginator() {
    while (true) {
        lemlib::telemetrySink()->debug("{},{},{}", 0, (dt_motor_lb.get_actual_velocity() + dt_motor_rb.get_actual_velocity()) / 2, (dt_motor_lb.get_target_velocity() + dt_motor_rb.get_target_velocity()) / 2);
        lemlib::telemetrySink()->debug("{},{}", 1, (dt_motor_lb.get_position() + dt_motor_rb.get_position()) / 2);
        lemlib::Pose posel = chassis.getPose();
        lemlib::telemetrySink()->debug("{},{}", 2, posel.theta);
        pros::delay(20 ? CONTROLLER_MODE == bluetooth : 100); 
    }
};

enum side { left, right, both }; 

void wings(bool state, side side=both) {
    if (side == left or side == both) {
        wing_right_solenoid.set_value(state); }
    if (side == right or side == both) {
        wing_left_solenoid.set_value(state); }
}

void auton_1() {
    chassis.setPose(-36,-60,0);

    //go to match load
    chassis.moveToPose(-60, -44, 250, 4000, {.forwards = true, .maxSpeed = 100}, false);

    //shoot
    puncher_motor.move_relative(48*360, 128);
    pros::delay(30000);

    //movement 1
    chassis.moveToPose(-24, -60, -90, 4000, {.forwards = false, .maxSpeed = 100, .minSpeed = 48}, false);
    chassis.moveToPose(38, -60, -90, 4000, {.forwards = false, .maxSpeed = 128, .minSpeed = 100}, true);
    chassis.waitUntil(48);
    wings(true, right);
    chassis.moveToPose(60, -30, 180, 8000, {.forwards = false, .maxSpeed = 128, .minSpeed = 64}, false);
    pros::delay(200);
    chassis.moveToPose(60, -32, 180, 8000, {.forwards = true, .maxSpeed = 96, .minSpeed = 48}, false);
    wings(false, right);
}

void auton_2() {
} 