#include <stdbool.h>

// TODO: make gravity be able to do >1G up to 20G
// TODO: add *-Spins (if they exist)
// TODO: add optional support for arika SRS
// TODO: make lock delay customizable
// TODO: make gravity speed customizable
// TODO: pass invisible row count, and spawn piece Y based on that instead of hardcoded 4

typedef struct TE_TetrisGrid TE_TetrisGrid;

// wow this type is a handful
#define TE_EVENT_HANDLER_TYPE(NAME) void (*NAME)(TE_TetrisGrid*,int,TE_EventData*)

struct TE_TetrisGrid {
    TE_Grid2D* grid;
    TE_TetrisBag* bag;
    TE_TIMESTAMP_TYPE gravityUpdate;
    bool pieceLock;
    TE_TIMESTAMP_TYPE pieceLockTimestamp;
    uint8_t pieceLockResetMoves;
    bool pieceTSpin;
    TE_GRID_COORD_TYPE cols;
    TE_GRID_COORD_TYPE rows;
    bool spawnPiecesRotated;
    TE_GRID_COORD_TYPE spawnHeight;
    TE_EVENT_HANDLER_TYPE(eventHandler);
    uint32_t linesCleared;
    uint8_t levelLines;
    uint32_t level;
    uint32_t score;
    uint16_t combo;
    bool paused;
    TE_TIMESTAMP_TYPE pauseTimestamp;
    uint16_t backToBack;
    uint8_t holdPiece; // WARNING: can be None at times in the python version - that's represented with 255
    bool canHoldPiece;
    bool gameOver;

    TE_GRID_COORD_TYPE tx;
    TE_GRID_COORD_TYPE ty;
    uint8_t tt;
    uint8_t tr;
};

TE_TetrisGrid* TE_TetrisGrid_create(TE_GRID_COORD_TYPE cols,TE_GRID_COORD_TYPE rows,TE_GRID_COORD_TYPE spawnHeight,TE_EVENT_HANDLER_TYPE(eventHandler)){
    TE_TetrisGrid* s = (TE_TetrisGrid*)malloc(sizeof(TE_TetrisGrid));
    s->grid=TE_Grid2D_create(cols,rows);
    s->bag=TE_TetrisBag_create();
    s->gravityUpdate = getTimestamp();
    s->pieceLock = false;
    s->pieceLockTimestamp = 0;
    s->pieceLockResetMoves = 15;
    s->pieceTSpin = false;
    s->cols=cols;
    s->rows=rows;
    s->spawnPiecesRotated=false;
    s->spawnHeight=spawnHeight;
    s->eventHandler=eventHandler;
    s->linesCleared = 0;
    s->levelLines = 0;
    s->level = 1;
    s->score = 0;
    s->combo = 0;
    s->paused = false;
    s->pauseTimestamp = 0;
    s->backToBack = 0;
    s->holdPiece = 255;
    s->canHoldPiece = true;
    s->gameOver = false;
    return s;
}

void TE_TetrisGrid_free(TE_TetrisGrid* s){
    TE_Grid2D_free(s->grid);
    free(s->bag);
    free(s);
}

uint16_t TE_TetrisGrid_getGravitySpeed(TE_TetrisGrid* s){
    return (800-((s->level-1)*7));
    // formula from Tetris Worlds: https://harddrop.com/wiki/Tetris_Worlds -> Gravity
}

uint16_t TE_TetrisGrid_getLockDelayTime(TE_TetrisGrid* s){
    return 500; // the lock delay doesn't change, and is always set to be half a second
}

bool TE_TetrisGrid_tetriminoColliding(TE_TetrisGrid* s,TE_GRID_COORD_TYPE x,TE_GRID_COORD_TYPE y,uint8_t t,uint8_t r,bool ground){
    // TODO: use pointers to optimize this
    for(int i=0;i<4;i++){
        for(int j=0;j<4;j++){
            if(TE_tetrominoData[t][r][i][j]==1){
                TE_GRID_COORD_TYPE vx = j+x;
                TE_GRID_COORD_TYPE vy = i+y;
                if(vx>=s->cols || vy>=s->rows){
                    if(ground){
                        return vy>=s->rows;
                    } else {
                        return true;
                    }
                } else if(TE_Grid2D_get(s->grid,vx,vy,0)>0){
                    return true;
                }
            }
        }
    }
    return false;
}

void TE_TetrisGrid_spawnSpecificTetrimino(TE_TetrisGrid* s,uint8_t tt){
    // s->tx=3
    s->tx=(s->cols-4)/2;
    s->ty=s->spawnHeight;
    s->tt=tt;
    s->tr=0;
    if(s->tt>=7){
        s->tt-=7;
        // make piece spawn 180° with s->ty 1 less
        if(s->spawnPiecesRotated){
            s->ty=3;
            s->tr=2;
        }
    }
}

void TE_TetrisGrid_checkPieceLock(TE_TetrisGrid* s){
    if(TE_TetrisGrid_tetriminoColliding(s,s->tx,s->ty+1,s->tt,s->tr,true)){
        if(!s->pieceLock){
            s->pieceLock=true;
            s->pieceLockTimestamp=getTimestamp();
        } else {
            s->pieceLockResetMoves-=1;
            if(s->pieceLockResetMoves>0){
                s->pieceLockTimestamp=getTimestamp();
            }
        }
    } else {
        s->pieceLock=false;
    }
}

void TE_TetrisGrid_spawnTetrimino(TE_TetrisGrid* s){
    TE_TetrisGrid_spawnSpecificTetrimino(s,TE_TetrisBag_pop(s->bag));
    s->pieceLock=false;
    s->pieceLockTimestamp=0;
    s->pieceTSpin=false;
    if(TE_TetrisGrid_tetriminoColliding(s,s->tx,s->ty,s->tt,s->tr,false))
        s->gameOver=true;
    s->pieceLockResetMoves=15;
    TE_TetrisGrid_checkPieceLock(s);
    if(!s->canHoldPiece){
        s->canHoldPiece=true;
        s->eventHandler(s,TE_Event_updateHoldPiece,TE_emptyEvent);
    }
    s->eventHandler(s,TE_Event_updateNext,TE_emptyEvent);
}

void TE_TetrisGrid_holdCurrentPiece(TE_TetrisGrid* s){
    if(!s->canHoldPiece) return;
    uint8_t heldPiece = s->holdPiece;
    s->holdPiece=s->tt;
    if(heldPiece!=255){
        TE_TetrisGrid_spawnSpecificTetrimino(s,heldPiece);
    } else {
        TE_TetrisGrid_spawnTetrimino(s);
    }
    s->canHoldPiece=false;
    s->eventHandler(s,TE_Event_updateHoldPiece,TE_emptyEvent);
}

bool TE_TetrisGrid_movePiece(TE_TetrisGrid* s,int8_t vx,int8_t vy){
    if(!TE_TetrisGrid_tetriminoColliding(s,s->tx+vx,s->ty+vy,s->tt,s->tr,false)){
         s->tx+=vx;
         s->ty+=vy;
         TE_TetrisGrid_checkPieceLock(s);
         return true;
    }
    return false;
}

bool TE_TetrisGrid_isAllClear(TE_TetrisGrid* s){
    for(TE_GRID_COORD_TYPE i=0;i<s->rows;i++){
        for(TE_GRID_COORD_TYPE j=0;j<s->cols;j++){
            if(TE_Grid2D_get(s->grid,j,i,1)>0){
                return false;
            }
        }
    }
    return true;
}

// nested functions for TE_TetrisGrid_clearLines

void TE_TetrisGrid_clearLines_removeLine(TE_GRID_COORD_TYPE y,TE_TetrisGrid* s,uint16_t* linesFound){
    // add to score
    s->linesCleared+=1;
    s->levelLines+=1;
    *linesFound+=1;
    // copy
    for(TE_GRID_COORD_TYPE i=y;i>0;i--){ // the original version had i be 1 less, but i don't wanna work with underflowing this
        for(TE_GRID_COORD_TYPE j=0;j<s->cols;j++){
            // NOTE: CHECK IF THIS IS CAUSING A SEGFAULT
            TE_Grid2D_set(s->grid,j,i,TE_Grid2D_get(s->grid,j,i-1,0));
        }
    }
    // remove top line
    for(TE_GRID_COORD_TYPE j=0;j<s->cols;j++){
        TE_Grid2D_set(s->grid,j,0,0);
    }
}

bool TE_TetrisGrid_clearLines_clearCheck(TE_GRID_COORD_TYPE y,TE_TetrisGrid* s){
    bool lineClear=true;
    for(TE_GRID_COORD_TYPE x=0;x<s->cols;x++){
        if(TE_Grid2D_get(s->grid,x,y,0)==0){
            lineClear=false;
        }
    }
    return lineClear;
}

// ---------------------------------------------

// forwards declaration of "isMiniTSpin" inside of tspin.h
bool TE_TetrisGrid_isMiniTSpin(TE_TetrisGrid* s);

// forwards declaration of "scoreClearedLines" inside of score.h
void TE_TetrisGrid_scoreClearedLines(TE_TetrisGrid* s,uint16_t linesFound,bool isMiniTSpin);

void TE_TetrisGrid_clearLines(TE_TetrisGrid* s){
    bool isMiniTSpin = TE_TetrisGrid_isMiniTSpin(s);
    uint16_t linesFound = 0;
    bool* linesToClear = (bool*)calloc(s->rows,sizeof(bool));
    bool foundLinesToClear=false;
    for(TE_GRID_COORD_TYPE y=0;y<s->rows;y++){
        if(TE_TetrisGrid_clearLines_clearCheck(y,s)){
            linesToClear[y]=true;
            foundLinesToClear=true;
        }
    }
    if(foundLinesToClear){
        TE_EventData event;
        event.linesCleared = linesToClear;
        s->eventHandler(s,TE_Event_lineClearAnim,&event);
    }
    for(TE_GRID_COORD_TYPE y=0;y<s->rows;y++){
        if(linesToClear[y]){
            TE_TetrisGrid_clearLines_removeLine(y,s,&linesFound);
        }
    }
    TE_TetrisGrid_scoreClearedLines(s,linesFound,isMiniTSpin);
    if(!foundLinesToClear){
        // since it hasn't been sent to an event, keeping it will result in a memory leak
        free(linesToClear);
    }
}

void TE_TetrisGrid_placeCurrentPiece(TE_TetrisGrid* s){
    s->eventHandler(s,TE_Event_place,TE_emptyEvent);
    for(uint8_t i=0;i<4;i++){
        for(uint8_t j=0;j<4;j++){
            if(TE_tetrominoData[s->tt][s->tr][i][j]==1){
                TE_Grid2D_set(s->grid,j+s->tx,i+s->ty,s->tt+1);
            }
        }
    }
    TE_TetrisGrid_clearLines(s);
    TE_TetrisGrid_spawnTetrimino(s);
}

void TE_TetrisGrid_dropCurrentPiece(TE_TetrisGrid* s,bool place){
    TE_GRID_COORD_TYPE amount=0;
    while(TE_TetrisGrid_movePiece(s,0,1)){
        amount+=1;
    }
    if(place)
        TE_TetrisGrid_placeCurrentPiece(s);
    if(place){
        s->score+=2*amount;
    } else {
        s->score+=1*amount;
    }
    s->eventHandler(s,TE_Event_updateScore,TE_emptyEvent);
}

TE_GRID_COORD_TYPE TE_TetrisGrid_getHardDropYPosition(TE_TetrisGrid* s){
    for(TE_GRID_COORD_TYPE i=s->ty;i<s->rows;i++){
        if(TE_TetrisGrid_tetriminoColliding(s,s->tx,i,s->tt,s->tr,false)){
            return i-1;
        }
    }
    return s->cols;
}

TE_Grid2D* TE_TetrisGrid_getScreenWithCurrentPiece(TE_TetrisGrid* s){
    TE_Grid2D* scr=TE_Grid2D_copy(s->grid);
    // TODO: optimize this with pointers
    TE_GRID_COORD_TYPE shadowY=TE_TetrisGrid_getHardDropYPosition(s);
    for(uint8_t i=0;i<4;i++){
        for(uint8_t j=0;j<4;j++){
            if(TE_tetrominoData[s->tt][s->tr][i][j]==1){
                TE_Grid2D_set(scr,j+s->tx,i+shadowY,8); // shadow
                TE_Grid2D_set(scr,j+s->tx,i+s->ty,s->tt+1);
            }
        }
    }
    return scr;
}

bool TE_TetrisGrid_cellOnCurrentPiece(TE_TetrisGrid* s,TE_GRID_COORD_TYPE x,TE_GRID_COORD_TYPE y){
    // TODO: optimize this with pointers
    TE_GRID_COORD_TYPE vx=x-s->tx;
    TE_GRID_COORD_TYPE vy=y-s->ty;
    if(vx>=4 || vy>=4) return false;
    return TE_tetrominoData[s->tt][s->tr][vy][vx]==1;
}

void TE_TetrisGrid_pause(TE_TetrisGrid* s,bool pauseValue){
    if(s->paused!=pauseValue){
        s->paused=pauseValue;
        if(pauseValue){
            s->pauseTimestamp=getTimestamp();
        } else {
            TE_TIMESTAMP_TYPE pauseTime=getTimestamp()-s->pauseTimestamp;
            s->gravityUpdate+=pauseTime;
            s->pieceLockTimestamp+=pauseTime;
            // NOTE: if any more timestamps show up, make sure to put them here
        }
    }
}

void TE_TetrisGrid_pauseToggle(TE_TetrisGrid* s){
    TE_TetrisGrid_pause(s,!s->paused);
}
