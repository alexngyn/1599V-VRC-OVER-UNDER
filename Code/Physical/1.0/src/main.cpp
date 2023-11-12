#include "main.hpp"
#include "lemlib/api.hpp"

#define DT_WHEEL_LF_PORT 11
#define DT_WHEEL_LM_PORT 12
#define DT_WHEEL_LB_PORT 13
#define DT_WHEEL_RF_PORT 20
#define DT_WHEEL_RM_PORT 19
#define DT_WHEEL_RB_PORT 18

#define DRIVE_DEADBAND 16 // deadband
#define DRIVE_SLEW 0.01 //acceleration
#define CD_NEG_INERTIA_SCALAR 0.1 // acceleration
#define CD_SENSITIVITY 0.5  //chesy turn

#define WING_SOLINOID_PORT 'D'
#define INTAKE_SOLINOID_PORT 'C'

#define CATA_LEFT_PORT 1
#define CATA_RIGHT_PORT 2

#define INERTIAL_SENSOR_PORT 14

#define AUTON_PORT 'A'

#define LED_PORT 'E'
#define LED_LENGTH 60

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

pros::ADIDigitalIn auton_jumper (AUTON_PORT);

pros::Imu inertial_sensor (INERTIAL_SENSOR_PORT);

bool intake_solinoid_toggle { false };
bool wing_solinoid_toggle { false };
bool cata_countinous_toggle { false };
bool reverse { false };

uint32_t ledbuffer[LED_LENGTH];
std::vector<uint32_t> ledbuffer_v;

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

void screen() {
    while (true) {
        lemlib::Pose pose = chassis.getPose(); // get the current position of the robot
        pros::lcd::print(0, "x: %f", pose.x); // print the x position
        pros::lcd::print(1, "y: %f", pose.y); // print the y position
        pros::lcd::print(2, "heading: %f", pose.theta); // print the heading
        pros::delay(20);
    }
}

struct rgb {
  double r;
  double g;
  double b;
};

std::uint32_t rgb_to_hex(int r, int g, int b) {
    return (((r & 0xff) << 16) + ((g & 0xff) << 8) + (b & 0xff));
}

rgb hex_to_rgb(std::uint32_t color) {
    rgb in;
    in.r = (color >> 16) & 0xff;
    in.g = (color >> 8) & 0xff;
    in.b = color & 0xff;
    return in;
}

uint32_t interpolate_rgb(std::uint32_t start_color, std::uint32_t end_color, int step,
                                       int fade_width) {
    rgb startComponents = hex_to_rgb(start_color);
    rgb endComponents = hex_to_rgb(end_color);

    double red_diff = endComponents.r - startComponents.r;
    double green_diff = endComponents.g - startComponents.g;
    double blue_diff = endComponents.b - startComponents.b;

    double red_step = red_diff / fade_width;
    double green_step = green_diff / fade_width;
    double blue_step = blue_diff / fade_width;

    rgb solved;

    solved.r = (startComponents.r + red_step * step);
    solved.g = (startComponents.g + green_step * step);
    solved.b = (startComponents.b + blue_step * step);
    return rgb_to_hex(solved.r, solved.g, solved.b);
}

void gradient(std::uint32_t start_color, std::uint32_t end_color, int fade_width) {
    for (int i = 0; i < fade_width; i++) {
    	ledbuffer_v[i] = interpolate_rgb(start_color, end_color, i, fade_width);
    }
	for (int i = 3; i < fade_width*2; i++) {
    	ledbuffer_v[i] = interpolate_rgb(end_color, start_color, i, fade_width);
    }
}

void addrled() {
	for(int i = 0;i<LED_LENGTH;i++){
		ledbuffer_v.push_back(0x00FF00);
	}
	pros::c::adi_led_t led = pros::c::adi_led_init(LED_PORT);
	pros::delay(1000);
	//ledbuffer = ledbuffer_v.data()
	//std::copy(ledbuffer_v.begin(), ledbuffer_v.end(), ledbuffer);
	//pros::c::adi_led_set(led, ledbuffer, LED_LENGTH);
	gradient(0xFFDA29, 0xC40233, 30);
	//gradient(0x00005F, 0x005F00, 30);

	std::copy(ledbuffer_v.begin(), ledbuffer_v.end(), ledbuffer);
	pros::c::adi_led_set(led, ledbuffer, LED_LENGTH);
	while (true) {
		//std::rotate(ledbuffer.begin(), ledbuffer.end() - 1, ledbuffer.end());
		std::rotate(ledbuffer_v.begin(), ledbuffer_v.begin() + 1, ledbuffer_v.end());
		std::copy(ledbuffer_v.begin(), ledbuffer_v.end(), ledbuffer);
		pros::c::adi_led_set(led, ledbuffer, LED_LENGTH);
		pros::delay(300);
	}
}

void initialize() {
	cata_motors.set_brake_modes(pros::E_MOTOR_BRAKE_HOLD);
	chassis.setPose(0, 0, 0); // X: 0, Y: 0, Heading: 0
    chassis.calibrate(); // calibrate the chassis
    pros::lcd::initialize(); // initialize brain screen
    pros::Task screenTask(screen); // create a task to print the position to the screen
	printf("int");
}

pros::Task lights(addrled);

void disabled() {}
void competition_initialize() {}

void autonomous() {
	//cata_motors.move_relative(1360, 100);
	//chassis.moveTo(0, 5, 1000, 50);
	//chassis.follow("path.txt", 2000, 15);

	cata_motors.move_relative(1080, 100);

	//intake_solinoid.set_value(false);
	//chassis.moveTo(0, 10, 1000, 50);

	//intake_solinoid.set_value(true);
	//chassis.moveTo(0, 14, 1000, 50);

	//intake_solinoid.set_value(false);

	//chassis.moveTo(0, 4, 1000, 50);

	// printf("%d",auton_jumper.get_value());
	// if (auton_jumper.get_value()) {
	// 	cata_motors.move_relative(1360, 100);

	// 	intake_solinoid.set_value(false);
	// 	chassis.moveTo(0, 20, 1000, 50);

	// 	intake_solinoid.set_value(true);
	// 	chassis.moveTo(0, 24, 1000, 50);
		
	// } else {
	// 	chassis.moveTo(0, 5, 1000, 50);
	// }
}

double _turnRemapping(double iturn, double CD_TURN_NONLINEARITY) {
	//double denominator = sin(M_PI / 2 * CD_TURN_NONLINEARITY);
	//double firstRemapIteration =
	//    sin(M_PI / 2 * CD_TURN_NONLINEARITY * iturn) / denominator;
	//return sin(M_PI / 2 * CD_TURN_NONLINEARITY * firstRemapIteration) / denominator;
	return (exp(-(CD_TURN_NONLINEARITY/10)) + (exp(std::abs(iturn)-128) * (1-exp(-(CD_TURN_NONLINEARITY/10))))) * ((12000 * iturn) / 128);
}

double quickStopAccumlator = 0.0;
double negInertiaAccumlator = 0.0;
void _updateAccumulators() {
	if (negInertiaAccumlator > 1) {
		negInertiaAccumlator -= 1;
	} else if (negInertiaAccumlator < -1) {
		negInertiaAccumlator += 1;
	} else {
		negInertiaAccumlator = 0;
	}

	if (quickStopAccumlator > 1) {
		quickStopAccumlator -= 1;
	} else if (quickStopAccumlator < -1) {
		quickStopAccumlator += 1;
	} else {
		quickStopAccumlator = 0.0;
	}
}

double prevTurn = 0.0;
double prevThrottle = 0.0;
std::pair<double, double> cheesyDrive(double ithrottle, double iturn) {
	bool turnInPlace = false;
	double linearCmd = ithrottle;
	if (fabs(ithrottle) < DRIVE_DEADBAND && fabs(iturn) > DRIVE_DEADBAND) {
		// The controller joysticks can output values near zero when they are
		// not actually pressed. In the case of small inputs like this, we
		// override the throttle value to 0.
		linearCmd = 0.0;
		turnInPlace = true;
	} else if (ithrottle - prevThrottle > DRIVE_SLEW) {
		linearCmd = prevThrottle + DRIVE_SLEW;
	} else if (ithrottle - prevThrottle < -(DRIVE_SLEW * 2)) {
		// We double the drive slew rate for the reverse direction to get
		// faster stopping.
		linearCmd = prevThrottle - (DRIVE_SLEW * 2);
	}

	double remappedStraight = _turnRemapping(linearCmd,4.2);

	double left, right;
	if (turnInPlace) {
		double remappedTurn = _turnRemapping(iturn,8.2);
		// The remappedTurn value is squared when turning in place. This
		// provides even more fine control over small speed values.
		right = remappedTurn;
		left = -remappedTurn;

		//printf("%f \n",remappedTurn);

	} else {
		double remappedTurn = _turnRemapping(iturn,4.2);
		double negInertiaPower = (iturn - prevTurn) * CD_NEG_INERTIA_SCALAR;
		negInertiaAccumlator += negInertiaPower;

		double angularCmd =
		    std::abs(4 / (1 + exp(-linearCmd/40)) - 2) *  // the more linear vel, the faster we turn
		        (remappedTurn + negInertiaAccumlator) *
		        CD_SENSITIVITY -  // we can scale down the turning amount by a
		                          // constant
		    quickStopAccumlator;

		right = left = remappedStraight;
		right += angularCmd;
		left -= angularCmd;

		_updateAccumulators();
	}

	prevTurn = iturn;
	prevThrottle = ithrottle;
	
	return std::make_pair(left, right);
}

void drive() {
	while (true) {
		double power = master.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
		if (master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_X)) {
			reverse = !reverse;
    	}
		if (reverse) {power = -power;}
	    double turn = master.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X);

		auto [left, right] = cheesyDrive(power, turn);

	    dt_left.move_voltage(left);
	    dt_right.move_voltage(right);

		pros::delay(5);
	}
}

void cata() {
	while (true) {
		if (cata_countinous_toggle) {cata_motors.move_velocity(200);} else {cata_motors.move_velocity(0);}

		while (master.get_digital(pros::E_CONTROLLER_DIGITAL_L2) & !cata_countinous_toggle) {
			cata_motors.move_velocity(100);
		}

		if (master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_A)) {
			cata_countinous_toggle = !cata_countinous_toggle;
    	}

		while (master.get_digital(pros::E_CONTROLLER_DIGITAL_B) & !cata_countinous_toggle) {
			cata_motors.move_velocity(-100);
    	}
		pros::delay(20);
	}
}

void wings() {
	while (true) {
		while (master.get_digital(pros::E_CONTROLLER_DIGITAL_R2) & !wing_solinoid_toggle) {
			wing_solinoid.set_value(true);
    	}
		if (!wing_solinoid_toggle) {wing_solinoid.set_value(false);};
		if (master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_R1)) {
			wing_solinoid.set_value(!wing_solinoid_toggle);
			wing_solinoid_toggle = !wing_solinoid_toggle;
    	}
		pros::delay(20);
	}
}

void intake() {
	while (true) {
		if (master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_L1)) {
			intake_solinoid.set_value(!intake_solinoid_toggle);
			intake_solinoid_toggle = !intake_solinoid_toggle;
    	}
		pros::delay(20);
	}
}

void opcontrol() {
	pros::Task drive_thread(drive);
	pros::Task cata_thread(cata);
	pros::Task wings_thread(wings);
	pros::Task intake_thread(intake);
	// while (true) {
	// 	pros::delay(5);
	// }
}
