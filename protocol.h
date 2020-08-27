#define PROTOCOL_REQ_READY  1001
#define PROTOCOL_ACK_READY  1002
#define PROTOCOL_REQ_SYNC   1003
#define PROTOCOL_ACK_SYNC   1004
#define PROTOCOL_ACK_END    1005

/*
typedef struct _ReqReady{
    int protocolMode;
}ReqReady;

typedef struct _AckEnd{
}_AckEnd;
*/

typedef struct _AckReady{
    int userId;
}AckReady;

typedef struct _ReqSync{
    int userId; //보낸 아이디
    int addCheckIdx;
    int addCheckData;
    int subTotal;
    int bonus;
    int total;
}ReqSync;

typedef struct _AckSync{
    int userId; //싱크를 맞춰야할 아이디
    int addCheckIdx;
    int addCheckData;
    int subTotal;
    int bonus;
    int total;
}AckSync;
