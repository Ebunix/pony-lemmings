#pragma once

enum MovementMode {
    Walker,
    Climber,
    Floater,
    Bomber,
    Blocker,
    Builder,
    Basher,
    Miner,
    Digger,
};

static const char* MovementModeText(MovementMode mode) {
    switch (mode)
    {
        case Walker:
            return "Walker";
        case Climber:
            return "Climber";
        case Floater:
            return "Floater";
        case Bomber:
            return "Bomber";
        case Blocker:
            return "Blocker";
        case Builder:
            return "Builder";
        case Basher:
            return "Basher";
        case Miner:
            return "Miner";
        case Digger:
            return "Digger";
        default:
            return "Unknown";
    }
}
