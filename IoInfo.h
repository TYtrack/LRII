#define MAXBUFSIZE 1024
#include <stdio.h>
#include <string.h>

class IoInfo{
private:
    char name[20];     
    float read_tps; //每秒钟发送到的I/O读请求数
    float write_tps; //每秒钟发送到的I/O写请求数
    float read_block; //每秒读取的block数
    float write_block; //每秒写入的block数
public:
    void getCurrentInfo();
    void getInfoString(char * retMessage,int msgLen);
};

