#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include<stdlib.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include"protocol.h"

typedef struct _UserScoreInfo{
    int userId;
    int userCheckList; //bit로 저장할것임.
    int userSubTotal;
    int userBonus;
    int userTotal;
}UserScoreInfo;

void errorHandling(char *msg){
    fprintf(stderr, "%s\n", msg);
    exit(1);
}

void * encodingProgocol(int protocalMode ,void * data){
    void * res = NULL;  


    return res;
}

int main(int argc, char **argv){

    int serverSocket = 0;
    int clientSocket[2] = {0};
    struct sockaddr_in serverAddr;
    struct sockaddr_in clientAddr[2];
    int clientAddrSize = 0;
    int i,j;

    if(argc != 2){
        printf("Usage : %s <port> \n", argv[0]);
        exit(1);
    }
    
    serverSocket = socket(PF_INET, SOCK_STREAM, 0); 
    if(serverSocket == -1){
        errorHandling("socket() error");
    }   
    
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddr.sin_port = htons(atoi(argv[1]));

    if(bind(serverSocket, (struct sockaddr*) &serverAddr, sizeof(serverAddr))==-1){
        errorHandling("bind() error");
    }   

    if(listen(serverSocket, 5) == -1){
        errorHandling("listen() error");
    }   

    
    clientAddrSize = sizeof(clientAddr[0]);
    for(i=0;i<2;i++){
        clientSocket[i] = accept(serverSocket, (struct sockaddr*)&(clientAddr[i]), &clientAddr);
        if(clientSocket[i] == -1){
            fprintf(stderr,"%d ", i);
            errorHandling("accept() error");
        }

        void * data = encodingProgocol(PROTOCOL_ACK_READY, (void *){1});
        //write(clientSocket[i], )
    }




    return 0;
}