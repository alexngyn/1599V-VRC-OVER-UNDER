#include "vex.h"

using namespace vex;

// A global instance of brain used for printing to the V5 brain screen
brain Brain;

vex::controller   Controller1       = vex::controller(primary);

vex::motor       front_right_motor  = vex::motor(vex::PORT3, vex::gearSetting::ratio6_1, false);
vex::motor       middle_right_motor = vex::motor(vex::PORT2, vex::gearSetting::ratio6_1, false);
vex::motor       back_right_motor   = vex::motor(vex::PORT1, vex::gearSetting::ratio6_1, false);
vex::motor       front_left_motor   = vex::motor(vex::PORT8, vex::gearSetting::ratio6_1, true);
vex::motor       middle_left_motor  = vex::motor(vex::PORT9, vex::gearSetting::ratio6_1, true);
vex::motor       back_left_motor    = vex::motor(vex::PORT10, vex::gearSetting::ratio6_1, true);
vex::motor_group left_motor_group   = vex::motor_group(front_left_motor, middle_left_motor, back_left_motor);
vex::motor_group right_motor_group  = vex::motor_group(front_right_motor, middle_right_motor, back_right_motor);

vex::motor       intake       = vex::motor(vex::PORT6, vex::gearSetting::ratio6_1, true);

vex::motor       puncher          = vex::motor(vex::PORT5, vex::gearSetting::ratio36_1, false);

vex::digital_out piston_wings      = digital_out(Brain.ThreeWirePort.H);
vex::digital_out piston_blocker      = digital_out(Brain.ThreeWirePort.G);

vex::inertial Inertial              = inertial(vex::PORT20);

/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Pro.
 *
 * This should be called at the start of your int main function.
 */
void vexcodeInit(void) {
  // Nothing to initialize
}