#include "vex.h"

using namespace vex;

// A global instance of brain used for printing to the V5 brain screen
brain Brain;

vex::controller con1 = controller(vex::controllerType::primary);

vex::motor       front_right_motor  = vex::motor(vex::PORT1, vex::gearSetting::ratio6_1, true);
vex::motor       middle_right_motor = vex::motor(vex::PORT2, vex::gearSetting::ratio6_1, true);
vex::motor       back_right_motor   = vex::motor(vex::PORT3, vex::gearSetting::ratio6_1, false);
vex::motor       front_left_motor   = vex::motor(vex::PORT11, vex::gearSetting::ratio6_1, false);
vex::motor       middle_left_motor  = vex::motor(vex::PORT12, vex::gearSetting::ratio6_1, false);
vex::motor       back_left_motor    = vex::motor(vex::PORT13, vex::gearSetting::ratio6_1, true);
vex::motor_group left_motor_group   = vex::motor_group(front_left_motor, middle_left_motor, back_left_motor);
vex::motor_group right_motor_group  = vex::motor_group(front_right_motor, middle_right_motor, back_right_motor);
vex::digital_out wing = digital_out(Brain.ThreeWirePort.A);


void vexcodeInit(void) {
  // Nothing to initialize
  /*
  Brain.Screen.print("Device initialization...");
  Brain.Screen.setCursor(2, 1);
  // calibrate the drivetrain GPS
  wait(200, msec);
  GPS.calibrate();
  Brain.Screen.print("Calibrating GPS for Drivetrain");
  // wait for the GPS calibration process to finish
  while (GPS.isCalibrating()) {
    wait(25, msec);
  }
  wait(50, msec);
  */
}