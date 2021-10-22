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
    void getCurrentInfo();
    void getInfoString(char * retMessage,int msgLen);
    
    void getCpuRateInfo(CpuInfo *cpu_info )
    {
        this->cpu_rate = cal_cpu_occupy(this,cpu_info);   
    }
    
    void getCpuRateInfoString(char * retMessage,int msgLen){
        memset(retMessage,'\0',msgLen);
        sprintf(retMessage,"CPU使用率：%8.2f\n",cpuRate);
    }


}