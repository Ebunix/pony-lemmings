#pragma once

struct StartInfo {
    int displayIndex;
    int width;
    int height;
    int gameScale;
    bool fullscreen;
};

#define BUTTON_SIZE 40
#define INFO_TEXT_GAP 15
#define LOWER_SCREEN_GAP (BUTTON_SIZE + INFO_TEXT_GAP)
