#include "NetInfo.h"

NetInfo::NetInfo(float rxpck,float txpck,float rxKB,float txKB,float rxcmp,float txcmp,float rxmcst){
    this->rxpck = rxpck;
    this->txpck = txpck;
    this->rxKB = rxKB;
    this->txKB = txKB;
    this->rxcmp = rxcmp;
    this->txcmp = txcmp;
    this->rxmcst = rxmcst;
}
    
void NetInfo::getCurrentInfo(int dev){
    char cmd[] ="sar -n DEV 1 1";
    char buffer[512];
    
    FILE* pipe = popen(cmd, "r");
    if (!pipe)  return ;
    for(int i=1;i<=8+dev;i++)
        fgets(buffer, sizeof(buffer), pipe);

    if (strcmp("\n",buffer)!=0)
    {
        sscanf(buffer,"%*s %s %f %f %f %f %f %f %f",this->name,&this->rxpck,&this->txpck,&this->rxKB,&this->txKB
                        ,&this->rxcmp,&this->txcmp,&this->rxmcst);    

    }


    pclose(pipe);
    return ;
    
}

void NetInfo::loadThreshold(){
    const char* ConfigFile;
    if(threshold_netInfo!=nullptr)
        delete threshold_netInfo;
    threshold_netInfo = new NetInfo();
    threshold_netInfo->
    int port = configSettings.Read("email", 1);  

    float max_cpu_rate = configSettings.Read("max_cpu_rate", ipAddress);  

}

void monitor(const char* ConfigFile){
    Config configSettings(ConfigFile);  

    int port = configSettings.Read("email", 1);  

    float max_cpu_rate = configSettings.Read("max_cpu_rate", ipAddress);  


}   


void NetInfo::getInfoString(char * retMessage,int msgLen,int dev){
    getCurrentInfo(dev);
    memset(retMessage,'\0',msgLen);
    sprintf(retMessage,"网络接口：%8s\t每秒钟接收的数据包：%8.2f\t每秒钟发送的数据包：%8.2f\t每秒钟接收的kB数：%8.2f\t\
                        每秒钟发送的kB数：%8.2f\t每秒钟接收的压缩数据包：%8.2f\t每秒钟发送的压缩数据包：%8.2f\t每秒钟接收的多播数据包：%8.2f\n", 
                    this->name,this->rxpck,this->txpck,this->rxKB,this->txKB
                        ,this->rxcmp,this->txcmp,this->rxmcst);
    

}