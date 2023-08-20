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
#include <algorithm>
#include <cmath>

using namespace vex;

vex::competition Competition;

Drive robot(left_motor_group, right_motor_group, 4.125, 0.428571);

int current_auton_selection = 0;
bool auto_started = false;

void pre_auton(void) {
  vexcodeInit();
  while(auto_started == false){
    Brain.Screen.clearScreen();  
    switch(current_auton_selection){
      case 0:
        Brain.Screen.printAt(50, 50, "Auton 1");
        break;
      case 1:
        Brain.Screen.printAt(50, 50, "Auton 2");
        break;
      case 2:
        Brain.Screen.printAt(50, 50, "Auton 3");
        break;
    }
    if(Brain.Screen.pressing()){
      while(Brain.Screen.pressing()) {}
      current_auton_selection ++;
    } else if (current_auton_selection == 3){
      current_auton_selection = 0;
    }
    task::sleep(10);
  }
}

void autonomous(void) {
  auto_started = true;
  switch(current_auton_selection){  
    case 0:
      auton_1();
      break;        
    case 1:         
      auton_2();
      break;
    case 2:
      auton_3();
      break;
  }
}

void usercontrol(void) {
  while (true) {
    //robot.control_arcade();
    double leftPos = con1.Axis2.position(pct);
    double rightPos = con1.Axis4.position(pct);

    // double leftPos = scurve(con1.Axis2.position(pct));
    // double rightPos = scurve(con1.Axis4.position(pct));
  
  
    double left  = leftPos + rightPos;
    double right = leftPos - rightPos;

    left_motor_group.spin(directionType::fwd, left, pct);
    right_motor_group.spin(directionType::fwd,right, pct);
    task::sleep(10);
  }
}

int main() {
  //Competition.autonomous(autonomous);
  Competition.drivercontrol(usercontrol);

  //pre_auton();

  while (true) {
    task::sleep(10);
  }
}
