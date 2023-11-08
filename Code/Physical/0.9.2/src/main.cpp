#include "main.hpp"
#include "lemlib/api.hpp"
// #include "sylib/sylib.hpp"

#define DT_WHEEL_LF_PORT 11
#define DT_WHEEL_LM_PORT 12
#define DT_WHEEL_LB_PORT 13
#define DT_WHEEL_RF_PORT 20
#define DT_WHEEL_RM_PORT 19
#define DT_WHEEL_RB_PORT 18

#define WING_SOLINOID_PORT 'A'
#define INTAKE_SOLINOID_PORT 'B'
#define CATA_LIMIT_SWITCH_PORT 'C'

#define CATA_LEFT_PORT 1
#define CATA_RIGHT_PORT 2

#define INERTIAL_SENSOR_PORT 14

pros::Controller master (pros::E_CONTROLLER_MASTER);

pros::Motor dt_motor_lf (DT_WHEEL_LF_PORT, pros::E_MOTOR_GEARSET_06, false, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor dt_motor_lm (DT_WHEEL_LM_PORT, pros::E_MOTOR_GEARSET_06, false, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor dt_motor_lb (DT_WHEEL_LB_PORT, pros::E_MOTOR_GEARSET_06, false, pros::E_MOTOR_ENCODER_DEGREES);

pros::Motor dt_motor_rf (DT_WHEEL_RF_PORT, pros::E_MOTOR_GEARSET_06, true, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor dt_motor_rm (DT_WHEEL_RM_PORT, pros::E_MOTOR_GEARSET_06, true, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor dt_motor_rb (DT_WHEEL_RB_PORT, pros::E_MOTOR_GEARSET_06, true, pros::E_MOTOR_ENCODER_DEGREES);

pros::Motor_Group dt_left ({dt_motor_lf, dt_motor_lm, dt_motor_lb});
pros::Motor_Group dt_right ({dt_motor_rf, dt_motor_rm, dt_motor_rb});

pros::Motor cata_left_motor (CATA_LEFT_PORT, pros::E_MOTOR_GEARSET_18, false, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor cata_right_motor (CATA_RIGHT_PORT, pros::E_MOTOR_GEARSET_18, true, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor_Group cata_motors ({cata_left_motor, cata_right_motor});

pros::ADIDigitalOut wing_solinoid (WING_SOLINOID_PORT);
pros::ADIDigitalOut intake_solinoid (INTAKE_SOLINOID_PORT);
pros::ADIDigitalIn cata_limit_switch (CATA_LIMIT_SWITCH_PORT);

bool intake_solinoid_toggle { false };
bool wing_solinoid_toggle { false };
bool cata_countinous_toggle { false };

pros::Imu inertial_sensor (INERTIAL_SENSOR_PORT);

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

// Create an addrled object
// auto addrled = sylib::Addrled(1,1,64);
 
// Set the LED strip to a gradient in HSV color space
// that displays a full range of hues
// addrled.gradient(0xFF0000, 0xFF0005, 0, 0, false, true);
 
// Cycle the colors at speed 10
// addrled.cycle(*addrled, 10);

void screen() {
    // loop forever
    while (true) {
        lemlib::Pose pose = chassis.getPose(); // get the current position of the robot
        pros::lcd::print(0, "x: %f", pose.x); // print the x position
        pros::lcd::print(1, "y: %f", pose.y); // print the y position
        pros::lcd::print(2, "heading: %f", pose.theta); // print the heading
        pros::delay(20);
    }
}

void initialize() {
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

void cata_shoot_once() {
	if (master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_R1)) {
		cata_motors.move_velocity(100);
		while (!cata_limit_switch.get_new_press()) { pros::delay(5); }
		cata_motors.move_velocity(0);
	}
}

void opcontrol() {

	pros::Task screenTask(screen);

	while (true) {
		int power = master.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
	    int turn = master.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X);
	    int left = power + turn;
	    int right = power - turn;
	    dt_left.move(left);
	    dt_right.move(right);

		if (master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_L2)) {
			intake_solinoid.set_value(!intake_solinoid_toggle);
			intake_solinoid_toggle = !intake_solinoid_toggle;
    	}

		if (master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_R2)) {
			wing_solinoid.set_value(!wing_solinoid_toggle);
			wing_solinoid_toggle = !wing_solinoid_toggle;
    	}

		if (master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_A)) {
			if (cata_countinous_toggle) {cata_motors.move_velocity(200);} else {cata_motors.move_velocity(0);}
			cata_countinous_toggle = !cata_countinous_toggle;
    	}

	    pros::delay(5);
	}
}