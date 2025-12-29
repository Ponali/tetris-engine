#include <stdint.h>

typedef struct{
    int8_t x;
    int8_t y;
} TE_SRSKick;

// NOTE: y positions are reversed on these tables: "-1" means 1 down, and not 1 up.
const TE_SRSKick TE_standardWallKicksCW[4][5]={
  {{ 0, 0},{-1, 0},{-1,+1},{ 0,-2},{-1,-2}},
  {{ 0, 0},{+1, 0},{+1,-1},{ 0,+2},{+1,+2}},
  {{ 0, 0},{+1, 0},{+1,+1},{ 0,-2},{+1,-2}},
  {{ 0, 0},{-1, 0},{-1,-1},{ 0,+2},{-1,+2}}
};
const TE_SRSKick TE_standardWallKicksCCW[4][5]={
  {{ 0, 0},{+1, 0},{+1,+1},{ 0,-2},{+1,-2}},
  {{ 0, 0},{+1, 0},{+1,-1},{ 0,+2},{+1,+2}},
  {{ 0, 0},{-1, 0},{-1,+1},{ 0,-2},{-1,-2}},
  {{ 0, 0},{-1, 0},{-1,-1},{ 0,+2},{-1,+2}}
};
const TE_SRSKick TE_standardWallKicks180[4][6]={ // from tetr.io
  {{ 0, 0},{ 0, 1},{+1,+1},{-1,+1},{+1, 0},{-1, 0}},
  {{ 0, 0},{+1, 0},{+1,+2},{+1,+1},{ 0,+2},{ 0,+1}},
  {{ 0, 0},{ 0,-1},{-1,-1},{+1,-1},{-1, 0},{+1, 0}},
  {{ 0, 0},{-1, 0},{-1,+2},{-1,+1},{ 0,+2},{ 0,+1}}
};
const TE_SRSKick TE_longPieceWallKicksCW[4][5]={
  {{ 0, 0},{-2, 0},{+1, 0},{-2,-1},{+1,+2}},
  {{ 0, 0},{-1, 0},{+2, 0},{-1,+2},{+2,-1}},
  {{ 0, 0},{+2, 0},{-1, 0},{+2,+1},{-1,-2}},
  {{ 0, 0},{+1, 0},{-2, 0},{+1,-2},{-2,+1}}
};
const TE_SRSKick TE_longPieceWallKicksCCW[4][5]={
  {{ 0, 0},{-1, 0},{+2, 0},{-1,+2},{+2,-1}},
  {{ 0, 0},{+2, 0},{-1, 0},{+2,+1},{-1,-2}},
  {{ 0, 0},{+1, 0},{-2, 0},{+1,-2},{-2,+1}},
  {{ 0, 0},{-2, 0},{+1, 0},{-2,-1},{+1,+2}}
};
const TE_SRSKick TE_longPieceWallKicks180[4][6]={ // i couldn't find anything on 180° rotations for I pieces, so this will be left unimplemented
  {{ 0, 0},{ 0, 0},{ 0, 0},{ 0, 0},{ 0, 0},{ 0, 0}},
  {{ 0, 0},{ 0, 0},{ 0, 0},{ 0, 0},{ 0, 0},{ 0, 0}},
  {{ 0, 0},{ 0, 0},{ 0, 0},{ 0, 0},{ 0, 0},{ 0, 0}},
  {{ 0, 0},{ 0, 0},{ 0, 0},{ 0, 0},{ 0, 0},{ 0, 0}}
}; // NOTE: this array uses 6 movements instead of 1 due to TE_standardWallKicks180 having 6 movements, and it generally makes things easier

void TE_TetrisGrid_rotatePieceCW(TE_TetrisGrid* s){
    uint8_t newr=(s->tr+1)&3;
    // TODO: use pointers to optimize this
    const TE_SRSKick* kickData;
    if(s->tt==0){
        kickData = TE_longPieceWallKicksCW[s->tr];
    } else {
        kickData = TE_standardWallKicksCW[s->tr];
    }
    for(int i=0;i<5;i++){
        if(!TE_TetrisGrid_tetriminoColliding(s,s->tx+kickData[i].x,s->ty-kickData[i].y,s->tt,newr,false)){
            s->tx+=kickData[i].x;
            s->ty-=kickData[i].y;
            s->tr=newr;
            TE_TetrisGrid_checkPieceLock(s);
            TE_TetrisGrid_checkTSpin(s);
            return;
        }
    }
}

void TE_TetrisGrid_rotatePieceCCW(TE_TetrisGrid* s){
    uint8_t newr=(s->tr+3)&3;
    // TODO: use pointers to optimize this
    const TE_SRSKick* kickData;
    if(s->tt==0){
        kickData = TE_longPieceWallKicksCCW[s->tr];
    } else {
        kickData = TE_standardWallKicksCCW[s->tr];
    }
    for(int i=0;i<5;i++){
        if(!TE_TetrisGrid_tetriminoColliding(s,s->tx+kickData[i].x,s->ty-kickData[i].y,s->tt,newr,false)){
            s->tx+=kickData[i].x;
            s->ty-=kickData[i].y;
            s->tr=newr;
            TE_TetrisGrid_checkPieceLock(s);
            TE_TetrisGrid_checkTSpin(s);
            return;
        }
    }
}

void TE_TetrisGrid_rotatePiece180(TE_TetrisGrid* s){
    uint8_t newr=(s->tr+2)&3;
    // TODO: use pointers to optimize this
    const TE_SRSKick* kickData;
    if(s->tt==0){
        kickData = TE_longPieceWallKicks180[s->tr];
    } else {
        kickData = TE_standardWallKicks180[s->tr];
    }
    for(int i=0;i<6;i++){
        if(!TE_TetrisGrid_tetriminoColliding(s,s->tx+kickData[i].x,s->ty-kickData[i].y,s->tt,newr,false)){
            s->tx+=kickData[i].x;
            s->ty-=kickData[i].y;
            s->tr=newr;
            TE_TetrisGrid_checkPieceLock(s);
            TE_TetrisGrid_checkTSpin(s);
            return;
        }
    }
}
