#include <stdbool.h>

enum TE_Event{
    TE_Event_place,
    TE_Event_tSpin,
    TE_Event_lineClear,
    TE_Event_lineClearAnim,
    TE_Event_updateNext,
    TE_Event_updateScore,
    TE_Event_updateHoldPiece,
};

typedef struct TE_EventData {
    TE_GRID_COORD_TYPE lines;
    bool mini; // Mini T-Spin
    bool allClear;
    bool* linesCleared; // line clear animation
} TE_EventData;

TE_EventData TE_emptyEventObject;
TE_EventData* TE_emptyEvent = &TE_emptyEventObject;
