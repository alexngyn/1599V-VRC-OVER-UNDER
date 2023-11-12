#include "main.h"
#include <cstdio>

#define LED_PORT 'e'
#define LED_LENGTH 60

uint32_t ledbuffer[LED_LENGTH];
std::vector<uint32_t> ledbuffer_v;

void initialize() {
	pros::lcd::initialize();
}

void disabled() {}

void competition_initialize() {}

void autonomous() {}

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

	std::copy(ledbuffer_v.begin(), ledbuffer_v.end(), ledbuffer);
	pros::c::adi_led_set(led, ledbuffer, LED_LENGTH);
	while (true) {
		//std::rotate(ledbuffer.begin(), ledbuffer.end() - 1, ledbuffer.end());
		std::rotate(ledbuffer_v.begin(), ledbuffer_v.begin() + 1, ledbuffer_v.end());
		std::copy(ledbuffer_v.begin(), ledbuffer_v.end(), ledbuffer);
		pros::c::adi_led_set(led, ledbuffer, LED_LENGTH);
		pros::delay(100);
	}
}

void opcontrol() {
	pros::Task lights(addrled);
}
