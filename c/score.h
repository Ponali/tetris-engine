void TE_TetrisGrid_scoreClearedLines(TE_TetrisGrid* s,uint16_t linesFound,bool isMiniTSpin){
    // handle combo
    if(linesFound>0){
        s->combo+=1;
    } else {
        s->combo=0;
    }
    if(s->combo>=2){
        s->score+=50*(s->combo-1)*s->level;
    }
    // handle back-to-back
    if(linesFound>0){
        if(linesFound<4 && !s->pieceTSpin){
            s->backToBack=0;
        } else {
            s->backToBack+=1;
        }
    }
    uint8_t b2bMultiplier=2;
    if(s->backToBack>2)
        b2bMultiplier=3;
    // handle "all clear"
    bool allClear = TE_TetrisGrid_isAllClear(s);
    // add points to score
    if(allClear){
        if(s->backToBack>2 && linesFound>=4){
            s->score+=3200*s->level;
        } else {
            switch(linesFound){
                case 1: s->score+=800*s->level*b2bMultiplier>>1;break;
                case 2: s->score+=1200*s->level*b2bMultiplier>>1;break;
                case 3: s->score+=1800*s->level*b2bMultiplier>>1;break;
                case 4: s->score+=2000*s->level*b2bMultiplier>>1;break;
            }
        }
    } else {
        if(s->pieceTSpin){
            if(isMiniTSpin){
                switch(linesFound){
                    case 0: s->score+=100*s->level;break;
                    case 1: s->score+=200*s->level*b2bMultiplier>>1;break;
                    case 2: s->score+=400*s->level*b2bMultiplier>>1;break;
                }
            } else {
                switch(linesFound){
                    case 0: s->score+=400*s->level;break;
                    case 1: s->score+=800*s->level*b2bMultiplier>>1;break;
                    case 2: s->score+=1200*s->level*b2bMultiplier>>1;break;
                    case 3: s->score+=1600*s->level*b2bMultiplier>>1;break;
                }
            }
        } else {
            switch(linesFound){
                case 1: s->score+=100*s->level*b2bMultiplier>>1;break;
                case 2: s->score+=300*s->level*b2bMultiplier>>1;break;
                case 3: s->score+=500*s->level*b2bMultiplier>>1;break;
                case 4: s->score+=800*s->level*b2bMultiplier>>1;break;
            }
        }
    }
    // update level
    while(s->levelLines>10){
        s->level+=1;
        s->levelLines-=10;
    }
    // send t-spin reward
    TE_EventData event;
    event.lines=linesFound;
    event.allClear=allClear;
    event.mini=isMiniTSpin;
    TE_EventData* eventPtr = &event;
    if(s->pieceTSpin){
        s->eventHandler(s,TE_Event_tSpin,eventPtr);
    } else if (linesFound>0){
        s->eventHandler(s,TE_Event_lineClear,eventPtr);
    }
    s->eventHandler(s,TE_Event_updateScore,TE_emptyEvent);
}
