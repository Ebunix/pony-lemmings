#pragma once

enum InputButton {
    Left,
    Right,
    Up, 
    Down,
    Number0,
    Number1,
    Number2,
    Number3,
    Number4,
    Number5,
    Number6,
    Number7,
    Number8,
    Number9,
    Space,
    Escape,
    InputButtonCount
};

class Input {
public:
    static bool GetButton(InputButton button);
    static bool GetButtonDown(InputButton button);
    static bool GetButtonUp(InputButton button);

    static void SetButtonState(InputButton button, bool state);
    static void Update();
};