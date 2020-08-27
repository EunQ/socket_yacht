#define PROTOCOL_REQ_READY  1001
#define PROTOCOL_ACK_READY  1002
#define PROTOCOL_REQ_Sync    1003
#define PROTOCOL_ACK_Sync    1004
#define PROTOCOL_ACK_END    1005

typedef struct _ReqReady{
    int protocolMode;
}ReqReady;

typedef struct _AckReady{
    int protocolMode;
    int userId;
}AckReady;

typedef struct _ReqSync{
    int protocolMode;
    int userId; //보낸 아이디
    int checkList;
    int subTotal;
    int bonus;
    int total;
}ReqSync;

typedef struct _AckSync{
    int protocolMode;
    int userId; //싱크를 맞춰야할 아이디
    int checkList;
    int subTotal;
    int bonus;
    int total;
}AckSync;

typedef struct _AckEnd{
    int protocolMode;
}_AckEnd;