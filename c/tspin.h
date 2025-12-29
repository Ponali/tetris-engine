void TE_TetrisGrid_checkTSpin(TE_TetrisGrid* s){
    if(s->tt!=2) return;
    if(TE_Grid2D_get(s->grid,s->tx,s->ty,1)>0 || TE_Grid2D_get(s->grid,s->tx+2,s->ty,1)>0){ // top part
        if(TE_Grid2D_get(s->grid,s->tx,s->ty+2,1)>0 || TE_Grid2D_get(s->grid,s->tx+2,s->ty+2,1)>0){ // bottom part
            s->pieceTSpin=true;
        }
    }
}

bool TE_TetrisGrid_isMiniTSpin(TE_TetrisGrid* s){
    if(s->tt!=2) return false;
    if(s->tr==0) return !(TE_Grid2D_get(s->grid,s->tx+0,s->ty+0,0)>0 && TE_Grid2D_get(s->grid,s->tx+2,s->ty+0,0)>0);
    if(s->tr==1) return !(TE_Grid2D_get(s->grid,s->tx+2,s->ty+0,0)>0 && TE_Grid2D_get(s->grid,s->tx+2,s->ty+2,0)>0);
    if(s->tr==2) return !(TE_Grid2D_get(s->grid,s->tx+2,s->ty+2,0)>0 && TE_Grid2D_get(s->grid,s->tx+0,s->ty+2,0)>0);
    if(s->tr==3) return !(TE_Grid2D_get(s->grid,s->tx+0,s->ty+2,0)>0 && TE_Grid2D_get(s->grid,s->tx+0,s->ty+0,0)>0);
    return false;
}
