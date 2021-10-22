#include “MemoryInfo.h”



void MemoryInfo::getCurrentInfo(){

    struct sysinfo s_info;

    if(sysinfo(&s_info)==0)
    {
        this->total_mem =s_info.totalram/1024/1024;
        this->free_mem= s_info.freeram/1024/1024;
        if(this->total_mem==0)
            this->mem_rate=1.0;
        else
            this->mem_rate = (this->total_mem - this->free_mem )/this->total_mem ;

        this->total_swap_mem = s_info.totalswap/1024/1024;
        this->free_swap_mem = s_info.freeswap/1024/1024; 
        if(this->total_swap_mem==0)
            this->swap_mem_rate=0.0;
        else
            this->swap_mem_rate = (this->total_swap_mem - this->free_swap_mem )/this->total_swap_mem ;
        
        this->boot_time = s_info.uptime/60;
        
    }
     
} 

void MemoryInfo::getInfoString(char * retMessage,int msgLen){
    memset(retMessage,'\0',msgLen);
    sprintf(retMessage,"总内存：%8.2f\t空闲内存：%8.2f\t内存使用率：%8.2f\t交换内存：%8.2f\t交换空闲内存：%8.2f\t交换空闲内存使用率：%8.2f\t系统运行时间：%ld\n",
                this->total_mem, this->free_mem, this->mem_rate, this->total_swap_mem, 
                this->free_swap_mem,this->swap_mem_rate,this->boot_time);
    
}

