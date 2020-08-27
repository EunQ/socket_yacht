#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>
#include <termios.h> 

//check borad idx
#define ACES        0
#define DEUSCES     1
#define THREES      2
#define FOURS       3
#define FIVES       4
#define SIXES       5
#define CHOCIE      6
#define FOUR_KIND   7
#define FULL_HOUSE  8
#define S_STRAIGHT  9
#define L_STRAIGHT  10
#define YACHT       11

//key define
#define KEY_ENTER   '\n'
#define KEY_UP      65
#define KEY_DOWN    66
#define KEY_RIGHT   67
#define KEY_LEFT    68

//possition define
#define POS_DICE_FIX    20
#define POS_DICE_ROLL   21
#define POS_BOARD_CHECK 22

#define DICE_NUM    5

#define DARW_HEGITH 28

char backgroundBuf[30][80];
typedef struct _Pos{
    int y;
    int x;
}Pos;
Pos firstCheckPos[12] = {
    { 4,17 },
    { 5,17 },
    { 6,17 },
    { 7,17 },
    { 8,17 },
    { 9,17 },
    { 14,17 },
    { 15,17 },
    { 16,17 },
    { 17,17 },
    { 18,17 },
    { 19,17 }
};
Pos secondCheckPos[12] = {
    { 4,41 },
    { 5,41 },
    { 6,41 },
    { 7,41 },
    { 8,41 },
    { 9,41 },
    { 14,41 },
    { 15,41 },
    { 16,41 },
    { 17,41 },
    { 18,41 },
    { 19,41 }
};
Pos dicesShowPos[5] = {
    {4, 61 },
    {8 ,61},
    {12, 61 },
    {16, 61 },
    {20, 61 }
};
Pos *pCheckPos[2] = {
    firstCheckPos, 
    secondCheckPos
};
Pos dicesFixPos[6] = {
    {4, 71 },
    {8 ,71},
    {12, 71 },
    {16, 71 },
    {20, 71 },
    {24, 71 }
};
Pos rollCntPos = {24, 61};
int rollCnt;
typedef struct _BoardCheckPosInfo{
    Pos* startAddr;
    int posCnt;
}BoardCheckPosInfo;
BoardCheckPosInfo boardCheckPosInfo[2];
int diceData[6];
int curUserXidx;
int curUserYidx;
int curUserId;
Pos curUserPos;
char preChar;
void setStatus(const char * msg){
    strcpy(backgroundBuf[28] + 5, msg);
}

void update(char ch);

void gotoxy(int y,int x) {
    printf("%c[%d;%df",0x1B,y,x);
	fflush(stdout);
}

int getkey(int is_echo) {
    int ch; 
    struct termios old;
    struct termios current; 
    /* 현재 설정된 terminal i/o 값을 backup함 */ 
    tcgetattr(0, &old); 
    /* 현재의 설정된 terminal i/o에 일부 속성만 변경하기 위해 복사함 */
    current = old; /* buffer i/o를 중단함 */ 
    current.c_lflag &= ~ICANON;
    if (is_echo) {
        // 입력값을 화면에 표시할 경우 
        current.c_lflag |= ECHO; }
    else { 
        // 입력값을 화면에 표시하지 않을 경우
        current.c_lflag &= ~ECHO; }
    /* 변경된 설정값으로 설정합니다.*/
    tcsetattr(0, TCSANOW, &current);
    ch = getchar();
    tcsetattr(0, TCSANOW, &old);
    return ch;
}

void flipDiceFix(int y, int x){
    
    if(backgroundBuf[y][x] == 'v'){
        backgroundBuf[y][x] = ' ';
    }
    else{
        backgroundBuf[y][x] = 'v';
    }
    return;
}

void init(){
    int i;
    preChar = ' ';
    strcpy (backgroundBuf[29]  , "0123456789012345678901234567890123456789012345678901234567890123456789012345678");
    strcpy (backgroundBuf[0]   , " ----------------------------------------------------------------------------- ");
    strcpy (backgroundBuf[1]   , "|                       |                       |                             |");
    strcpy (backgroundBuf[2]   , "|        1st            |        2nd            |          Dices      Fix     |");
    strcpy (backgroundBuf[3]   , "|                       |                       |                             |");
    strcpy (backgroundBuf[4]   , "| Aces        : [ ]  00 | Aces        : [ ]  00 |   1 .   [  0  ]     [ ]     |");
    strcpy (backgroundBuf[5]   , "| Deusces     : [ ]  00 | Deusces     : [ ]  00 |                             |");
    strcpy (backgroundBuf[6]   , "| Threes      : [ ]  00 | Threes      : [ ]  00 |                             |");
    strcpy (backgroundBuf[7]   , "| Fours       : [ ]  00 | Fours       : [ ]  00 |                             |");
    strcpy (backgroundBuf[8]   , "| Fives       : [ ]  00 | Fives       : [ ]  00 |   2 .   [  0  ]     [ ]     |");
    strcpy (backgroundBuf[9]   , "| Sixes       : [ ]  00 | Sixes       : [ ]  00 |                             |");
    strcpy (backgroundBuf[10]  , "|                       |                       |                             |");
    strcpy (backgroundBuf[11]  , "| SubTotal : 000 / 63   | SubTotal : 000 / 63   |                             |");
    strcpy (backgroundBuf[12]  , "| +35 Bonus   :      00 | +35 Bonus   :      00 |   3 .   [  0  ]     [ ]     |");
    strcpy (backgroundBuf[13]  , "|                       |                       |                             |");
    strcpy (backgroundBuf[14]  , "| Choice      : [ ]  00 | Choice      : [ ]  00 |                             |");
    strcpy (backgroundBuf[15]  , "| 4 of a kind : [ ]  00 | 4 of a kind : [ ]  00 |                             |");
    strcpy (backgroundBuf[16]  , "| Full House  : [ ]  00 | Full House  : [ ]  00 |   4 .   [  0  ]     [ ]     |");
    strcpy (backgroundBuf[17]  , "| S. Straight : [ ]  00 | S. Straight : [ ]  00 |                             |");
    strcpy (backgroundBuf[18]  , "| L. Straight : [ ]  00 | L. Straight : [ ]  00 |                             |");
    strcpy (backgroundBuf[19]  , "| Yacht       : [ ]  00 | Yacht       : [ ]  00 |                             |");
    strcpy (backgroundBuf[20]  , "|                       |                       |   5 .   [  0  ]     [ ]     |");
    strcpy (backgroundBuf[21]  , "|                       |                       |                             |");
    strcpy (backgroundBuf[22]  , "|                       |                       |                             |");
    strcpy (backgroundBuf[23]  , "|  Total   :   000      |   Total   :   000     |                             |");
    strcpy (backgroundBuf[24]  , "|                       |                       |    res  :  3 , roll [ ]     |");
    strcpy (backgroundBuf[25]  , "|                       |                       |                             |");
    strcpy (backgroundBuf[26]  , " ----------------------------------------------------------------------------- ");
    strcpy (backgroundBuf[27]  , " status :                                                                      ");
    strcpy (backgroundBuf[28]  , " ----------------------------------------------------------------------------- ");



    boardCheckPosInfo[0].startAddr = firstCheckPos;
    boardCheckPosInfo[0].posCnt = sizeof(firstCheckPos) / sizeof(Pos);

    boardCheckPosInfo[1].startAddr = dicesFixPos;
    boardCheckPosInfo[1].posCnt = sizeof(dicesFixPos) / sizeof(Pos);

    curUserXidx = 1;
    curUserYidx = 0;
    curUserPos.y = boardCheckPosInfo[curUserXidx].startAddr[0].y;
    curUserPos.x = boardCheckPosInfo[curUserXidx].startAddr[0].x;

    
    srand(time(NULL));

    rollCnt = 3;

    for(i=0;i<5;i++){
        diceData[i] = 0;
        backgroundBuf[dicesShowPos[i].y][dicesShowPos[i].x] = (char)(diceData[i] + '0');
    }

    update(0);
}
int rollDice(int dicePosY, int dicePosX, int milsec){
    int num = 0;
    for(int i=0;i<milsec;i++){
        gotoxy(dicePosY+2,dicePosX+1);
        //printf("\n");
        num = (rand()%6)+1;
        backgroundBuf[dicePosY][dicePosX] = (char)(num+'0');
        //draw();
        printf("%d", num);
        usleep(i*1000);
    }
    //backgroundBuf[dicePosY][dicePosX] = (char)(num+'0');
    return num;
}
void rollCntDecrease(){
    rollCnt--;
    backgroundBuf[rollCntPos.y][rollCntPos.x] = (char)('0' + rollCnt);
}
int checkPosition(int y, int x, int* idx){
    int i=0;
    *idx = 0;
    for(i=0;i<6;i++){
        if(i == 5 && y == dicesFixPos[i].y && x == dicesFixPos[i].x){
            return POS_DICE_ROLL;
        }
        if(y == dicesFixPos[i].y && x == dicesFixPos[i].x){
            *idx = i;
            return POS_DICE_FIX;
        }
    }
    for(int i=0;i<12;i++){
        if(y == pCheckPos[curUserId][i].y && x ==  pCheckPos[curUserId][i].x){
            *idx = i;
            break;
        }
    }
    return POS_BOARD_CHECK;
}
void update(char ch){
    //printf("%d\n", (int) ch);
    int i,j;
    int curX, nextYidx;
    int plusNum = 1;
    int curPosIdx = 0;
    backgroundBuf[curUserPos.y][curUserPos.x] = preChar;
    int curPosStatus = 0;
    switch (ch)
    {
    case KEY_ENTER:
        //printf("enter");
        curPosStatus = checkPosition(curUserPos.y, curUserPos.x, &curPosIdx);
        if(curPosStatus == POS_DICE_ROLL){
            //roll 일때 위치.
            if(rollCnt <= 0){
                printf("rollCnt : %d\n", rollCnt);
                setStatus("No roll count");
            }
            else{
                for(i = 0;i<DICE_NUM;i++){
                    //printf("show dice idx : %d\n", i);
                    //printf("show dice Y : %d, X : %d\n", dicesShowPos[i].y, dicesShowPos[i].x);
                    if(backgroundBuf[dicesFixPos[i].y][dicesFixPos[i].x] != 'v'){
                        int diceNum = rollDice(dicesShowPos[i].y, dicesShowPos[i].x, 25000);
                        diceData[i] = diceNum;
                    }
                }
                rollCntDecrease();
            }
        }
        else if(curPosStatus == POS_DICE_FIX){
            //주사위를 fix할때 위치.
            flipDiceFix(dicesFixPos[curPosIdx].y, dicesFixPos[curPosIdx].x);
            //backgroundBuf[dicesFixPos[curPosIdx].y][dicesFixPos[curPosIdx].x] = 'v';
        }
        else{
            //보드에서 점수를 얻기위해 체크하는 위치.
            printf("\n board score check\n");
            switch (curPosStatus)
            {
            case ACES:
                printf("ACES\n");
                break;
            
            default:
                break;
            }
        }
        break;
    case KEY_UP:
        //printf("key up\n");
        plusNum = -1;
    case KEY_DOWN:
        //printf("key down\n");
        j = curUserYidx;
        for(i=0;i<boardCheckPosInfo[curUserXidx].posCnt;i++){
            int nextX,nextY;
            nextYidx = (j + plusNum + boardCheckPosInfo[curUserXidx].posCnt)%boardCheckPosInfo[curUserXidx].posCnt;
            //printf("nextYidx = %d\n", nextYidx);
            nextX = boardCheckPosInfo[curUserXidx].startAddr[nextYidx].x;
            nextY = boardCheckPosInfo[curUserXidx].startAddr[nextYidx].y;
            //printf("nextY : %d, nextX : %d\n", nextY, nextX);
            if(backgroundBuf[nextY][nextX] == ' ' || curUserXidx == 1){
                curUserPos.x = nextX;
                curUserPos.y = nextY;
                curUserYidx = nextYidx;
                break;
            }
            j += plusNum;
        }
        break;
    case KEY_RIGHT:
        //left, right는 모드를 바꾸기위해.
        //printf("key right");
    case KEY_LEFT:
        //printf("key left");
        curUserYidx = 0;
        curUserXidx = (curUserXidx+1)%2;
        curUserPos.y = boardCheckPosInfo[curUserXidx].startAddr[0].y;
        curUserPos.x = boardCheckPosInfo[curUserXidx].startAddr[0].x;
        break;
    }

    preChar = backgroundBuf[curUserPos.y][curUserPos.x];
    backgroundBuf[curUserPos.y][curUserPos.x] = 'o';
    
}
void draw(){
    int i;
    //system("clear");
    gotoxy(0, 1);
    printf("\n");
    for(i=0;i<=DARW_HEGITH;i++){
        printf("%s\n",backgroundBuf[i]);
    }
    printf("\n");
    fflush(stdout);
}
int main(){
    int i,j;
    init();
    
    for(i=0;i<12;i++){
        //backgroundBuf[firstCheckPos[i].y][firstCheckPos[i].x] = 'v';
        //backgroundBuf[secondCheckPos[i].y][secondCheckPos[i].x] = 'v';
    }
    for(i =0 ;i<5;i++){
        backgroundBuf[dicesShowPos[i].y][dicesShowPos[i].x] = (char)(i+'1');
        //backgroundBuf[dicesFixPos[i].y][dicesFixPos[i].x] = 'v';
    }
    draw();
    while(1){
        char ch = getkey(0);
        update(ch);
        draw();
    }
    return 0;
}