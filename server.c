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

char revBuf[4096];
int serverSocket = 0;
int clientSocket[2] = {0};

//서로 같은 운영체제에서 보낼꺼니 공백이나 엔디간 결과는 신경 쓰지 말자.
void decodingProtocol(int protocolMode, char * data, void * voidRevData){
    switch (protocolMode)
    {
    case PROTOCOL_REQ_SYNC:
        memcpy(voidRevData, data, sizeof(ReqSync));
        break;
    default:
        printf("wrong protocol mode in decoding %d\n", protocolMode);
        break;
    }
}

void protocolHandling(int protocolMode, int gameCnt, int recvId, void * voidRevData){
    //짝수는 0, 홀수는 1 idx가 가리키는 소켓으로 보낸다. recvId는 서버에 요청을 보낸 id

    switch (protocolMode)
    {
    case PROTOCOL_REQ_READY:
        AckReady ackReady;
        ackReady.userId =  recvId;  
        write(clientSocket[recvId], (void *)&ackReady, sizeof(ackReady));
        break;
    case PROTOCOL_REQ_SYNC:
        
        break;
    default:
        printf("wrong protocol mode in handling %d\n", protocolMode);
        break;
    }
}

/*
char * encodingProgocol(int protocolMode ,void * data){
    char * res = NULL;  
    switch (protocolMode)
    {
    case PROTOCOL_REQ_READY:
        ReqReady * sendData = (ReqReady *) data;
        break;
    case PROTOCOL_REQ_Sync:
        ReqSync * sendData = (ReqSync *) data;
        break;
    default:
        printf("wrong protocol mode %d\n", protocolMode);
        break;
    }
    return res;
}
*/

int main(int argc, char **argv){

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
        int protocolMode= 0;
        clientSocket[i] = accept(serverSocket, (struct sockaddr*)&(clientAddr[i]), &clientAddr);
        if(clientSocket[i] == -1){
            fprintf(stderr,"%d ", i);
            errorHandling("accept() error");
        }
        int cnt = read(clientSocket[i], &protocolMode, sizeof(int));
        if(cnt != sizeof(int)){
            errorHandling("first. read() != int error");
            exit(1);
        }

        send(clientSocket[i], (void *))
        void * data = encodingProgocol(PROTOCOL_ACK_READY, (void *){1});
        //write(clientSocket[i], )
    }




    return 0;
}