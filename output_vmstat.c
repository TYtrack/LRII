#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/sysinfo.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#define MAXBUFSIZE 1024


int mysystem(char* cmdstring, char* buf, int len)
{
      int   fd[2];
      pid_t pid;
      int   n, count;
      memset(buf, 0, len);
      if (pipe(fd) < 0)
          return -1;
      if ((pid = fork()) < 0)
          return -1;
      else if (pid > 0)     /* parent process */
      {
          close(fd[1]);     /* close write end */
          count = 0;
          while ((n = read(fd[0], buf + count, len)) > 0 && count > len)
              count += n;
          close(fd[0]);
          if (waitpid(pid, NULL, 0) > 0)
              return -1;
      }
      else                  /* child process */
      {
          close(fd[0]);     /* close read end */
          if (fd[1] != STDOUT_FILENO)
          {
              if (dup2(fd[1], STDOUT_FILENO) != STDOUT_FILENO)
              {
                  return -1;
              }
              close(fd[1]);
          }
          if (execl("/bin/sh", "sh", "-c", cmdstring, (char*)0) == -1){
              return -1;
          }
              return -1;
      }
      return 0;
}

static char* cmd_date =(char *)"date +\"[%F %T\"] | tr '\n' ' '>> temp.his";
static char* cmd_cpu_info_head =(char *)"vmstat -a | tail -3 |head -n 2 >> temp.his";
static char* cmd_cpu_info =(char *)"vmstat -a | tail -1 >> temp.his";

typedef struct mem_occupy{
    double total_mem;       //单位为M
    double free_mem;
    double mem_rate;        //内存使用律
    double total_swap_mem;
    double free_swap_mem;
    double swap_mem_rate;  //交换内存使用律
    long boot_time;        //系统运行时间 分钟
}mem_occupy_t;


void getMemoryInfo(mem_occupy_t * mem_info){

    struct sysinfo s_info;

    if(sysinfo(&s_info)==0)
    {
        mem_info->total_mem =s_info.totalram/1024/1024;
        mem_info->free_mem= s_info.freeram/1024/1024;
        mem_info->total_swap_mem = s_info.totalswap/1024/1024;
        mem_info->free_swap_mem = s_info.freeswap/1024/1024; 
        mem_info->boot_time = s_info.uptime/60;

        mem_info->mem_rate = 1-(mem_info->free_mem - mem_info->total_mem );
        mem_info->swap_mem_rate = 1-(mem_info->free_swap_mem - mem_info->total_swap_mem );
        
    }
    
} 


typedef struct cpu_occupy          //定义一个cpu occupy的结构体
{
    char name[20];                  //定义一个char类型的数组名name有20个元素
    unsigned int user;              //定义一个无符号的int类型的user
    unsigned int nice;              //定义一个无符号的int类型的nice
    unsigned int system;            //定义一个无符号的int类型的system
    unsigned int idle;              //定义一个无符号的int类型的idle
    unsigned int iowait;
    unsigned int irq;
    unsigned int softirq;
}cpu_occupy_t;


void get_cpu_occupy (cpu_occupy_t *cpust)
{
    FILE *fd;
    int n;
    char buff[256];
    cpu_occupy_t *cpu_occupy;
    cpu_occupy=cpust;
 
    fd = fopen ("/proc/stat", "r");
    if(fd == NULL)
    {
            perror("fopen:");
            exit (0);
    }
    fgets (buff, sizeof(buff), fd);
 
    sscanf (buff, "%s %u %u %u %u %u %u %u", cpu_occupy->name, &cpu_occupy->user, &cpu_occupy->nice,&cpu_occupy->system, &cpu_occupy->idle ,&cpu_occupy->iowait,&cpu_occupy->irq,&cpu_occupy->softirq);
 
    fclose(fd);
}

double cal_cpu_occupy (cpu_occupy_t *o, cpu_occupy_t *n)
{
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

double get_sysCpuUsage()
{
    cpu_occupy_t cpu_stat1;
    cpu_occupy_t cpu_stat2;
    double cpu;
    get_cpu_occupy(&cpu_stat1);
    sleep(2);
    //第二次获取cpu使用情况
    get_cpu_occupy(&cpu_stat2);
 
    //计算cpu使用率
    cpu = cal_cpu_occupy(&cpu_stat1, &cpu_stat2);
 
    return cpu;
}


typedef struct io_occupy          //定义一个 io occupy的结构体
{
    char name[20];     
    float read_tps; //每秒钟发送到的I/O读请求数
    float write_tps; //每秒钟发送到的I/O写请求数
    float read_block; //每秒读取的block数
    float write_block; //每秒写入的block数

}io_occupy_t;

void get_io_occupy( io_occupy_t * io_info){
    io_info->read_block=0;
    io_info->write_block=0;
    io_info->read_tps=0;
    io_info->write_block=0;

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
    io_occupy_t temp;
    while (strcmp("\n",buffer)!=0)
    {
        sscanf(buffer,"%s %f %f %f %f ",temp.name,&temp.read_tps, &temp.write_tps,&temp.read_block,&temp.write_block);    
        io_info->read_tps+=temp.read_tps;
        io_info->read_block+=temp.read_block;
        io_info->write_tps+=temp.write_tps;
        io_info->write_block+=temp.write_block;
        fgets(buffer, sizeof(buffer), pipe);
    }
    
    printf("IO使用情况：\t %f %f %f %f\n",io_info->read_tps,io_info->write_block,io_info->read_block,io_info->write_tps);
    pclose(pipe);
    return ;
    
}


int main(){
    int i=0;
    struct sysinfo s_info;
    char info_buff[100];
    io_occupy_t * io_info=(io_occupy_t*)malloc(sizeof(io_occupy_t));
    

    while(i<=10){
        printf("cpu占用率：%f\n",get_sysCpuUsage());

        mysystem(cmd_date, info_buff, 100);
        printf("%s",info_buff);
        mysystem(cmd_cpu_info_head, info_buff, 100);
        printf("%s",info_buff);
        mysystem(cmd_cpu_info, info_buff, 100);
        printf("%s",info_buff);

        //getMemoryInfo();
        get_io_occupy(io_info);
        i++;
        i%=10;
        sleep(1);
    }
    

    return 0;
}

