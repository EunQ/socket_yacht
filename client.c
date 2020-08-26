#include<stdio.h>
#include<unistd.h>
#include<string.h>
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

char backgroundBuf[27][80];
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
Pos dicesFixPos[6] = {
    {4, 71 },
    {8 ,71},
    {12, 71 },
    {16, 71 },
    {20, 71 },
    {24, 71 }
};
typedef struct _BoardCheckPosInfo{
    Pos* startAddr;
    int posCnt;
}BoardCheckPosInfo;
BoardCheckPosInfo boardCheckPosInfo[2];
int curUserIdx;
Pos curUserPos;
void gotoxy(int x,int y) {
	printf("\033[%d;%df",y,x);
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

void init(){
    strcpy (backgroundBuf[26]  , "0123456789012345678901234567890123456789012345678901234567890123456789012345678");
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
    strcpy (backgroundBuf[20]  , "|                       |                       |   5 .   [     ]     [ ]     |");
    strcpy (backgroundBuf[21]  , "|                       |                       |                             |");
    strcpy (backgroundBuf[22]  , "|                       |                       |                             |");
    strcpy (backgroundBuf[23]  , "|  Total   :   000      |   Total   :   000     |                             |");
    strcpy (backgroundBuf[24]  , "|                       |                       |    res  :  3 , roll [ ]     |");
    strcpy (backgroundBuf[25]  , "|                       |                       |                             |");
    strcpy (backgroundBuf[26]   , " ----------------------------------------------------------------------------- ");



    boardCheckPosInfo[0].startAddr = firstCheckPos;
    boardCheckPosInfo[0].posCnt = sizeof(firstCheckPos) / sizeof(Pos);

    boardCheckPosInfo[1].startAddr = dicesFixPos;
    boardCheckPosInfo[1].posCnt = sizeof(dicesFixPos) / sizeof(Pos);

    curUserIdx = 1;
    curUserPos.y = boardCheckPosInfo[curUserIdx].startAddr[0].y;
    curUserPos.x = boardCheckPosInfo[curUserIdx].startAddr[0].x;

    update(0);
}
void update(char ch){
    //printf("%d\n", (int) ch);
    backgroundBuf[curUserPos.y][curUserPos.x] = ' ';
    switch (ch)
    {
    case KEY_ENTER:
        printf("enter");
        break;
    case KEY_UP:
        printf("key up");
        
        break;
    case KEY_DOWN:
        printf("key down");
        
        curUserPos.y = boardCheckPosInfo[curUserIdx].startAddr[0].y;
        curUserPos.x = boardCheckPosInfo[curUserIdx].startAddr[0].x;
        break;
    case KEY_RIGHT:
        //left, right는 모드를 바꾸기위해.
        //printf("key right");
    case KEY_LEFT:
        //printf("key left");
        curUserIdx = (curUserIdx+1)%2;
        curUserPos.y = boardCheckPosInfo[curUserIdx].startAddr[0].y;
        curUserPos.x = boardCheckPosInfo[curUserIdx].startAddr[0].x;
        break;
    }
    
    backgroundBuf[curUserPos.y][curUserPos.x] = 'o';
    
}
void draw(){
    int i;
    gotoxy(0, 1);
    printf("\n");
    for(i=0;i<27;i++){
        printf("%s\n",backgroundBuf[i]);
    }
    printf("\n");
    fflush(stdout);
}
int main(){
    int i,j;
    init();
    
    for(i=0;i<12;i++){
        backgroundBuf[firstCheckPos[i].y][firstCheckPos[i].x] = 'v';
        backgroundBuf[secondCheckPos[i].y][secondCheckPos[i].x] = 'v';
    }
    for(i =0 ;i<5;i++){
        backgroundBuf[dicesShowPos[i].y][dicesShowPos[i].x] = '9';
        backgroundBuf[dicesFixPos[i].y][dicesFixPos[i].x] = 'v';
    }
    draw();
    while(1){
        char ch = getkey(0);
        update(ch);
        draw();
    }
    return 0;
}