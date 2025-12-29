void TE_TetrisGrid_update(TE_TetrisGrid* s,TE_INPUT_TYPE left,TE_INPUT_TYPE right,TE_INPUT_TYPE down,bool rotateCW,bool rotateCCW,bool rotate180,bool sonicDrop,bool hardDrop,bool holdPiece){
    if(s->gameOver) return;
    if(sonicDrop){
        TE_TetrisGrid_dropCurrentPiece(s,false);
    } else if(hardDrop){
        TE_TetrisGrid_dropCurrentPiece(s,true);
    } else if(left>0){
        for(TE_INPUT_TYPE i=0;i<left;i++)
            TE_TetrisGrid_movePiece(s,-1, 0);
    } else if(right>0){
        for(TE_INPUT_TYPE i=0;i<right;i++)
            TE_TetrisGrid_movePiece(s, 1, 0);
    } else if(down>0){
        for(TE_INPUT_TYPE i=0;i<down;i++)
            TE_TetrisGrid_movePiece(s, 0, 1);
        s->score+=1*down;
        s->eventHandler(s,TE_Event_updateScore,TE_emptyEvent);
    } else if(rotateCW){
        TE_TetrisGrid_rotatePieceCW(s);
    } else if(rotateCCW){
        TE_TetrisGrid_rotatePieceCCW(s);
    } else if(rotate180){
        TE_TetrisGrid_rotatePiece180(s);
    } else if(holdPiece){
        TE_TetrisGrid_holdCurrentPiece(s);
    }

    if(s->gravityUpdate+TE_TetrisGrid_getGravitySpeed(s)<getTimestamp()){
        s->gravityUpdate=getTimestamp();
        TE_TetrisGrid_movePiece(s, 0, 1);
    }
    if(s->pieceLock && s->pieceLockTimestamp+TE_TetrisGrid_getLockDelayTime(s)<getTimestamp()){
        TE_TetrisGrid_placeCurrentPiece(s);
    }
}
