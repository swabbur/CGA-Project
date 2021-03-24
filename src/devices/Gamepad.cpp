#include <devices/Gamepad.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

Gamepad::Gamepad(unsigned int index) : index(index), connected(false), axes{}, pressed(), released(), down() {}

bool Gamepad::is_connected() const {
    return connected;
}

float Gamepad::get_axis(unsigned int axis) const {
    if (axes.contains(axis)) {
        return axes.at(axis);
    }
    return 0.0f;
}

bool Gamepad::is_pressed(unsigned char button) const {
    return pressed.contains(button);
}

bool Gamepad::is_released(unsigned char button) const {
    return released.contains(button);
}

bool Gamepad::is_down(unsigned char button) const {
    return down.contains(button);
}

void Gamepad::poll() {

    axes.clear();
    pressed.clear();
    released.clear();

    connected = glfwJoystickPresent(GLFW_JOYSTICK_1 + index);
    if (connected) {

        int axis_count = 6;
        float const * axis_states = glfwGetJoystickAxes(GLFW_JOYSTICK_1 + index, &axis_count);
        for (int axis = 0; axis < axis_count; axis++) {
            axes.insert_or_assign(axis, axis_states[axis]);
        }

        int button_count = 15;
        unsigned char const * button_states = glfwGetJoystickButtons(GLFW_JOYSTICK_1 + index, &button_count);
        for (int button = 0; button < button_count; button++) {
            switch (button_states[button]) {

                case GLFW_PRESS:
                    if (down.insert(button).second) {
                        pressed.insert(button);
                    }
                    break;

                case GLFW_RELEASE:
                    if (down.erase(button)) {
                        released.insert(button);
                    }
                    break;
            }
        }
    } else {
        axes.clear();
        down.clear();
    }
}