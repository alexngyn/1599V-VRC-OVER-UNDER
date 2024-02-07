/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       VEX                                                       */
/*    Created:      Thu Sep 26 2019                                           */
/*    Description:  Competition Template                                      */
/*                                                                            */
/*----------------------------------------------------------------------------*/

// ---- START VEXCODE CONFIGURED DEVICES ----
// ---- END VEXCODE CONFIGURED DEVICES ----

#include "vex.h"
#include <cmath>
#include <string>

using namespace vex;

// A global instance of competition
competition Competition;

// define your global instances of motors and other devices here

float expected;
float error;
float actual;
float speed;
float coefficient;

bool reversey;
float scurveVal;
float xPos;
float yPos;
int autonToRun;
double dir;

inertial TurnGyroSmart(PORT20);
//drive functions

void reverse_dir(){
  
  Controller1.Screen.clearScreen();
  
  reversey = !reversey;
  
  if (reversey){
    Controller1.Screen.setCursor(1,1);
    Controller1.Screen.print("front: cata/wings");
  }
  else{
    Controller1.Screen.setCursor(1,1);
    Controller1.Screen.print("front: sleds/intake");
  }

}

// float scurve(float x, float k){
//   scurveVal = (exp(-k/10)+(exp((std::fabs(x)-100)/10))*(1-exp(-k/10)))*(12*x/100);
//   return scurveVal;
// }

void wings_expand(){
  piston_wings.set(true);
}

void wings_retract(){
  piston_wings.set(false);
}

// void blocker_up(){
//   piston_blocker.set(true);
// }

// void blocker_down(){
//   piston_blocker.set(false);
// }

void blocker(){
  piston_blocker.set(!piston_blocker.value());
}

void cata_launch(){
  puncher.setVelocity(200,rpm);
  puncher.spinFor(vex::directionType::fwd, 1, rev);
}

//auton functions

void my_turn(double expected, double coefficient){

  expected = expected + Inertial.rotation(degrees);
  
  double error = expected;
  
  while (std::abs(error) > 2){
    actual = Inertial.rotation(degrees);
    error = expected - actual;
    speed = error * coefficient;
    
    left_motor_group.setVelocity(speed, percent);
    right_motor_group.setVelocity(-1 * speed, percent);

    left_motor_group.spin(forward);
    right_motor_group.spin(forward);
    
    wait(150, msec);
  }

  left_motor_group.stop();
  right_motor_group.stop();

}

void drive_forwards(double distance){
  //drives forward distance mm. Circumference of wheel in mm = 3.25π(in) * 25.4(cm/in)
  left_motor_group.setVelocity(600, rpm);
  right_motor_group.setVelocity(600, rpm);
  double revolutions = distance / (3.25 * 2.54 * M_PI);
  left_motor_group.spinFor(revolutions, rev, false);
  right_motor_group.spinFor(revolutions, rev, true);
}

void go_to (double xFinal, double yFinal){
  
  // calculating the differences
  double deltaX = xFinal - xPos;
  double deltaY = yFinal - yPos;

  double mag = sqrt(pow(deltaX, 2) + pow(deltaY, 2));
  dir = atan(deltaX/deltaY) * 180 / M_PI;

  if (deltaY < 0){
    dir += 180;
  }

  my_turn(dir, 0.6);
  drive_forwards(mag);

  xPos = xFinal;
  yPos = yFinal;
  
}

void same_auton(){
  xPos = -90;
  yPos = -150;

  intake.spinFor(fwd, 1, rev, false);

  drive_forwards(180);
  
  my_turn(-90, 0.6);
  intake.spinFor(reverse, 1, rev, false);
  drive_forwards(40);
  wait(100, msec);

  drive_forwards (-30);
  drive_forwards(40);

  // wings_retract();

  // my_turn(126.87, 0.6);

  // drive_forwards(100);
  // my_turn(-36.87, 0.6);
  // drive_forwards(60);
}

void opp_auton(){
  // xPos = 90;
  // yPos = -150;

  drive_forwards(180);
  
  my_turn(90, 0.6);


  intake.spin(reverse);
  drive_forwards(30);
  wait(100, msec);
  drive_forwards(-30);

  my_turn(170, 0.6);
  intake.spin(forward);
  drive_forwards(69.64);
  xPos = 25;
  yPos = 60;

  my_turn(180, 0.6);
  intake.spin(reverse);
  drive_forwards(50);
  drive_forwards(-50);
  
  my_turn(-90, 0.6);
  drive_forwards(90);

  intake.spin(forward);
  my_turn(-60, 0.6);
  wings_expand();
  drive_forwards(-110);
  my_turn(180, 0.6);
  intake.spin(reverse);
  drive_forwards(20);

}

void skill_auton(){
  puncher.setVelocity(100,rpm);
  puncher.spin(forward);
  wait(5, sec);
  puncher.stop();
  my_turn(-30, 0.6);
  drive_forwards(-210);
  my_turn(45, 0.6);
  wings_expand();
  drive_forwards(-90);
}

/*---------------------------------------------------------------------------*/
/*                          Pre-Autonomous Functions                         */
/*                                                                           */
/*  You may want to perform some actions before the competition starts.      */
/*  Do them in the following function.  You must return from this function   */
/*  or the autonomous and usercontrol tasks will not be started.  This       */
/*  function is only called once after the V5 has been powered on and        */
/*  not every time that the robot is disabled.                               */
/*---------------------------------------------------------------------------*/

void pre_auton(void) {
  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();

  intake.stop(hold);

  piston_wings.set(false);
  piston_blocker.set(false);

  left_motor_group.setVelocity(600, rpm);
  right_motor_group.setVelocity(600, rpm);

  // All activities that occur before the competition starts
  // Example: clearing encoders, setting servo positions, ...
}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              Autonomous Task                              */
/*                                                                           */
/*  This task is used to control your robot during the autonomous phase of   */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */
/*---------------------------------------------------------------------------*/

void autonomous(void) {
  // ..........................................................................
  // Insert autonomous user code here.
  // ..........................................................................
  piston_wings.set(false);
  piston_blocker.set(false);
  
  intake.setVelocity(600, rpm);

  skill_auton();

}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              User Control Task                            */
/*                                                                           */
/*  This task is used to control your robot during the user control phase of */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */
/*---------------------------------------------------------------------------*/

float driveCurve(float input, float scale) {
    if (scale != 0) {
        return (powf(2.718, -(scale / 10)) + powf(2.718, (fabs(input) - 100) / 10) * (1 - powf(2.718, -(scale / 10)))) * 0.12 * input;
    }
    return input;
}

void usercontrol(void) {
  // User control code here, inside the loop

  piston_wings.set(false);
  
  reversey = false;
  Controller1.ButtonX.pressed(reverse_dir);
  
  //config
  intake.setVelocity(600, rpm);
  puncher.setVelocity(200, rpm);

  //wings
  Controller1.ButtonR2.pressed(wings_expand);
  Controller1.ButtonR2.released(wings_retract);

  Controller1.ButtonA.pressed(blocker);
  
  while (1) {
    // This is the main execution loop for the user control program.
    // Each time through the loop your program should update motor + servo
    // values based on feedback from the joysticks.

    // ........................................................................
    // Insert user code here. This is where you use the joystick values to
    // update your motors, etc.
    // ........................................................................

    //arcade drive


    // Throttle is how fast we want to be moving foward or backward.
    // Turn is how fast we want to spin about the center.
    // double throttle = Controller1.Axis3.position() * 12 / 100;
    // double turn = Controller1.Axis1.position() * 12 / 100;
    double throttle = driveCurve(Controller1.Axis3.position(),4.3);
    double turn = driveCurve(Controller1.Axis1.position(),4.3);
    
    double leftVal = throttle + turn;
    double rightVal = throttle - turn;

    if (reversey){
      leftVal = -1 * (throttle - turn);
      rightVal = -1 * (throttle + turn);
    }

    left_motor_group.spin(directionType::fwd,leftVal, voltageUnits::volt);
    right_motor_group.spin(directionType::fwd,rightVal, voltageUnits::volt);

    if (Controller1.ButtonR1.pressing()){
      puncher.spin(vex::directionType::fwd, 80, rpm);
    }
    else if (Controller1.ButtonB.pressing()){
      puncher.spin(vex::directionType::rev);
    }
    else{
      puncher.stop(brakeType::hold);
    }

    if (Controller1.ButtonL2.pressing()){
      intake.spin(vex::directionType::fwd);
    }
    else if (Controller1.ButtonL1.pressing()){
      intake.spin(vex::directionType::rev);
    }
    else{
      intake.stop();
    }

    wait(5, msec); // Sleep the task for a short amount of time to
                    // prevent wasted resources.
  }
}

//
// Main will set up the competition functions and callbacks.
//
int main() {
  // Set up callbacks for autonomous and driver control periods.
  Competition.autonomous(autonomous);
  Competition.drivercontrol(usercontrol);

  // Run the pre-autonomous function.
  pre_auton();

  // Prevent main from exiting with an infinite loop.
  while (true) {
    wait(100, msec);
  }
}
