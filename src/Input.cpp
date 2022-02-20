#include "Input.h"

bool buttonStates[InputButtonCount];
bool lastButtonStates[InputButtonCount];

bool Input::GetButton(InputButton button) {
    return buttonStates[button];
}
bool Input::GetButtonUp(InputButton button) {
    return !buttonStates[button] && lastButtonStates[button];
}
bool Input::GetButtonDown(InputButton button) {
    return buttonStates[button] && !lastButtonStates[button];
}
void Input::SetButtonState(InputButton button, bool state) {
    buttonStates[button] = state;
}
void Input::Update() {
    for (int i = 0; i < InputButtonCount; i++) {
        lastButtonStates[i] = buttonStates[i];
    }
}