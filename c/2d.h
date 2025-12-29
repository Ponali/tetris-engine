#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#ifndef TE_CELL_TYPE
#define TE_CELL_TYPE uint8_t
#endif
#ifndef TE_GRID_COORD_TYPE
#define TE_GRID_COORD_TYPE uint8_t
#endif

typedef struct{
    TE_CELL_TYPE* arr;
    TE_GRID_COORD_TYPE cols;
    TE_GRID_COORD_TYPE rows;
} TE_Grid2D;

TE_Grid2D* TE_Grid2D_create(TE_GRID_COORD_TYPE cols,TE_GRID_COORD_TYPE rows){
    TE_Grid2D* s = (TE_Grid2D*)malloc(sizeof(TE_Grid2D));
    // calloc automatically fills everything by 0, which is set as air, so we don't have to fill it with something else
    s->arr = (TE_CELL_TYPE*)calloc(cols*rows,sizeof(TE_CELL_TYPE));
    s->cols = cols;
    s->rows = rows;
    return s;
}

TE_CELL_TYPE TE_Grid2D_get(TE_Grid2D* s,TE_GRID_COORD_TYPE x,TE_GRID_COORD_TYPE y,TE_CELL_TYPE replace){
    if(x>=s->cols||y>=s->rows) return replace;
    return s->arr[(uint16_t)x+(uint16_t)y*s->cols];
}

void TE_Grid2D_set(TE_Grid2D* s,TE_GRID_COORD_TYPE x,TE_GRID_COORD_TYPE y,TE_CELL_TYPE v){
    if(x>=s->cols||y>=s->rows) return;
    s->arr[(uint16_t)x+(uint16_t)y*s->cols]=v;
}

TE_Grid2D* TE_Grid2D_copy(TE_Grid2D* s){
    TE_Grid2D* n = TE_Grid2D_create(s->cols,s->rows);
    memcpy(n->arr,s->arr,sizeof(TE_CELL_TYPE)*s->cols*s->rows);
    return n;
}

void TE_Grid2D_free(TE_Grid2D* s){
    free(s->arr);
    free(s);
}
