#include "api.hpp"
#include "pros/motors.h"
#include "pros/rtos.hpp"
#include "setup.hpp"

//ASSET(path1_txt);

double meter_to_in (double meter) { return meter * 39.37008; }

void set_pose_gps() {
    status_main = gps_main.get_status();
    chassis.setPose(meter_to_in(status_main.x), meter_to_in(status_main.y), gps_main.get_heading());
    pros::delay(50);
}

void update_pose_gps(bool wait) {
    pros::delay(wait);
    set_pose_gps();
}

void auton_1() {
    dt_left.set_brake_modes(pros::E_MOTOR_BRAKE_HOLD);
    dt_right.set_brake_modes(pros::E_MOTOR_BRAKE_HOLD);

    chassis.setPose(-46,60,135);
    puncher_motor.move_velocity(100);
    pros::delay(45000);
    puncher_motor.move_velocity(0);
    chassis.moveToOld(-40,55, 1000, false, false, 48);
    chassis.moveToOld(36,60, 1000, false, false, 48);
    chassis.moveToOld(58,30, 1000, false, false, 48);
    chassis.moveToOld(48,46, 1000, true, false, 48);
    chassis.moveToOld(8,0, 1000, true, false, 48);
    wing_solenoid.set_value(true);
    chassis.moveToOld(40,0, 1000, false, false, 48);

    //chassis.turnTo(-60, -360, 1000);
    // chassis.moveTo(0,12, 0, 10000, true, 2, 0.6, 128, false);
    // chassis.moveTo(12,24, 0, 10000, true, 2, 0.6, 128, false);
    // chassis.moveTo(0,0, 0, 10000, false, 2, 0.6, 128, false);

    // chassis.moveToOld(0,12, 1000, true, false, 48);
    // chassis.turnTo(12, 24, 1000, true, 48, false);
    // chassis.moveToOld(12,24, 1000, true, false, 48);
    // chassis.moveToOld(0,12, 1000, false, false, 36);
    // chassis.turnTo(0, 0, 1000, false, 48, false);
    // chassis.moveToOld(0,0, 10000, false, false, 48);
    //chassis.turnTo(0, 12, 1000, true, 96);
    // chassis.moveToOld(0,12, 10000, true, false, 127);
    // chassis.turnTo(12, 24, 10000, true, 127, false);
    // chassis.moveToOld(12,24, 10000, true, false, 127);
    // pros::delay(10000);
    // dt_right.brake();
    // dt_left.brake();
    // chassis.moveToOld(0,12, 10000, false, false, 127);
    // chassis.turnTo(0, 0, 10000, false, 127, false);
   //  chassis.moveToOld(0,0, 1000, false, false, 127);
    //chassis.turnTo(0, 12, 1000, true, 127);
    //chassis.turnTo(12, 12, 10000, true, 128, false);
    //chassis.moveToOld(12,12, 1000, true, false, 64);
    //chassis.moveToOld(float x, float y, int timeout, bool forwards = true, bool async = false, float maxSpeed = 127)
    //chassis.turnTo(0, -12, 1000,true, 96);
    //chassis.turnTo(0, 12, 1000, true, 96);
    //pros::delay(3000);
    //chassis.moveToPose(0,0, 180, 10000, false, 2, 0.6, 32, true);
    //while (true) {
        //lemlib::telemetrySink()->debug("{},{},{}", 0, (dt_motor_lb.get_actual_velocity() + dt_motor_rb.get_actual_velocity()) / 2, (dt_motor_lb.get_target_velocity() + dt_motor_rb.get_target_velocity()) / 2);
        //linearPID.log();
        //angularPID
        //pros::delay(20);    
    //}
    //lemlib::telemetrySink()->debug("{},{},{}", 0, dt_motor_rb.get_actual_velocity(), dt_motor_rb.get_target_velocity());
    //lemlib::telemetrySink()->debug("{},{}", 0, puncher_motor.get_temperature());

    //pros::lcd::print(3, "Theta: %f", 4);
    //pros::delay(20000);    
}

// void auton_2() {
//     chassis.setPose(-36, -60, 20);
//     chassis.moveToPose(-60, -30, 0, 1000, false);
//     wing_solinoid.set_value(true);
//     chassis.waitUntil(20);
//     wing_solinoid.set_value(false);

//     chassis.moveToPose(-56, -52, 243.435, 1000);
//     chassis.waitUntilDone();

//     puncher_motor.move_velocity(100);
//     pros::delay(10000);
//     puncher_motor.move_velocity(0);

//     chassis.follow(path1_txt, 15, 4000, false);
// }

// void auton_2() { }