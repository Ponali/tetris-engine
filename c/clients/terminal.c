#define frontendVersion "1.1.1"
#include <termios.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <signal.h>
#include <stdint.h>
#include <stdio.h>

#define TE_TIMESTAMP_TYPE uint32_t
#include <time.h>

TE_TIMESTAMP_TYPE getTimestamp(){
    struct timespec spec;
    clock_gettime(CLOCK_REALTIME, &spec);
    return (TE_TIMESTAMP_TYPE)spec.tv_sec*1000+(TE_TIMESTAMP_TYPE)spec.tv_nsec/1000000;
}

#include "../engine.h"

#define min(a,b) ((a) < (b) ? (a) : (b))
#define max(a,b) ((a) > (b) ? (a) : (b))

// default arguments
TE_GRID_COORD_TYPE columns = 10;
TE_GRID_COORD_TYPE rows = 20;
TE_GRID_COORD_TYPE invisibleRows = 4;
bool spawnPiecesRotated = false;
bool elektronikaMode = false;
bool fastRenderMode = false;

int rewardColor[3] = {255,255,255};
char* rewardString = "";
TE_TIMESTAMP_TYPE rewardTimestamp;
bool rewardClear = false;

bool* linesCleared;
TE_TIMESTAMP_TYPE lineClearTimestamp = 0;

TE_Grid2D* gridBefore = NULL;
TE_TetrisGrid* grid;

struct winsize ttySize;

const int cellColors[9][3] = {
    {  0,  0,  0},
    {  0,255,255},
    {255,255,  0},
    {255,  0,255},
    {255,127,  0},
    {  0,  0,255},
    {  0,255,  0},
    {255,  0,  0},
    { 80, 80, 80},
};

const int green[3] = {0,255,0};

const int* getCellColor(uint8_t cell){
    if(elektronikaMode){
        return green;
    }
    return cellColors[cell];
}

/* char* doubleSize(char* string){
    char* new = (char*)malloc(9+strlen(string)*2);
    sprintf(new,"\x1b#3%s\n\x1b#4\r%s",string,string);
    return new;
} */

void printDoubleSize(char* string){
    printf("\x1b#3%s\n\x1b#4\r%s",string,string);
}

char* center(char* string,int res){
    char* new = (char*)malloc(res+1);
    sprintf(new,"%*s",(int)((res-strlen(string))/2+strlen(string)),string);
    return new;
}

void getGridPos(int* x, int* y){
    *x = (ttySize.ws_col-columns*2)/2;
    *y = (ttySize.ws_row-rows)/2;
}

void renderGrid(bool usePieceDim){
    int gridX,gridY;
    getGridPos(&gridX,&gridY);
    float pieceDim = 1.0f;
    if(grid->pieceLock){
        float timePassed = (float)(getTimestamp()-grid->pieceLockTimestamp)/(float)TE_TetrisGrid_getLockDelayTime(grid);
        pieceDim = 1.0f-timePassed/2.0f;
    }
    TE_Grid2D* scr = TE_TetrisGrid_getScreenWithCurrentPiece(grid);
    if(elektronikaMode){
        printf("\x1b[38;2;0;255;0m");
    }
    for(int y=invisibleRows;y<rows+invisibleRows;y++){
        for(int x=0;x<columns;x++){
            uint8_t cell = TE_Grid2D_get(scr,x,y,0);
            if((gridBefore!=NULL && TE_Grid2D_get(gridBefore,x,y,0)==cell) && !(TE_TetrisGrid_cellOnCurrentPiece(grid,x,y) || (linesCleared[y] && getTimestamp()-lineClearTimestamp<600))){
                continue;
            }
            printf("\x1b[%d;%df",(int)gridY+y-(int)invisibleRows,(int)gridX+(int)x*2);
            if(elektronikaMode){
                if(cell==0 || cell==8){
                    printf(" .");
                } else {
                    printf("[]");
                }
            } else {
                const int* cellColor = getCellColor(cell);
                int r = cellColor[0];
                int g = cellColor[1];
                int b = cellColor[2];
                if(TE_TetrisGrid_cellOnCurrentPiece(grid,x,y)&&usePieceDim){
                    r=r*pieceDim;
                    g=g*pieceDim;
                    b=b*pieceDim;
                }
                if(linesCleared[y]){
                    float light=max(min(1.2f-(float)(getTimestamp()-lineClearTimestamp)/500.0f,1.0f),0.0f);
                    r=max(min(r+(int)(light*70.0f),255),0);
                    g=max(min(g+(int)(light*70.0f),255),0);
                    b=max(min(b+(int)(light*70.0f),255),0);
                }
                int dr=max(min(r-15,255),0);
                int dg=max(min(g-15,255),0);
                int db=max(min(b-15,255),0);
                int br=max(min(r+60,255),0);
                int bg=max(min(g+60,255),0);
                int bb=max(min(b+60,255),0);
                if(cell==0 || cell==8 || fastRenderMode){
                    printf("\x1b[48;2;%d;%d;%dm  ",r,g,b);
                } else {
                    printf("\x1b[48;2;%d;%d;%dm\x1b[38;2;%d;%d;%dm🬿\x1b[38;2;%d;%d;%dm🭥",r,g,b,dr,dg,db,br,bg,bb);
                }
            }
        }
    }
    if(gridBefore!=NULL) TE_Grid2D_free(gridBefore);
    gridBefore=scr;
}

void renderPieceColored(uint8_t piece,const int* color){
    int rotation=0;
    if(piece>=7){
        piece-=7;
        if(spawnPiecesRotated){
            rotation=2;
        }
    }
    printf("\x1b[38;2;%d;%d;%dm",color[0],color[1],color[2]);
    const char blocks[4][5] = {" ","▀","▄","█"};
    for(int i=0;i<4;i+=2){
        for(int j=0;j<4;j++){
            uint8_t idx = TE_tetrominoData[piece][rotation][i][j]+TE_tetrominoData[piece][rotation][i+1][j]*2;
            printf("%s",blocks[idx]);
        }
        printf("\x1b[4D\n");
    }
}

void renderPiece(uint8_t piece){
    renderPieceColored(piece,getCellColor(piece%7+1));
}

void showInfo(){
    printf("\x1b[%d;1f\x1b#5\x1b[2m\x1b[2K",ttySize.ws_row);
    char text[256];
    sprintf(text,"tetris-engine v%s terminal frontend v%s (c)",TE_VERSION,frontendVersion);
    printf("%s%*s",text,(int)(ttySize.ws_col-strlen(text)),"");
    printf("\x1b[0m");
    fflush(stdout);
}

void renderReward(){
    int posY = min((ttySize.ws_row+rows+2)/2,ttySize.ws_row-2);
    printf("\x1b[%d;1f",posY);
    float fade = max(min(2.5f-(float)(getTimestamp()-rewardTimestamp)/600.0f,1.0f),0.0f);
    int r = (int)(fade*(float)rewardColor[0]);
    int g = (int)(fade*(float)rewardColor[1]);
    int b = (int)(fade*(float)rewardColor[2]);
    if(elektronikaMode){
        r=0;
        g=(int)(fade*255.0f);
        b=0;
    }
    printf("\x1b[49m\x1b[38;2;%d;%d;%dm",r,g,b);
    if(fade>0){
        char* ansiDim="";
        if(fade<0.5){
            ansiDim="\x1b[2m";
        }
        char string[1024];
        char* centered = center(rewardString,ttySize.ws_col/2);
        sprintf(string,"\x1b[2K%s%s",ansiDim,centered);
        // printf(doubleSize(string));
        printDoubleSize(string);
        free(centered);
    } else {
        // printf(doubleSize("\x1b[2K"));
        printDoubleSize("\x1b[2K");
        rewardClear=false;
    }
}

void renderSide(){
    int gridX,gridY;
    getGridPos(&gridX,&gridY);
    printf("\x1b[0m\x1b[%d;%df",gridY,gridX+columns*2+1);
    for(int i=0;i<4;i++){
        renderPiece(grid->bag->outgoing[i]);
    }
    if(elektronikaMode){
        printf("\x1b[38;2;0;255;0m");
    } else {
        printf("\x1b[39;49m");
    }
    printf("\x1b[s");
    const char* newline = "\x1b[u\n\x1b[s\x1b[K";
    char string[512];
    sprintf(string,"level %d%slines: %d%sscore: %06d",grid->level,newline,grid->linesCleared,newline,grid->score);
    if(grid->combo>=2){
        sprintf(strchr(string,'\0'),"%s\x1b[0m\x1b[1mcombo x%d\x1b[22m",newline,grid->combo);
        // turns out, using "0m" instead of "39m" is required to have the plasma-themed bold color
    }
    if(grid->backToBack>=2){
        sprintf(strchr(string,'\0'),"%s\x1b[0m\x1b[1mback-to-back x%d\x1b[22m",newline,grid->backToBack);
    }
    sprintf(strchr(string,'\0'),"%s%s",newline,newline);
    printf("\x1b[K%s",string);
    fflush(stdout);
}

void renderHoldPiece(){
    int gridX,gridY;
    getGridPos(&gridX,&gridY);
    printf("\x1b[39m\x1b[48;2;50;50;50m");
    if(elektronikaMode){
        printf("\x1b[49m\x1b[38;2;0;255;0m");
    }
    printf("\x1b[%d;%df",gridY,gridX-6);
    for(int i=0;i<3;i++){
        printf("      \n\x1b[6D");
    }
    printf("\x1b[3A");
    printf(" HOLD\n\x1b[4D");
    if(grid->holdPiece==255){
        printf("\x1b[38;2;100;100;100mnone");
    } else {
        if(grid->canHoldPiece){
            renderPiece(grid->holdPiece);
        } else {
            const int grayColor[3] = {120,120,120};
            renderPieceColored(grid->holdPiece,grayColor);
        }
    }
    printf("\x1b[39;49m");
    fflush(stdout);
}

void eventHandler(TE_TetrisGrid* grid,int eventType,TE_EventData* data){
    if(eventType==TE_Event_place){
        renderGrid(false);
    }
    if(eventType==TE_Event_updateNext || eventType==TE_Event_updateScore){
        renderSide();
    }
    if(eventType==TE_Event_updateHoldPiece){
        renderHoldPiece();
    }
    if(eventType==TE_Event_tSpin){
        switch(data->lines){
            case 0: rewardString="t-spin";break;
            case 1: rewardString="t-spin single";break;
            case 2: rewardString="t-spin double";break;
            case 3: rewardString="t-spin triple";break;
        }
        // rewardColor={255,255,190};
        rewardColor[0]=255;
        rewardColor[1]=255;
        rewardColor[2]=190;
        if(data->mini){
            switch(data->lines){
                case 0: rewardString="mini t-spin";break;
                case 1: rewardString="mini t-spin single";break;
                case 2: rewardString="mini t-spin double";break;
                case 3: rewardString="mini t-spin triple";break;
            }
            // rewardColor={210,170,210};
            rewardColor[0]=210;
            rewardColor[1]=170;
            rewardColor[2]=210;
        }
        rewardTimestamp=getTimestamp();
        rewardClear=true;
    }
    if(eventType==TE_Event_lineClearAnim){
        free(linesCleared);
        linesCleared=data->linesCleared;
        lineClearTimestamp=getTimestamp();
        gridBefore=NULL;
    }
    if(eventType==TE_Event_lineClear){
        if(data->allClear){
            rewardString="ALL CLEAR!";
            // rewardColor={255,255,190};
            rewardColor[0]=255;
            rewardColor[1]=255;
            rewardColor[2]=190;
        } else {
            switch(data->lines){
                case 1: return;
                case 2:
                    rewardString="double";
                    // rewardColor={80,80,80};
                    rewardColor[0]=80;
                    rewardColor[1]=80;
                    rewardColor[2]=80;
                    break;
                case 3:
                    rewardString="triple";
                    // rewardColor={150,180,180};
                    rewardColor[0]=150;
                    rewardColor[1]=180;
                    rewardColor[2]=180;
                    break;
                case 4:
                    rewardString="tetris";
                    // rewardColor={80,255,255};
                    rewardColor[0]=80;
                    rewardColor[1]=255;
                    rewardColor[2]=255;
                    break;
            }
        }
        rewardTimestamp=getTimestamp();
        rewardClear=true;
    }
}

void render(){
    int gridX,gridY;
    getGridPos(&gridX,&gridY);
    // printf("\x1b[%d;%df",gridY,gridX);
    renderGrid(true);
    if(rewardClear)
        renderReward();
    fflush(stdout);
}

void rerender(){
    printf("\x1b[0m\x1b[H\x1b[2J"); // clear screen
    showInfo();
    gridBefore=NULL;
    renderGrid(true);
    rewardClear=true;
    renderReward();
    renderSide();
    renderHoldPiece();
}

void resChange(int signum){
    ioctl(1, TIOCGWINSZ, &ttySize);
    rerender();
}

int main(int argc, char *argv[]){
    ioctl(1, TIOCGWINSZ, &ttySize);

    for(int i=1;i<argc;i++){
        char* v = argv[i];
        char* idxPtr = strchr(v,'=');
        if(idxPtr!=NULL){
            int index = (int)(idxPtr-v);
            char name[20];
            strncpy(name,v,index);
            char value[20];
            strncpy(value,v+index+1,strlen(v)-index);
            if(strcmp(name,"w")==0 || strcmp(name,"width")==0 || strcmp(name,"c")==0 || strcmp(name,"cols")==0 || strcmp(name,"columns")==0){
                columns = atoi(value);
            }
            if(strcmp(name,"h")==0 || strcmp(name,"height")==0 || strcmp(name,"r")==0 || strcmp(name,"rows")==0 || strcmp(name,"l")==0 || strcmp(name,"lines")==0){
                rows = atoi(value);
            }
            if(strcmp(name,"ir")==0 || strcmp(name,"irows")==0 || strcmp(name,"bz")==0 || strcmp(name,"bufferzone")==0){
                invisibleRows = atoi(value);
            }
        } else {
            if(strcmp(v,"sr")==0 || strcmp(v,"rotate")==0 || strcmp(v,"spawnrotated")==0){
                spawnPiecesRotated=true;
            }
            if(strcmp(v,"alexey")==0 || strcmp(v,"leonidovich")==0 || strcmp(v,"pajitnov")==0 || strcmp(v,"elektronika")==0){
                elektronikaMode=true;
            }
            if(strcmp(v,"fast")==0 || strcmp(v,"fastrender")==0 || strcmp(v,"boxrender")==0){
                fastRenderMode=true;
            }
        }
    }

    linesCleared = (bool*)calloc(rows+invisibleRows, sizeof(bool));

    srand(time(NULL));

    grid = TE_TetrisGrid_create(columns,rows+invisibleRows,invisibleRows,eventHandler);
    grid->spawnPiecesRotated = spawnPiecesRotated;

    // set terminal to raw mode
    // https://viewsourcecode.org/snaptoken/kilo/02.enteringRawMode.html
    struct termios old_settings;
    tcgetattr(0, &old_settings);
    struct termios new_settings = old_settings;
    new_settings.c_lflag &= ~(ECHO | ICANON);
    new_settings.c_oflag &= ~(ONLCR);
    tcsetattr(0, TCSAFLUSH, &new_settings);

    printf("\x1b[?1049h"); // use alternative screen buffer
    printf("\x1b[?25l"); // hide cursor
    showInfo();
    signal(SIGWINCH, resChange); // everytime the terminal changes resolution, it sends the SIGWINCH signal
    TE_TetrisGrid_spawnTetrimino(grid);
    renderHoldPiece();

    while(true){
        char chr = '\0';
        bool up = false;
        bool down = false;
        bool left = false;
        bool right = false;
        int n = 0;
        // Source - https://stackoverflow.com/a
        // Posted by aragaer, modified by community. See post 'Timeline' for change history
        // Retrieved 2025-12-13, License - CC BY-SA 4.0
        if(ioctl(0, FIONREAD, &n) == 0 && n > 0){
            chr = getchar();
            if(chr=='\x1b'){
                if(getchar()=='['){
                    char ansiChar = getchar();
                    switch(ansiChar){
                        case 'A': up=true; break;
                        case 'B': down=true; break;
                        case 'C': right=true; break;
                        case 'D': left=true; break;
                    }
                }
            }
            if(chr=='\x03' || chr=='q') break;
        }
        TE_TetrisGrid_update(grid,
            (TE_INPUT_TYPE)((int)(left  || chr=='4')+(int)(chr=='s')*columns),
            (TE_INPUT_TYPE)((int)(right || chr=='6')+(int)(chr=='d')*columns),
            (TE_INPUT_TYPE)(down || chr=='2'),
            up || chr=='x' || chr=='1' || chr=='5' || chr=='9',
            chr=='z' || chr=='3' || chr=='7',
            chr=='a',
            chr==',' || chr=='.',
            chr==' ' || chr=='8',
            chr=='c' || chr=='0'
        );
        render();
        usleep(16667); // 1/60s converted into microseconds
        if(grid->gameOver){
            printf("\x1b[%d;1H\x1b[0;7m\x1b[2K",ttySize.ws_row);
            const char* gameOverText = "game over - press enter to exit, or press r to restart";
            printf("%s%*s",gameOverText,(int)(ttySize.ws_col-strlen(gameOverText)),"");
            fflush(stdout);
            while(!(chr=='\n' || chr=='\r' || chr=='\x03' || chr=='r')){
                chr = getchar();
            }
            if(chr!='r'){
                break;
            } else {
                printf("\x1b[0m");
                showInfo();
            }
        }
        if(chr=='p'){
            printf("\x1b[%d;1H\x1b[0;7m\x1b[2K",ttySize.ws_row);
            const char* pauseText = "game paused";
            printf("%s%*s",pauseText,(int)(ttySize.ws_col-strlen(pauseText)),"");
            fflush(stdout);
            while(getchar()!='p') continue;
            printf("\x1b[0m");
            showInfo();
        }
        if(chr=='r'){
            free(grid);
            grid = TE_TetrisGrid_create(columns,rows+invisibleRows,invisibleRows,eventHandler);
            TE_TetrisGrid_spawnTetrimino(grid);
            renderHoldPiece();
        }
    }

    printf("\x1b[?25h"); // show cursor
    printf("\x1b[0m"); // remove all attributes
    printf("\x1b[?1049l"); // stop using alternative screen buffer

    tcsetattr(0, TCSAFLUSH, &old_settings);
}
