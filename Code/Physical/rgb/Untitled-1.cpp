
Addrled::Addrled(const uint8_t smart_port, const uint8_t adi_port, const uint8_t strip_length,
                 std::vector<uint32_t> colors)
    : Device(6, 0),
      smart_port(smart_port),
      adi_port(adi_port),
      strip_length(strip_length),
      device(vexDeviceGetByIndex(smart_port - 1)) {
    std::unique_lock<sylib::Mutex> _lock;
    if (sylib::millis() > 1) {
        _lock = std::unique_lock<sylib::Mutex>(sylib_port_mutexes[smart_port - 1]);
    }
    getAllAddrleds().push_back(this);
    setAddrledUpdateCycles(getAllAddrleds().size());
    vexDeviceAdiPortConfigSet(device, adi_port - 1, kAdiPortTypeDigitalOut);
    buffer.resize(strip_length);
    colors.resize(strip_length);
    for (std::size_t i = 0; i < strip_length; i++) {
        buffer[i] = 0x000000;
    }
    for (std::size_t i = 0; i < colors.size(); i++) {
        buffer[i] = colors[i];
    }
    addrledControlMode = SylibAddrledControlModeMANUAL;
}

const std::vector<uint32_t> Addrled::off_buffer = std::vector<uint32_t>(64, 0x000000);

std::vector<sylib::Addrled*>& Addrled::getAllAddrleds() {
    static auto allAddrleds = std::vector<sylib::Addrled*>();
    return allAddrleds;
}

void Addrled::setAddrledUpdateCycles(int count) {
    int addrledCount = 0;
    for (auto& subTask : getAllAddrleds()) {
        subTask->setUpdateFrequency(6 * count);
        subTask->setUpdateOffset(6 * addrledCount);
        addrledCount++;
    }
}

void Addrled::update() {
    mutex_lock _lock(sylib_port_mutexes[smart_port - 1]);
    std::uint32_t currentTime = sylib::millis();
    static std::vector<uint32_t> preOverlayBuffer;
    static std::vector<uint32_t> lightOutput;
    lightOutput.resize(buffer.size());

    if (!addrled_enabled) {
        vexDeviceAdiAddrLedSet(device, adi_port - 1, (std::uint32_t*)off_buffer.data(), 0, 64, 0);
        return;
    }

    if (addrledControlMode == SylibAddrledControlModeOFF) {
        vexDeviceAdiAddrLedSet(device, adi_port - 1, (std::uint32_t*)off_buffer.data(), 0, 64, 0);
        return;
    }

    if (addrledControlMode == SylibAddrledControlModeMANUAL) {
        lightOutput = buffer;
    } 
    else if (addrledControlMode == SylibAddrledControlModeCYCLE) {
        for (std::size_t i = 0; i < buffer.size(); i++) {
            if (rotation_buffer[i] <= 0xFFFFFF) {
                buffer[i] = rotation_buffer[i];
            }
        }
        lightOutput = buffer;
        for (int i = 0;
             i <
             ((int)((currentTime - cycleStartMovementTime) / controlSpeed - cyclePixelsShifted));
             i++) {
            if (!cycleControlReversed) {
                std::rotate(rotation_buffer.begin(), rotation_buffer.end() - 1,
                            rotation_buffer.end());
            } else {
                std::rotate(rotation_buffer.begin(), rotation_buffer.begin() + 1,
                            rotation_buffer.end());
            }
            cyclePixelsShifted++;
        }
    }
    if (sendingPulse) {
        preOverlayBuffer = buffer;
        for (std::size_t i = 0; i < buffer.size(); i++) {
            if (template_buffer[i + (controlPulseWidth)] <= 0xFFFFFF) {
                buffer[i] = template_buffer[i + (controlPulseWidth)];
            }
        }

        lightOutput = buffer;
        for (int i = 0;
             i < ((int)((currentTime - pulseStartMovementTime) / pulseSpeed - pulsePixelsShifted));
             i++) {
            if (pulsePixelsShifted < (pixelsToMove - controlPulseWidth + 1)) {
                if (!pulseControlReversed) {
                    std::rotate(template_buffer.begin(), template_buffer.end() - 1,
                                template_buffer.end());
                } else {
                    std::rotate(template_buffer.begin(), template_buffer.begin() + 1,
                                template_buffer.end());
                }
                pulsePixelsShifted++;
            } else {
                pulsePixelsShifted = 0;
                sendingPulse = false;
                break;
            }
        }

        buffer = preOverlayBuffer;
    }
    preOverlayBuffer = buffer;
    for (std::size_t i = 0; i < buffer.size(); i++) {
        rgb shiftedColor;
        shiftedColor.r = hex_to_rgb(lightOutput[i]).r + redShift;
        shiftedColor.g = hex_to_rgb(lightOutput[i]).g + greenShift;
        shiftedColor.b = hex_to_rgb(lightOutput[i]).b + blueShift;

        if (shiftedColor.r > 255) {
            shiftedColor.r = 255;
        } else if (shiftedColor.r < 0) {
            shiftedColor.r = 0;
        }

        if (shiftedColor.g > 255) {
            shiftedColor.g = 255;
        } else if (shiftedColor.g < 0) {
            shiftedColor.g = 0;
        }

        if (shiftedColor.b > 255) {
            shiftedColor.b = 255;
        } else if (shiftedColor.b < 0) {
            shiftedColor.b = 0;
        }

        buffer[i] = rgb_to_hex(shiftedColor.r, shiftedColor.g, shiftedColor.b);
    }
    lightOutput = buffer;
    buffer = preOverlayBuffer;

    vexDeviceAdiAddrLedSet(device, adi_port - 1, (std::uint32_t*)lightOutput.data(), 0,
                           strip_length, 0);
}

void Addrled::set_all(std::uint32_t color) {
    mutex_lock _lock(sylib_port_mutexes[smart_port - 1]);
    addrledControlMode = SylibAddrledControlModeMANUAL;
    for (std::size_t i = 0; i < buffer.size(); i++) {
        buffer[i] = color;
    }
}

void Addrled::set_pixel(std::uint32_t color, std::uint8_t index) {
    mutex_lock _lock(sylib_port_mutexes[smart_port - 1]);
    addrledControlMode = SylibAddrledControlModeMANUAL;
    buffer[index] = color;
}

void Addrled::set_buffer(std::vector<std::uint32_t> colors) {
    mutex_lock _lock(sylib_port_mutexes[smart_port - 1]);
    addrledControlMode = SylibAddrledControlModeMANUAL;
    colors.resize(strip_length);
    buffer = colors;
}

void Addrled::turn_off() {
    mutex_lock _lock(sylib_port_mutexes[smart_port - 1]);
    addrledControlMode = SylibAddrledControlModeOFF;
}

void Addrled::turn_on() {
    mutex_lock _lock(sylib_port_mutexes[smart_port - 1]);
    addrledControlMode = SylibAddrledControlModeMANUAL;
}

void Addrled::clear() {
    mutex_lock _lock(sylib_port_mutexes[smart_port - 1]);
    buffer.clear();
    buffer.resize(strip_length);
    addrledControlMode = SylibAddrledControlModeMANUAL;
}

std::uint32_t Addrled::interpolate_rgb(std::uint32_t start_color, std::uint32_t end_color, int step,
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

void Addrled::gradient(std::uint32_t start_color, std::uint32_t end_color, int fade_width,
                       int start_pos, bool reverse, bool hsv_mode) {
    mutex_lock _lock(sylib_port_mutexes[smart_port - 1]);
    if (fade_width == 0) {
        fade_width = strip_length;
    }
    for (int i = 0; i < fade_width; i++) {
        buffer[start_pos + i] = interpolate_rgb(start_color, end_color, i, fade_width);
    }
    addrledControlMode = SylibAddrledControlModeMANUAL;
}

void Addrled::cycle(std::vector<uint32_t> pattern, int speed, int start_pos, bool reverse) {
    mutex_lock _lock(sylib_port_mutexes[smart_port - 1]);
    addrledControlMode = SylibAddrledControlModeCYCLE;
    if (speed > 100) {
        speed = 100;
    } else if (speed < 1) {
        speed = 1;
    }
    controlSpeed = 250.0 / speed;
    cycleStartMovementTime = sylib::millis();
    cyclePixelsShifted = 0;
    cycleControlReversed = reverse;
    rotation_buffer = pattern;
}

void Addrled::rotate(int pixels, bool reverse) {
    mutex_lock _lock(sylib_port_mutexes[smart_port - 1]);
    addrledControlMode = SylibAddrledControlModeMANUAL;
    if (!reverse) {
        std::rotate(buffer.begin(), buffer.end() - pixels, buffer.end());
    } else {
        std::rotate(buffer.begin(), buffer.begin() + pixels, buffer.end());
    }
}

std::uint32_t Addrled::rgb_to_hex(int r, int g, int b) {
    return (((r & 0xff) << 16) + ((g & 0xff) << 8) + (b & 0xff));
}

rgb Addrled::hex_to_rgb(std::uint32_t color) {
    rgb in;
    in.r = (color >> 16) & 0xff;
    in.g = (color >> 8) & 0xff;
    in.b = color & 0xff;
    return in;
}