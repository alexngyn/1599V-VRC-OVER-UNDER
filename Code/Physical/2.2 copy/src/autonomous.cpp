#include "api.hpp"
#include "pros/rtos.hpp"
#include "setup.hpp"

ASSET(path1_txt);
ASSET(path2_txt);
ASSET(path3_txt);
ASSET(path4_txt);
ASSET(path5_txt);
ASSET(path6_txt);
ASSET(path7_txt);

double meter_to_in(double meter) { return meter * 39.37008; }

void logginator() {
    while (true) {
        lemlib::telemetrySink()->debug("{},{},{}", 0,
                                       (dt_motor_lb.get_actual_velocity() + dt_motor_rb.get_actual_velocity()) / 2,
                                       (dt_motor_lb.get_target_velocity() + dt_motor_rb.get_target_velocity()) / 2);
        lemlib::telemetrySink()->debug("{},{}", 1, (dt_motor_lb.get_position() + dt_motor_rb.get_position()) / 2);
        lemlib::Pose posel = chassis.getPose();
        lemlib::telemetrySink()->debug("{},{}", 2, posel.theta);
        pros::delay(20 ? CONTROLLER_MODE == bluetooth : 100);
    }
};

enum side { left, right, both };

void wings(bool state, side side = both) {
    if (side == left or side == both) { wing_right_solenoid.set_value(state); }
    if (side == right or side == both) { wing_left_solenoid.set_value(state); }
}

void auton_1() {
    chassis.setPose(-36.5, -61.5, 315);

    //wings(true, right);

    // go to match load
    chassis.moveToPoint(-50, -42, 4000, true, 127, false);
    //wings(false, right);
    chassis.turnTo(-60,-44, 1000, true, 127, false);
    // chassis.moveToPose(-62, -46, 250, 4000, {.forwards = true, .maxSpeed = 128}, false);
    

    // // shoot
    // puncher_motor.move_relative(1 * 360, 128);
    // pros::delay(1000);

    // // movement 1
    // chassis.follow(path1_txt, 10, 5000, false, false);
    // chassis.follow(path2_txt, 10, 3000, true, false);
    // // wings(true, left);
    // chassis.follow(path3_txt, 10, 5000, false, false);
    // // wings(false, left);
    // chassis.follow(path4_txt, 10, 5000, true, false);
    // chassis.turnTo(20, 20, 1000, false, 127, false);
    // wings(true, right);
    // chassis.follow(path5_txt, 10, 5000, false, false);
    // wings(false, right);
    // chassis.follow(path6_txt, 10, 5000, true, false);
    // wings(false, both);
    // chassis.follow(path7_txt, 10, 5000, false, false);
}

void auton_2() {}
