#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<time.h>

void gotoxy(int x,int y) {
	printf("\033[%d;%df",y,x);
	fflush(stdout);
}
int main(){
    srand(time(NULL));
    int num = 0;
    for(int i=0;i<25000;i++){
        gotoxy(0,1);
        num = (rand()%6)+1;
        printf("%d", num);
        sleep(i);
    }

    return 0;
}