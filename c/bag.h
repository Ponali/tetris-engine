#ifndef TE_RAND
#define TE_RAND rand
#endif

typedef struct{
    uint8_t outgoing[14];
    uint8_t pieces;
} TE_TetrisBag;

void TE_TetrisBag_addRandomPieces(TE_TetrisBag* s){
    // make piece list
    uint8_t pieceList[7] = {};
    for(int i=0;i<7;i++) pieceList[i] = 255;
    for(int i=0;i<7;i++){
        for(;;){
            uint8_t place = TE_RAND()%7;
            if(pieceList[place]==255){
                pieceList[place]=i;
                if((TE_RAND()&1)==1) // randomly rotate piece
                    pieceList[place]+=7;
                break;
            }
        }
    }
    // add it to outgoing list
    // not sure if memcpy is a good idea but i would rather use a native C function than a for loop
    memcpy(s->outgoing+s->pieces,pieceList,sizeof(pieceList));
    s->pieces+=7;
}

TE_TetrisBag* TE_TetrisBag_create(){
    TE_TetrisBag* s = (TE_TetrisBag*)malloc(sizeof(TE_TetrisBag));
    s->pieces=0;
    TE_TetrisBag_addRandomPieces(s);
    TE_TetrisBag_addRandomPieces(s);
    return s;
}

uint8_t TE_TetrisBag_pop(TE_TetrisBag* s){
    uint8_t piece = s->outgoing[0];

    // pop array
    memcpy(s->outgoing,s->outgoing+1,sizeof(uint8_t)*13);
    s->pieces-=1;

    // add random pieces if necessary
    if(s->pieces==7)
        TE_TetrisBag_addRandomPieces(s);

    return piece;
}
