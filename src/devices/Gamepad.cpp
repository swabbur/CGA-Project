#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <devices/Gamepad.hpp>

Gamepad::Gamepad() : connected(false), axes{}, pressed(), released(), down() {}

Gamepad::~Gamepad() = default;

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

    connected = glfwJoystickIsGamepad(GLFW_JOYSTICK_1);
    if (connected) {

        GLFWgamepadstate state;
        glfwGetGamepadState(GLFW_JOYSTICK_1, &state);

        int axis_count = 6;
        for (int axis = 0; axis < axis_count; axis++) {
            axes.insert_or_assign(axis, state.axes[axis]);
        }

        int button_count = 15;
        for (int button = 0; button < button_count; button++) {
            switch (state.buttons[button]) {

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