#include "IoInfo.h"


void IoInfo::getCurrentInfo(){
    this->read_block=0;
    this->write_block=0;
    this->read_tps=0;
    this->write_block=0;

    char cmd[] ="iostat -d -x";
    char buffer[MAXBUFSIZE];
    char a[20];
    float arr[20];
    FILE* pipe = popen(cmd, "r");
    if (!pipe)  return ;
    fgets(buffer, sizeof(buffer), pipe);
    fgets(buffer, sizeof(buffer), pipe);
    fgets(buffer, sizeof(buffer), pipe);
    fgets(buffer, sizeof(buffer), pipe);
    IoInfo temp;
    int i=0;
    while (strcmp("\n",buffer)!=0)
    {
        sscanf(buffer,"%s %f %f %f %f ",temp.name,&temp.read_tps, &temp.write_tps,&temp.read_block,&temp.write_block);    
        this->read_tps+=temp.read_tps;
        this->read_block+=temp.read_block;
        this->write_tps+=temp.write_tps;
        this->write_block+=temp.write_block;
        fgets(buffer, sizeof(buffer), pipe);
    }


    pclose(pipe);
    return ;
    
}


void IoInfo::getInfoString(char * retMessage,int msgLen){
    getCurrentInfo();
    memset(retMessage,'\0',msgLen);
    sprintf(retMessage,"每秒读IO次数：%8.2f\t每秒写IO次数：%8.2f\t每秒读KB数：%8.2f\t每秒写KB数：%8.2f\n", 
                    this->read_tps, this->write_block, this->read_block, this->write_tps);
    
}