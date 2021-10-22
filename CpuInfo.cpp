#include "CpuInfo.h"


void CpuInfo::getCurrentInfo(){
    FILE *fd;
    int n;
    char buff[256];
    
 
    fd = fopen ("/proc/stat", "r");
    if(fd == NULL)
    {
            perror("fopen:");
            exit (0);
    }
    fgets (buff, sizeof(buff), fd);
 
    sscanf(buff, "%s %u %u %u %u %u %u %u", this->name, &this->user, &this->nice,&this->system, &this->idle ,&this->iowait,&this->irq,&this->softirq);
    fclose(fd);
}

void CpuInfo::getInfoString(char * retMessage,int msgLen){
    memset(retMessage,'\0',msgLen);
    sprintf(retMessage,"处理器名字：%s\t用户态的运行时间：%8u\t高优先级运行时间：%8u\t内核态运行时间：%8u\tIO等待时间：%8u\t除IO等待的空闲时间：%8u\t硬中断时间：%8u\t软中断时间：%8u\n",
                        this->name, this->user, this->nice,this->system, 
                        this->idle ,this->iowait,this->irq,this->softirq);
   
}

double CpuInfo::calCpuOccupy(cpu_occupy_t *o, cpu_occupy_t *n){
    double od, nd;
    double id, sd;
    double cpu_use ;
    /*
    CPU的即时利用率的计算公式：
    CPU在t1到t2时间段总的使用时间 = ( user2+ nice2+ system2+ idle2+ iowait2+ irq2+ softirq2) - ( user1+ nice1+ system1+ idle1+ iowait1+ irq1+ softirq1)
    CPU在t1到t2时间段空闲使用时间 = (idle2 - idle1)
    CPU在t1到t2时间段即时利用率 =  1 - CPU空闲使用时间 / CPU总的使用时间
    */
   
    nd = (double) (n->user + n->nice + n->system +n->idle+n->softirq+n->iowait+n->irq);//第二次(用户+优先级+系统+空闲)的时间再赋给od
    od = (double) (o->user + o->nice + o->system +o->idle+o->softirq+o->iowait+o->irq);//第一次(用户+优先级+系统+空闲)的时间再赋给od
 
    id = (double) (n->idle);    //用户第一次和第二次的时间之差再赋给id
    sd = (double) (o->idle) ;    //系统第一次和第二次的时间之差再赋给sd
    if((nd-od) != 0)
        cpu_use =100.0 - ((id-sd))/(nd-od)*100.00; //((用户+系统)乖100)除(第一次和第二次的时间差)再赋给g_cpu_used
    else 
        cpu_use = 0;
    return cpu_use;

}