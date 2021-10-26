#include <stdio.h>
#include <string.h>
#include <stdlib.h>


class NetInfo{
private:
    char name[20];                  //定义一个char类型的数组名name有20个元素
    float rxpck;             
    float txpck;         
    float rxKB;
    float txKB;
    float rxcmp;
    float txcmp;
    float rxmcst;
    float ifutil;

public:
    void getCurrentInfo(int dev);
    void getInfoString(char * retMessage,int msgLen,int dev=0);
};