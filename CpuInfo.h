#include <stdio.h>
#include <string.h>
#include <stdlib.h>

class CpuInfo{
private:
    char name[20];                  //定义一个char类型的数组名name有20个元素
    unsigned int user;              //定义一个无符号的int类型的user
    unsigned int nice;              //定义一个无符号的int类型的nice
    unsigned int system;            //定义一个无符号的int类型的system
    unsigned int idle;              //定义一个无符号的int类型的idle
    unsigned int iowait;
    unsigned int irq;
    unsigned int softirq;
    double cpuRate;
    double calCpuOccupy(CpuInfo *o, CpuInfo *n);

public:
    CpuInfo();
    void getCurrentInfo();
    void getInfoString(char * retMessage,int msgLen);
    void getCpuRateInfo();
    void getCpuRateInfoString(char * retMessage,int msgLen);
};