#include "api.hpp"
#include "setup.hpp"


// get a path used for pure pursuit
// this needs to be put outside a function
ASSET(file1_txt);

void matchload(int amount) {
    int timeout = 1000;
    float maxSpeed = 127*0.65;
    float kF = 0;
    float kA = 0;
    float kP = 1;
    float kI = 0;
    float kD = 1;
    float largeError = 3;
    float smallError = 1;
    float largeErrorTimeout = 500;
    float smallErrorTimeout = 100;
    float targetTheta;
    float deltaTheta;
    float motorPower;
    float startTheta = puncher_motor.get_position();

    lemlib::FAPID pid = lemlib::FAPID(kF, kA, kP, kI, kD, "matchloadPID");
    pid.setExit(largeError, smallError, largeErrorTimeout, smallErrorTimeout, timeout);

    targetTheta = puncher_motor.get_position() + 360*amount;

    while (!pid.settled()) {
        // calculate deltaTheta
        deltaTheta = lemlib::angleError(targetTheta, puncher_motor.get_position());

        // calculate the speed
        motorPower = pid.update(0, deltaTheta);

        // cap the speed
        if (motorPower > maxSpeed) motorPower = maxSpeed;
        else if (motorPower < -maxSpeed) motorPower = -maxSpeed;

        puncher_motor.move(motorPower);

        pros::delay(10);
    }
    puncher_motor.move(0);
}

void auton_1() {
    //chassis.turnTo(-60, -360, 1000);
    chassis.moveTo(-60, 36, 0, 1000);
    // chassis.turnTo(0, -180, 10000);
    // chassis.moveTo(-60, -30, 0, 1000);
    // chassis.waitUntil(20);
    // intake_motors.move(127);
    // chassis.waitUntilDone();
    // intake_motors.move(0);
    // chassis.moveTo(-42, -45, 243.435, 1000, false);
    // chassis.moveTo(-56, -52, 243.435, 1000);
    // chassis.waitUntilDone();
    // matchload(22);
    //chassis.follow(file1_txt, 15, 4000, false);
    //chassis.moveTo(-50, -52, 325, 1000);
    //chassis.follow(file2_txt, 15, 4000, false);
    //wing_solinoid.set_value(true);
    //chassis.waitUntil(30);
    //wing_solinoid.set_value(false);
    // chassis.moveTo(8, -24, 180, 1000);
    // chassis.moveTo(8, 24, 180, 1000);
    
    
    
}

// void auton_2() {
// 	// example movement: Move to x: 20 and y:15, and face heading 90. Timeout set to 4000 ms
//     chassis.moveTo(20, 15, 90, 4000);
//     // move to the point (10, 0) at heading 270 with a timeout of 1000 ms.
//     // Move in reverse
//     chassis.moveTo(10, 0, 270, 1000, false);

// 	// example movement: Turn to face the point x:45, y:-45. Timeout set to 1000
//     chassis.turnTo(45, -45, 1000, true, 60);
    
// 	// turn to the point (53, 53) with a timeout of 1000 ms
//     chassis.turnTo(53, 53, 1000);
//     // turn to the point (-20, 32) with the back of the robot facing the point, and a timeout of 1500 ms
//     chassis.turnTo(-20, 32, 1500, true);
//     // turn to the point (10, 0) with a timeout of 1000 ms, and a maximum speed
//     // of 50, with the front of the robot facing the point
//     chassis.turnTo(10, 0, 1000, false, 50);


// 	// example movement: Follow the path in path.txt. Lookahead at 15, Timeout set to 4000
//     // following the path with the back of the robot (forwards = false)
//     // see line 116 to see how to define a path
//     //chassis.follow(example_txt, 15, 4000, false);
//     // wait until the chassis has traveled 10 inches. Otherwise the code directly after
//     // the movement will run immediately
//     // Unless its another movement, in which case it will wait
//     chassis.waitUntil(10);
//     pros::lcd::print(4, "Travelled 10 inches during pure pursuit!");
//     // wait until the movement is done
//     chassis.waitUntilDone();
//     pros::lcd::print(4, "pure pursuit finished!");
// }

// void auton_2() { }