#include "main.h"
#include "lemlib/api.hpp"

#define DT_WHEEL_LF_PORT 1
#define DT_WHEEL_LM_PORT 2
#define DT_WHEEL_LB_PORT 3
#define DT_WHEEL_RF_PORT 4
#define DT_WHEEL_RM_PORT 5
#define DT_WHEEL_RB_PORT 6

#define INTAKE_SOLINOID_PORT 'A'
#define WING_SOLINOID_PORT 'B'

#define INERTIAL_SENSORE_PORT 8

void screen() {
    // loop forever
    while (true) {
        lemlib::Pose pose = chassis.getPose(); // get the current position of the robot
        pros::lcd::print(0, "x: %f", pose.x); // print the x position
        pros::lcd::print(1, "y: %f", pose.y); // print the y position
        pros::lcd::print(2, "heading: %f", pose.theta); // print the heading
        pros::delay(10);
    }
}

void initialize() {
	pros::Controller master (pros::E_CONTROLLER_MASTER);

	pros::Motor dt_motor_lf (DT_WHEEL_LF_PORT, E_MOTOR_GEARSET_06, false, E_MOTOR_ENCODER_DEGREES);
	pros::Motor dt_motor_lm (DT_WHEEL_LM_PORT, E_MOTOR_GEARSET_06, false, E_MOTOR_ENCODER_DEGREES);
	pros::Motor dt_motor_lb (DT_WHEEL_LB_PORT, E_MOTOR_GEARSET_06, true, E_MOTOR_ENCODER_DEGREES);

	pros::Motor dt_motor_rf (DT_WHEEL_RF_PORT, E_MOTOR_GEARSET_06, true, E_MOTOR_ENCODER_DEGREES);
	pros::Motor dt_motor_rm (DT_WHEEL_RM_PORT, E_MOTOR_GEARSET_06, false, E_MOTOR_ENCODER_DEGREES);
	pros::Motor dt_motor_rb (DT_WHEEL_RB_PORT, E_MOTOR_GEARSET_06, false, E_MOTOR_ENCODER_DEGREES);

	pros::Motor_Group dt_left ({dt_motor_lf, dt_motor_lm, dt_motor_lb});
	pros::Motor_Group dt_right ({dt_motor_rf, dt_motor_rm, dt_motor_rb});

	pros::ADIDigitalOut wing_solidnoid (WING_SOLINOID_PORT);
	pros::ADIDigitalOut intake_solidnoid (INTAKE_SOLINOID_PORT);

	bool intake_solinoid_toggle { false };
	bool wing_solinoid_toggle { false };

	pros::Imu inertial_sensor (INERTIAL_SENSORE_PORT);

	lemlib::Drivetrain_t drivetrain {
	    &dt_left, // left drivetrain motors
	    &dt_right, // right drivetrain motors
	    10.625, // track width
	    3.25, // wheel diameter
	    360 // wheel rpm
	};

	lemlib::OdomSensors_t sensors {
	    nullptr, // vertical tracking wheel 1
	    nullptr, // vertical tracking wheel 2
	    nullptr, // horizontal tracking wheel 1
	    nullptr, // we don't have a second tracking wheel, so we set it to nullptr
	    &inertial_sensor // inertial sensor
	};

	// forward/backward PID
	lemlib::ChassisController_t lateralController {
	    8, // kP
	    30, // kD
	    1, // smallErrorRange
	    100, // smallErrorTimeout
	    3, // largeErrorRange
	    500, // largeErrorTimeout
	    5 // slew rate
	};
	
	// turning PID
	lemlib::ChassisController_t angularController {
	    4, // kP
	    40, // kD
	    1, // smallErrorRange
	    100, // smallErrorTimeout
	    3, // largeErrorRange
	    500, // largeErrorTimeout
	    0 // slew rate
	};

	lemlib::Chassis chassis(drivetrain, lateralController, angularController, sensors);

	//chassis.setPose(0, 0, 0); // X: 0, Y: 0, Heading: 0
    //chassis.setPose(5.2, 10.333, 87); // X: 5.2, Y: 10.333, Heading: 87

	pros::lcd::initialize(); // initialize brain screen
    chassis.calibrate(); // calibrate the chassis
    pros::Task screenTask(screen); // create a task to print the position to the screen
}

void disabled() {}
void competition_initialize() {}

void autonomous() {
    chassis.turnTo(10, 0, 1000, false, 50); // turn to the point (10, 0) with a timeout of 1000 ms, and a maximum speed of 50
    chassis.moveTo(10, 0, 1000, 50); // move to the point (10, 0) with a timeout of 1000 ms, and a maximum speed of 50

	 // file name: path.txt
    // timeout: 2000 ms
    // lookahead distance: 15 inches
    chassis.follow("path.txt", 2000, 15);
    // follow the next path, but with the robot going backwards
    chassis.follow("path2.txt", 2000, 15, true);
}

void opcontrol() {
	while (true) {
		int power = master.get_analog(E_CONTROLLER_ANALOG_LEFT_Y);
	    int turn = master.get_analog(E_CONTROLLER_ANALOG_RIGHT_X);
	    int left = power + turn;
	    int right = power - turn;
	    dt_left.move(left);
	    dt_right.move(right);

		if (master.get_digital_new_press(E_CONTROLLER_DIGITAL_A)) {
			intake_solidnoid.set(!intake_solinoid_toggle);
			intake_solinoid_toggle = !intake_solinoid_toggle
    	}

		if (master.get_digital_new_press(E_CONTROLLER_DIGITAL_B)) {
			wing_solidnoid.set(!wing_solinoid_toggle);
			wing_solinoid_toggle = !wing_solinoid_toggle
    	}

	    pros::delay(5);
	}
}