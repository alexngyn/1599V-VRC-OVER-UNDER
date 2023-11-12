#include "api.hpp"
#include "setup.hpp"

static uint32_t ledbuffer[LED_LENGTH];
static std::vector<uint32_t> ledbuffer_v;

struct rgb {
  double r;
  double g;
  double b;
};

static std::uint32_t rgb_to_hex(int r, int g, int b) {
    return (((r & 0xff) << 16) + ((g & 0xff) << 8) + (b & 0xff));
}

static rgb hex_to_rgb(std::uint32_t color) {
    rgb in;
    in.r = (color >> 16) & 0xff;
    in.g = (color >> 8) & 0xff;
    in.b = color & 0xff;
    return in;
}

static uint32_t interpolate_rgb(std::uint32_t start_color, std::uint32_t end_color, int step,
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

static void gradient(std::uint32_t start_color, std::uint32_t end_color, int fade_width) {
    for (int i = 0; i < fade_width; i++) {
    	ledbuffer_v[i] = interpolate_rgb(start_color, end_color, i, fade_width);
    }
	for (int i = 3; i < fade_width*2; i++) {
    	ledbuffer_v[i] = interpolate_rgb(end_color, start_color, i, fade_width);
    }
}

void addrled() {
	for(int i = 0;i<LED_LENGTH;i++){
		ledbuffer_v.push_back(0x000000);
	}
	
	pros::delay(1000);

	gradient(0xFFDA29, 0xC40233, 30);

	std::copy(ledbuffer_v.begin(), ledbuffer_v.end(), ledbuffer);
	pros::c::adi_led_set(led_strip, ledbuffer, LED_LENGTH);
	while (true) {
		std::rotate(ledbuffer_v.begin(), ledbuffer_v.begin() + 1, ledbuffer_v.end());
		std::copy(ledbuffer_v.begin(), ledbuffer_v.end(), ledbuffer);
		pros::c::adi_led_set(led_strip, ledbuffer, LED_LENGTH);
		pros::delay(300);
	}
}