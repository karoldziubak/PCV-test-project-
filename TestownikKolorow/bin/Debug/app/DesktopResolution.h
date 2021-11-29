#pragma once

#define BUTTON_A sf::Joystick::X // A button on controller
#define BUTTON_X sf::Joystick::Z // X button on controller
#define BUTTON_B sf::Joystick::Y // B button on controller
#define BUTTON_Y sf::Joystick::R // Y button on controller
#define BUTTON_LB sf::Joystick::U // Left bumper on controller
#define BUTTON_RB sf::Joystick::V // Right bumper on controller
#define AXIS_LEFT_X sf::Joystick::Axis::X //  Left stick x value
#define AXIS_LEFT_Y sf::Joystick::Axis::Y // Left stick y value
#define AXIS_RIGHT_X sf::Joystick::Axis::U // Right stick x value
#define AXIS_RIGHT_Y sf::Joystick::Axis::R // Right stick y value
#define TRIGGERS sf::Joystick::Axis::Z // Positive Z = LT, and Negative Z = RT
#define BUTTON_BACK sf::Joystick::PovX // Back button on controller
#define BUTTON_START sf::Joystick::PovY // Start button on controller
#define DPAD_X sf::Joystick::Axis::PovY // Dpad left and right
#define DPAD_Y sf::Joystick::Axis::PovX // Dpad Up and down

void GetDesktopResolution(int& horizontal, int& vertical);
